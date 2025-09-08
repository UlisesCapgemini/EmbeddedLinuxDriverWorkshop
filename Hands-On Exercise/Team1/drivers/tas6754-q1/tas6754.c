// SPDX-License-Identifier: GPL-2.0
/*
 * ALSA SoC Texas Instruments TAS6754 Quad-Channel Audio Amplifier
 *
 * Copyright (C) 2023 Your Name <your.email@example.com>
 * Based on TAS6424 driver by Andreas Dannenberg <dannenberg@ti.com>
 * and Andrew F. Davis <afd@ti.com>
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>

#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>

#include "tas6754.h"

/* Define how often to check (and clear) the fault status register (in ms) */
#define TAS6754_FAULT_CHECK_INTERVAL 200

static const char * const tas6754_supply_names[] = {
	"dvdd", /* Digital power supply. Connect to 3.3-V supply. */
	"vbat", /* Supply used for higher voltage analog circuits. */
	"pvdd", /* Class-D amp output FETs supply. */
};
#define TAS6754_NUM_SUPPLIES ARRAY_SIZE(tas6754_supply_names)

struct tas6754_data {
	struct device *dev;
	struct regmap *regmap;
	struct regulator_bulk_data supplies[TAS6754_NUM_SUPPLIES];
	struct delayed_work fault_check_work;
	unsigned int last_cfault;
	unsigned int last_fault1;
	unsigned int last_fault2;
	unsigned int last_warn;
	struct gpio_desc *standby_gpio;
	struct gpio_desc *mute_gpio;
};

/*
 * DAC digital volumes. From -103.5 to 24 dB in 0.5 dB steps. Note that
 * setting the gain below -100 dB (register value <0x7) is effectively a MUTE
 * as per device datasheet.
 */
static DECLARE_TLV_DB_SCALE(dac_tlv, -10350, 50, 0);

static const struct snd_kcontrol_new tas6754_snd_controls[] = {
	SOC_SINGLE_TLV("Speaker Driver CH1 Playback Volume",
		       TAS6754_CH1_VOL_CTRL, 0, 0xff, 0, dac_tlv),
	SOC_SINGLE_TLV("Speaker Driver CH2 Playback Volume",
		       TAS6754_CH2_VOL_CTRL, 0, 0xff, 0, dac_tlv),
	SOC_SINGLE_TLV("Speaker Driver CH3 Playback Volume",
		       TAS6754_CH3_VOL_CTRL, 0, 0xff, 0, dac_tlv),
	SOC_SINGLE_TLV("Speaker Driver CH4 Playback Volume",
		       TAS6754_CH4_VOL_CTRL, 0, 0xff, 0, dac_tlv),
	SOC_SINGLE_STROBE("Auto Diagnostics Switch", TAS6754_DC_DIAG_CTRL1,
			  TAS6754_LDGBYPASS_SHIFT, 1),
};

static int tas6754_dac_event(struct snd_soc_dapm_widget *w,
			     struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);

	dev_dbg(component->dev, "%s() event=0x%0x\n", __func__, event);

	if (event & SND_SOC_DAPM_POST_PMU) {
		/* Observe codec shutdown-to-active time */
		msleep(12);

		/* Turn on TAS6754 periodic fault checking/handling */
		tas6754->last_fault1 = 0;
		tas6754->last_fault2 = 0;
		tas6754->last_warn = 0;
		schedule_delayed_work(&tas6754->fault_check_work,
				      msecs_to_jiffies(TAS6754_FAULT_CHECK_INTERVAL));
	} else if (event & SND_SOC_DAPM_PRE_PMD) {
		/* Disable TAS6754 periodic fault checking/handling */
		cancel_delayed_work_sync(&tas6754->fault_check_work);
	}

	return 0;
}

static const struct snd_soc_dapm_widget tas6754_dapm_widgets[] = {
	SND_SOC_DAPM_AIF_IN("DAC IN", "Playback", 0, SND_SOC_NOPM, 0, 0),
	SND_SOC_DAPM_DAC_E("DAC", NULL, SND_SOC_NOPM, 0, 0, tas6754_dac_event,
			   SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
	SND_SOC_DAPM_OUTPUT("OUT")
};

static const struct snd_soc_dapm_route tas6754_audio_map[] = {
	{ "DAC", NULL, "DAC IN" },
	{ "OUT", NULL, "DAC" },
};


static int tas6754_hw_params(struct snd_pcm_substream *substream,
			     struct snd_pcm_hw_params *params,
			     struct snd_soc_dai *dai)
{
	struct snd_soc_component *component = dai->component;
	unsigned int rate = params_rate(params);
	unsigned int width = params_width(params);
	u8 sap_ctrl = 0;
	int ret;

	dev_dbg(component->dev, "%s() rate=%u width=%u\n", __func__, rate, width);

	/* Set the sample rate */
	switch (rate) {
	case 44100:
		sap_ctrl |= TAS6754_SAP_RATE_44100;
		break;
	case 48000:
		sap_ctrl |= TAS6754_SAP_RATE_48000;
		break;
	case 96000:
		sap_ctrl |= TAS6754_SAP_RATE_96000;
		break;
	case 192000:
		sap_ctrl |= TAS6754_SAP_RATE_192000;
		break;
	default:
		dev_err(component->dev, "unsupported sample rate: %u\n", rate);
		return -EINVAL;
	}

	/* Set the sample width */
	switch (width) {
	case 16:
		sap_ctrl |= TAS6754_SAP_TDM_SLOT_SZ_16;
		break;
	case 24:
		/* Default is 24-bit, no need to set any bits */
		break;
	default:
		dev_err(component->dev, "unsupported sample width: %u\n", width);
		return -EINVAL;
	}

	/* Update the SAP control register with the new settings */
	ret = snd_soc_component_update_bits(component, TAS6754_SAP_CTRL,
			    TAS6754_SAP_RATE_MASK |
			    TAS6754_SAP_TDM_SLOT_SZ_16,
			    sap_ctrl);
	if (ret < 0) {
		dev_err(component->dev, "failed to update SAP_CTRL: %d\n", ret);
		return ret;
	}

	/* 
	 * For higher sample rates (96kHz and 192kHz), we may need to adjust
	 * the PWM frequency to maintain good audio quality
	 */
	if (rate >= 96000) {
		/* Set PWM frequency to the highest setting for high sample rates */
		ret = snd_soc_component_update_bits(component, TAS6754_MISC_CTRL2,
				TAS6754_PWM_FREQ_MASK,
				0x6 << 4); /* Set to 44x or 48x fs */
		if (ret < 0) {
			dev_err(component->dev, "failed to update PWM frequency: %d\n", ret);
			return ret;
		}
	}

	return 0;
}





static int tas6754_set_dai_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
	struct snd_soc_component *component = dai->component;
	u8 serial_format = 0;

	dev_dbg(component->dev, "%s() fmt=0x%0x\n", __func__, fmt);

	/* clock masters */
	switch (fmt & SND_SOC_DAIFMT_CLOCK_PROVIDER_MASK) {
	case SND_SOC_DAIFMT_CBC_CFC:
		break;
	default:
		dev_err(component->dev, "Invalid DAI clocking\n");
		return -EINVAL;
	}

	/* signal polarity */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	default:
		dev_err(component->dev, "Invalid DAI clock signal polarity\n");
		return -EINVAL;
	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		serial_format |= TAS6754_SAP_I2S;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		serial_format |= TAS6754_SAP_DSP;
		break;
	case SND_SOC_DAIFMT_DSP_B:
		/*
		 * We can use the fact that the TAS6754 does not care about the
		 * LRCLK duty cycle during TDM to receive DSP_B formatted data
		 * in LEFTJ mode (no delaying of the 1st data bit).
		 */
		serial_format |= TAS6754_SAP_LEFTJ;
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		serial_format |= TAS6754_SAP_LEFTJ;
		break;
	default:
		dev_err(component->dev, "Invalid DAI interface format\n");
		return -EINVAL;
	}

	snd_soc_component_update_bits(component, TAS6754_SAP_CTRL,
			    TAS6754_SAP_FMT_MASK, serial_format);

	return 0;
}


static int tas6754_set_dai_tdm_slot(struct snd_soc_dai *dai,
				    unsigned int tx_mask, unsigned int rx_mask,
				    int slots, int slot_width)
{
	struct snd_soc_component *component = dai->component;
	unsigned int first_slot, last_slot;
	bool sap_tdm_slot_last;
	u8 tdm_config = 0;
	int ret;

	dev_dbg(component->dev, "%s() tx_mask=0x%x rx_mask=0x%x slots=%d slot_width=%d\n", 
		__func__, tx_mask, rx_mask, slots, slot_width);

	if (!tx_mask || !rx_mask)
		return 0; /* nothing needed to disable TDM mode */

	/*
	 * Determine the first slot and last slot that is being requested so
	 * we'll be able to more easily enforce certain constraints as the
	 * TAS6754's TDM interface is not fully configurable.
	 */
	first_slot = __ffs(tx_mask);
	last_slot = __fls(rx_mask);

	if (last_slot - first_slot != 4) {
		dev_err(component->dev, "tdm mask must cover 4 contiguous slots\n");
		return -EINVAL;
	}

	switch (first_slot) {
	case 0:
		sap_tdm_slot_last = false;
		break;
	case 4:
		sap_tdm_slot_last = true;
		break;
	default:
		dev_err(component->dev, "tdm mask must start at slot 0 or 4\n");
		return -EINVAL;
	}

	/* Configure TDM slot selection */
	ret = snd_soc_component_update_bits(component, TAS6754_SAP_CTRL, 
			    TAS6754_SAP_TDM_SLOT_LAST,
			    sap_tdm_slot_last ? TAS6754_SAP_TDM_SLOT_LAST : 0);
	if (ret < 0) {
		dev_err(component->dev, "failed to update TDM slot selection: %d\n", ret);
		return ret;
	}

	/* Configure TDM slot width */
	switch (slot_width) {
	case 16:
		tdm_config |= TAS6754_TDM_SLOT_WIDTH_16;
		break;
	case 24:
		tdm_config |= TAS6754_TDM_SLOT_WIDTH_24;
		break;
	case 32:
		tdm_config |= TAS6754_TDM_SLOT_WIDTH_32;
		break;
	default:
		dev_err(component->dev, "unsupported TDM slot width: %d\n", slot_width);
		return -EINVAL;
	}

	/* Configure total number of TDM slots */
	switch (slots) {
	case 4:
		tdm_config |= TAS6754_TDM_SLOTS_4;
		break;
	case 8:
		tdm_config |= TAS6754_TDM_SLOTS_8;
		break;
	case 12:
		tdm_config |= TAS6754_TDM_SLOTS_12;
		break;
	case 16:
		tdm_config |= TAS6754_TDM_SLOTS_16;
		break;
	default:
		dev_err(component->dev, "unsupported number of TDM slots: %d\n", slots);
		return -EINVAL;
	}

	/* Update TDM configuration register */
	ret = snd_soc_component_update_bits(component, TAS6754_SAP_CTRL,
			    TAS6754_TDM_SLOT_WIDTH_MASK | TAS6754_TDM_SLOTS_MASK,
			    tdm_config);
	if (ret < 0) {
		dev_err(component->dev, "failed to update TDM configuration: %d\n", ret);
		return ret;
	}

	return 0;
}





static int tas6754_mute(struct snd_soc_dai *dai, int mute, int direction)
{
	struct snd_soc_component *component = dai->component;
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
	unsigned int val;

	dev_dbg(component->dev, "%s() mute=%d\n", __func__, mute);

	if (tas6754->mute_gpio) {
		gpiod_set_value_cansleep(tas6754->mute_gpio, mute);
		return 0;
	}

	if (mute)
		val = TAS6754_ALL_STATE_MUTE;
	else
		val = TAS6754_ALL_STATE_PLAY;

	snd_soc_component_write(component, TAS6754_CH_STATE_CTRL, val);

	return 0;
}

static int tas6754_power_off(struct snd_soc_component *component)
{
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
	int ret;

	snd_soc_component_write(component, TAS6754_CH_STATE_CTRL, TAS6754_ALL_STATE_HIZ);

	regcache_cache_only(tas6754->regmap, true);
	regcache_mark_dirty(tas6754->regmap);

	ret = regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies),
				     tas6754->supplies);
	if (ret < 0) {
		dev_err(component->dev, "failed to disable supplies: %d\n", ret);
		return ret;
	}

	return 0;
}

static int tas6754_power_on(struct snd_soc_component *component)
{
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
	int ret;
	u8 chan_states;
	int no_auto_diags = 0;
	unsigned int reg_val;

	if (!regmap_read(tas6754->regmap, TAS6754_DC_DIAG_CTRL1, &reg_val))
		no_auto_diags = reg_val & TAS6754_LDGBYPASS_MASK;

	ret = regulator_bulk_enable(ARRAY_SIZE(tas6754->supplies),
				    tas6754->supplies);
	if (ret < 0) {
		dev_err(component->dev, "failed to enable supplies: %d\n", ret);
		return ret;
	}

	regcache_cache_only(tas6754->regmap, false);

	ret = regcache_sync(tas6754->regmap);
	if (ret < 0) {
		dev_err(component->dev, "failed to sync regcache: %d\n", ret);
		return ret;
	}

	if (tas6754->mute_gpio) {
		gpiod_set_value_cansleep(tas6754->mute_gpio, 0);
		/*
		 * channels are muted via the mute pin.  Don't also mute
		 * them via the registers so that subsequent register
		 * access is not necessary to un-mute the channels
		 */
		chan_states = TAS6754_ALL_STATE_PLAY;
	} else {
		chan_states = TAS6754_ALL_STATE_MUTE;
	}
	snd_soc_component_write(component, TAS6754_CH_STATE_CTRL, chan_states);

	/* any time we come out of HIZ, the output channels automatically run DC
	 * load diagnostics if autodiagnotics are enabled. wait here until this
	 * completes.
	 */
	if (!no_auto_diags)
		msleep(230);

	return 0;
}

static int tas6754_set_bias_level(struct snd_soc_component *component,
				  enum snd_soc_bias_level level)
{
	dev_dbg(component->dev, "%s() level=%d\n", __func__, level);

	switch (level) {
	case SND_SOC_BIAS_ON:
	case SND_SOC_BIAS_PREPARE:
		break;
	case SND_SOC_BIAS_STANDBY:
		if (snd_soc_component_get_bias_level(component) == SND_SOC_BIAS_OFF)
			tas6754_power_on(component);
		break;
	case SND_SOC_BIAS_OFF:
		tas6754_power_off(component);
		break;
	}

	return 0;
}

static struct snd_soc_component_driver soc_codec_dev_tas6754 = {
	.set_bias_level		= tas6754_set_bias_level,
	.controls		= tas6754_snd_controls,
	.num_controls		= ARRAY_SIZE(tas6754_snd_controls),
	.dapm_widgets		= tas6754_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(tas6754_dapm_widgets),
	.dapm_routes		= tas6754_audio_map,
	.num_dapm_routes	= ARRAY_SIZE(tas6754_audio_map),
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static const struct snd_soc_dai_ops tas6754_speaker_dai_ops = {
	.hw_params	= tas6754_hw_params,
	.set_fmt	= tas6754_set_dai_fmt,
	.set_tdm_slot	= tas6754_set_dai_tdm_slot,
	.mute_stream	= tas6754_mute,
	.no_capture_mute = 1,
};

static struct snd_soc_dai_driver tas6754_dai[] = {
	{
		.name = "tas6754-amplifier",
		.playback = {
			.stream_name = "Playback",
			.channels_min = 1,
			.channels_max = 4,
			.rates = TAS6754_RATES,
			.formats = TAS6754_FORMATS,
		},
		.ops = &tas6754_speaker_dai_ops,
	},
};

static void tas6754_fault_check_work(struct work_struct *work)
{
	struct tas6754_data *tas6754 = container_of(work, struct tas6754_data,
						    fault_check_work.work);
	struct device *dev = tas6754->dev;
	unsigned int reg;
	int ret;

	ret = regmap_read(tas6754->regmap, TAS6754_CHANNEL_FAULT, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read CHANNEL_FAULT register: %d\n", ret);
		goto out;
	}

	if (!reg) {
		tas6754->last_cfault = reg;
		goto check_global_fault1_reg;
	}

	/*
	 * Only flag errors once for a given occurrence. This is needed as
	 * the TAS6754 will take time clearing the fault condition internally
	 * during which we don't want to bombard the system with the same
	 * error message over and over.
	 */
	if ((reg & TAS6754_FAULT_OC_CH1) && !(tas6754->last_cfault & TAS6754_FAULT_OC_CH1))
		dev_crit(dev, "experienced a channel 1 overcurrent fault\n");

	if ((reg & TAS6754_FAULT_OC_CH2) && !(tas6754->last_cfault & TAS6754_FAULT_OC_CH2))
		dev_crit(dev, "experienced a channel 2 overcurrent fault\n");

	if ((reg & TAS6754_FAULT_OC_CH3) && !(tas6754->last_cfault & TAS6754_FAULT_OC_CH3))
		dev_crit(dev, "experienced a channel 3 overcurrent fault\n");

	if ((reg & TAS6754_FAULT_OC_CH4) && !(tas6754->last_cfault & TAS6754_FAULT_OC_CH4))
		dev_crit(dev, "experienced a channel 4 overcurrent fault\n");

	if ((reg & TAS6754_FAULT_DC_CH1) && !(tas6754->last_cfault & TAS6754_FAULT_DC_CH1))
		dev_crit(dev, "experienced a channel 1 DC fault\n");

	if ((reg & TAS6754_FAULT_DC_CH2) && !(tas6754->last_cfault & TAS6754_FAULT_DC_CH2))
		dev_crit(dev, "experienced a channel 2 DC fault\n");

	if ((reg & TAS6754_FAULT_DC_CH3) && !(tas6754->last_cfault & TAS6754_FAULT_DC_CH3))
		dev_crit(dev, "experienced a channel 3 DC fault\n");

	if ((reg & TAS6754_FAULT_DC_CH4) && !(tas6754->last_cfault & TAS6754_FAULT_DC_CH4))
		dev_crit(dev, "experienced a channel 4 DC fault\n");

	/* Store current fault1 value so we can detect any changes next time */
	tas6754->last_cfault = reg;

check_global_fault1_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_GLOB_FAULT1, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read GLOB_FAULT1 register: %d\n", ret);
		goto out;
	}

	/*
	 * Ignore any clock faults as there is no clean way to check for them.
	 * We would need to start checking for those faults *after* the SAIF
	 * stream has been setup, and stop checking *before* the stream is
	 * stopped to avoid any false-positives. However there are no
	 * appropriate hooks to monitor these events.
	 */
	reg &= TAS6754_FAULT_PVDD_OV |
	       TAS6754_FAULT_VBAT_OV |
	       TAS6754_FAULT_PVDD_UV |
	       TAS6754_FAULT_VBAT_UV;

	if (!reg) {
		tas6754->last_fault1 = reg;
		goto check_global_fault2_reg;
	}

	if ((reg & TAS6754_FAULT_PVDD_OV) && !(tas6754->last_fault1 & TAS6754_FAULT_PVDD_OV))
		dev_crit(dev, "experienced a PVDD overvoltage fault\n");

	if ((reg & TAS6754_FAULT_VBAT_OV) && !(tas6754->last_fault1 & TAS6754_FAULT_VBAT_OV))
		dev_crit(dev, "experienced a VBAT overvoltage fault\n");

	if ((reg & TAS6754_FAULT_PVDD_UV) && !(tas6754->last_fault1 & TAS6754_FAULT_PVDD_UV))
		dev_crit(dev, "experienced a PVDD undervoltage fault\n");

	if ((reg & TAS6754_FAULT_VBAT_UV) && !(tas6754->last_fault1 & TAS6754_FAULT_VBAT_UV))
		dev_crit(dev, "experienced a VBAT undervoltage fault\n");

	/* Store current fault1 value so we can detect any changes next time */
	tas6754->last_fault1 = reg;

check_global_fault2_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_GLOB_FAULT2, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read GLOB_FAULT2 register: %d\n", ret);
		goto out;
	}

	reg &= TAS6754_FAULT_OTSD |
	       TAS6754_FAULT_OTSD_CH1 |
	       TAS6754_FAULT_OTSD_CH2 |
	       TAS6754_FAULT_OTSD_CH3 |
	       TAS6754_FAULT_OTSD_CH4;

	if (!reg) {
		tas6754->last_fault2 = reg;
		goto check_warn_reg;
	}

	if ((reg & TAS6754_FAULT_OTSD) && !(tas6754->last_fault2 & TAS6754_FAULT_OTSD))
		dev_crit(dev, "experienced a global overtemp shutdown\n");

	if ((reg & TAS6754_FAULT_OTSD_CH1) && !(tas6754->last_fault2 & TAS6754_FAULT_OTSD_CH1))
		dev_crit(dev, "experienced an overtemp shutdown on CH1\n");

	if ((reg & TAS6754_FAULT_OTSD_CH2) && !(tas6754->last_fault2 & TAS6754_FAULT_OTSD_CH2))
		dev_crit(dev, "experienced an overtemp shutdown on CH2\n");

	if ((reg & TAS6754_FAULT_OTSD_CH3) && !(tas6754->last_fault2 & TAS6754_FAULT_OTSD_CH3))
		dev_crit(dev, "experienced an overtemp shutdown on CH3\n");

	if ((reg & TAS6754_FAULT_OTSD_CH4) && !(tas6754->last_fault2 & TAS6754_FAULT_OTSD_CH4))
		dev_crit(dev, "experienced an overtemp shutdown on CH4\n");

	/* Store current fault2 value so we can detect any changes next time */
	tas6754->last_fault2 = reg;

check_warn_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_WARN, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read WARN register: %d\n", ret);
		goto out;
	}

	reg &= TAS6754_WARN_VDD_UV |
	       TAS6754_WARN_VDD_POR |
	       TAS6754_WARN_VDD_OTW |
	       TAS6754_WARN_VDD_OTW_CH1 |
	       TAS6754_WARN_VDD_OTW_CH2 |
	       TAS6754_WARN_VDD_OTW_CH3 |
	       TAS6754_WARN_VDD_OTW_CH4;

	if (!reg) {
		tas6754->last_warn = reg;
		goto out;
	}

	if ((reg & TAS6754_WARN_VDD_UV) && !(tas6754->last_warn & TAS6754_WARN_VDD_UV))
		dev_warn(dev, "experienced a VDD under voltage condition\n");

	if ((reg & TAS6754_WARN_VDD_POR) && !(tas6754->last_warn & TAS6754_WARN_VDD_POR))
		dev_warn(dev, "experienced a VDD POR condition\n");

	if ((reg & TAS6754_WARN_VDD_OTW) && !(tas6754->last_warn & TAS6754_WARN_VDD_OTW))
		dev_warn(dev, "experienced a global overtemp warning\n");

	if ((reg & TAS6754_WARN_VDD_OTW_CH1) && !(tas6754->last_warn & TAS6754_WARN_VDD_OTW_CH1))
		dev_warn(dev, "experienced an overtemp warning on CH1\n");

	if ((reg & TAS6754_WARN_VDD_OTW_CH2) && !(tas6754->last_warn & TAS6754_WARN_VDD_OTW_CH2))
		dev_warn(dev, "experienced an overtemp warning on CH2\n");

	if ((reg & TAS6754_WARN_VDD_OTW_CH3) && !(tas6754->last_warn & TAS6754_WARN_VDD_OTW_CH3))
		dev_warn(dev, "experienced an overtemp warning on CH3\n");

	if ((reg & TAS6754_WARN_VDD_OTW_CH4) && !(tas6754->last_warn & TAS6754_WARN_VDD_OTW_CH4))
		dev_warn(dev, "experienced an overtemp warning on CH4\n");

	/* Store current warn value so we can detect any changes next time */
	tas6754->last_warn = reg;

	/* Clear any warnings by toggling the CLEAR_FAULT control bit */
	ret = regmap_write_bits(tas6754->regmap, TAS6754_MISC_CTRL3,
				TAS6754_CLEAR_FAULT, TAS6754_CLEAR_FAULT);
	if (ret < 0)
		dev_err(dev, "failed to write MISC_CTRL3 register: %d\n", ret);

	ret = regmap_write_bits(tas6754->regmap, TAS6754_MISC_CTRL3,
				TAS6754_CLEAR_FAULT, 0);
	if (ret < 0)
		dev_err(dev, "failed to write MISC_CTRL3 register: %d\n", ret);

out:
	/* Schedule the next fault check at the specified interval */
	schedule_delayed_work(&tas6754->fault_check_work,
			      msecs_to_jiffies(TAS6754_FAULT_CHECK_INTERVAL));
}

static const struct reg_default tas6754_reg_defaults[] = {
	{ TAS6754_MODE_CTRL,		0x00 },
	{ TAS6754_MISC_CTRL1,		0x32 },
	{ TAS6754_MISC_CTRL2,		0x62 },
	{ TAS6754_SAP_CTRL,		0x04 },
	{ TAS6754_CH_STATE_CTRL,	0x55 },
	{ TAS6754_CH1_VOL_CTRL,		0xcf },
	{ TAS6754_CH2_VOL_CTRL,		0xcf },
	{ TAS6754_CH3_VOL_CTRL,		0xcf },
	{ TAS6754_CH4_VOL_CTRL,		0xcf },
	{ TAS6754_DC_DIAG_CTRL1,	0x00 },
	{ TAS6754_DC_DIAG_CTRL2,	0x11 },
	{ TAS6754_DC_DIAG_CTRL3,	0x11 },
	{ TAS6754_PIN_CTRL,		0xff },
	{ TAS6754_AC_DIAG_CTRL1,	0x00 },
	{ TAS6754_MISC_CTRL3,		0x00 },
	{ TAS6754_CLIP_CTRL,		0x01 },
	{ TAS6754_CLIP_WINDOW,		0x14 },
	{ TAS6754_CLIP_WARN,		0x00 },
	{ TAS6754_CBC_STAT,		0x00 },
	{ TAS6754_MISC_CTRL4,		0x40 },
};

static bool tas6754_is_writable_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case TAS6754_MODE_CTRL:
	case TAS6754_MISC_CTRL1:
	case TAS6754_MISC_CTRL2:
	case TAS6754_SAP_CTRL:
	case TAS6754_CH_STATE_CTRL:
	case TAS6754_CH1_VOL_CTRL:
	case TAS6754_CH2_VOL_CTRL:
	case TAS6754_CH3_VOL_CTRL:
	case TAS6754_CH4_VOL_CTRL:
	case TAS6754_DC_DIAG_CTRL1:
	case TAS6754_DC_DIAG_CTRL2:
	case TAS6754_DC_DIAG_CTRL3:
	case TAS6754_PIN_CTRL:
	case TAS6754_AC_DIAG_CTRL1:
	case TAS6754_MISC_CTRL3:
	case TAS6754_CLIP_CTRL:
	case TAS6754_CLIP_WINDOW:
	case TAS6754_CLIP_WARN:
	case TAS6754_CBC_STAT:
	case TAS6754_MISC_CTRL4:
		return true;
	default:
		return false;
	}
}

static bool tas6754_is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case TAS6754_DC_LOAD_DIAG_REP12:
	case TAS6754_DC_LOAD_DIAG_REP34:
	case TAS6754_DC_LOAD_DIAG_REPLO:
	case TAS6754_CHANNEL_STATE:
	case TAS6754_CHANNEL_FAULT:
	case TAS6754_GLOB_FAULT1:
	case TAS6754_GLOB_FAULT2:
	case TAS6754_WARN:
	case TAS6754_AC_LOAD_DIAG_REP1:
	case TAS6754_AC_LOAD_DIAG_REP2:
	case TAS6754_AC_LOAD_DIAG_REP3:
	case TAS6754_AC_LOAD_DIAG_REP4:
		return true;
	default:
		return false;
	}
}

static const struct regmap_config tas6754_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,

	.writeable_reg = tas6754_is_writable_reg,
	.volatile_reg = tas6754_is_volatile_reg,

	.max_register = TAS6754_MAX,
	.reg_defaults = tas6754_reg_defaults,
	.num_reg_defaults = ARRAY_SIZE(tas6754_reg_defaults),
	.cache_type = REGCACHE_RBTREE,
};

#if IS_ENABLED(CONFIG_OF)
static const struct of_device_id tas6754_of_ids[] = {
	{ .compatible = "ti,tas6754", },
	{ },
};
MODULE_DEVICE_TABLE(of, tas6754_of_ids);
#endif

static int tas6754_i2c_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct tas6754_data *tas6754;
	int ret;
	int i;

	tas6754 = devm_kzalloc(dev, sizeof(*tas6754), GFP_KERNEL);
	if (!tas6754)
		return -ENOMEM;
	dev_set_drvdata(dev, tas6754);

	tas6754->dev = dev;

	tas6754->regmap = devm_regmap_init_i2c(client, &tas6754_regmap_config);
	if (IS_ERR(tas6754->regmap)) {
		ret = PTR_ERR(tas6754->regmap);
		dev_err(dev, "unable to allocate register map: %d\n", ret);
		return ret;
	}

	/*
	 * Get control of the standby pin and set it LOW to take the codec
	 * out of the stand-by mode.
	 * Note: The actual pin polarity is taken care of in the GPIO lib
	 * according the polarity specified in the DTS.
	 */
	tas6754->standby_gpio = devm_gpiod_get_optional(dev, "standby",
						      GPIOD_OUT_LOW);
	if (IS_ERR(tas6754->standby_gpio)) {
		if (PTR_ERR(tas6754->standby_gpio) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_info(dev, "failed to get standby GPIO: %ld\n",
			PTR_ERR(tas6754->standby_gpio));
		tas6754->standby_gpio = NULL;
	}

	/*
	 * Get control of the mute pin and set it HIGH in order to start with
	 * all the output muted.
	 * Note: The actual pin polarity is taken care of in the GPIO lib
	 * according the polarity specified in the DTS.
	 */
	tas6754->mute_gpio = devm_gpiod_get_optional(dev, "mute",
						      GPIOD_OUT_HIGH);
	if (IS_ERR(tas6754->mute_gpio)) {
		if (PTR_ERR(tas6754->mute_gpio) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_info(dev, "failed to get nmute GPIO: %ld\n",
			PTR_ERR(tas6754->mute_gpio));
		tas6754->mute_gpio = NULL;
	}

	for (i = 0; i < ARRAY_SIZE(tas6754->supplies); i++)
		tas6754->supplies[i].supply = tas6754_supply_names[i];
	ret = devm_regulator_bulk_get(dev, ARRAY_SIZE(tas6754->supplies),
				      tas6754->supplies);
	if (ret) {
		dev_err(dev, "unable to request supplies: %d\n", ret);
		return ret;
	}

	ret = regulator_bulk_enable(ARRAY_SIZE(tas6754->supplies),
				    tas6754->supplies);
	if (ret) {
		dev_err(dev, "unable to enable supplies: %d\n", ret);
		return ret;
	}

	/* Reset device to establish well-defined startup state */
	ret = regmap_update_bits(tas6754->regmap, TAS6754_MODE_CTRL,
				 TAS6754_RESET, TAS6754_RESET);
	if (ret) {
		dev_err(dev, "unable to reset device: %d\n", ret);
		goto disable_regs;
	}

	INIT_DELAYED_WORK(&tas6754->fault_check_work, tas6754_fault_check_work);

	ret = devm_snd_soc_register_component(dev, &soc_codec_dev_tas6754,
				     tas6754_dai, ARRAY_SIZE(tas6754_dai));
	if (ret < 0) {
		dev_err(dev, "unable to register codec: %d\n", ret);
		goto disable_regs;
	}

	return 0;

disable_regs:
	regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
	return ret;
}

static void tas6754_i2c_remove(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct tas6754_data *tas6754 = dev_get_drvdata(dev);
	int ret;

	cancel_delayed_work_sync(&tas6754->fault_check_work);

	/* put the codec in stand-by */
	if (tas6754->standby_gpio)
		gpiod_set_value_cansleep(tas6754->standby_gpio, 1);

	ret = regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies),
				     tas6754->supplies);
	if (ret < 0)
		dev_err(dev, "unable to disable supplies: %d\n", ret);
}

static const struct i2c_device_id tas6754_i2c_ids[] = {
	{ "tas6754", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, tas6754_i2c_ids);

static struct i2c_driver tas6754_i2c_driver = {
	.driver = {
		.name = "tas6754",
		.of_match_table = of_match_ptr(tas6754_of_ids),
	},
	.probe = tas6754_i2c_probe,
	.remove = tas6754_i2c_remove,
	.id_table = tas6754_i2c_ids,
};
module_i2c_driver(tas6754_i2c_driver);

MODULE_AUTHOR("Your Name <your.email@example.com>");
MODULE_DESCRIPTION("TAS6754 Audio amplifier driver");
MODULE_LICENSE("GPL v2");