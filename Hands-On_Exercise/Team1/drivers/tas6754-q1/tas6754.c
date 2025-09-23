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

/* Order of array elements for supplies follows power-down sequence from datasheet, which is recommended to disable PVDD and VBAT first, then DVDD */
static const char * const tas6754_supply_names[] = {
    "pvdd", /* Class-D amp output FETs supply. */
    "vbat", /* Supply used for higher voltage analog circuits. */
    "dvdd", /* Digital power supply. Connect to 3.3-V supply. */
};
#define TAS6754_NUM_SUPPLIES ARRAY_SIZE(tas6754_supply_names)

struct tas6754_data {
	struct device *dev;
	struct regmap *regmap;
	struct regulator_bulk_data supplies[TAS6754_NUM_SUPPLIES];
	struct delayed_work fault_check_work;
	unsigned int last_oc_dc_fault;//last_cfault
	unsigned int last_power_fault;//last_fault1;
	unsigned int last_ot_fault;//last_fault2;
	unsigned int last_cbc_fault_warn;//last_warn;
	unsigned int last_rtldg_ol_sl_fault;
	struct gpio_desc *pd_gpio;
    struct gpio_desc *stby_gpio;
	//struct gpio_desc *standby_gpio;
	//struct gpio_desc *mute_gpio;
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
		tas6754->last_oc_dc_fault = 0;
		tas6754->last_power_fault = 0;
		tas6754->last_ot_fault = 0;
		tas6754->last_cbc_fault_warn = 0;
		tas6754->last_rtldg_ol_sl_fault = 0;
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


/**
 * Power-Down Sequence
 * @brief To power-down the device, first set the STBY pin or PD pin low for at least 10ms before removing PVDD, VBAT 
 * or DVDD. After 10ms, the power supplies can be removed. Removing PVDD and VBAT first is recommended 
 * before removing the DVDD supply. 
 */
static int tas6754_power_off(struct snd_soc_component *component)
{
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
	int ret;

	//dev_dbg(component->dev, "%s: Powering off TAS6754\n", __func__);

	/* Put all channels in HiZ state to prevent pops/click, effectively disconnecting outputs for safety.*/
	snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_STATE_HIZ);
	snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_STATE_HIZ);

	/* Switch regmap to cache-only mode to preserve settings */
	regcache_cache_only(tas6754->regmap, true);
	regcache_mark_dirty(tas6754->regmap);

	/* Put device in standby (DEEP SLEEP mode) */
	if (tas6754->stby_gpio){
		gpiod_set_value_cansleep(tas6754->stby_gpio, 0);
	}
	else{
		/* If no GPIO, use fallback to state control register for setting standby (DEEP SLEEP mode)*/
		snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, 
							TAS6754_STATE_CTRL_CH1_CH2_STATE_DEEP_SLEEP);
		snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, 
							TAS6754_STATE_CTRL_CH3_CH4_STATE_DEEP_SLEEP);

	}

	/* Wait at least 10ms before removing power supplies as per datasheet */
	msleep(10);

	/* For complete shutdown, assert PD pin
     * This will reset all registers on next power-up */
	if (tas6754->pd_gpio){
		gpiod_set_value_cansleep(tas6754->pd_gpio, 0);
	}

	/*
	TODO[DTS]: 	To ensure regulators are disabled in the correct sequence (PVDD and VBAT first, then DVDD),
				you should define them in the correct order in your device structure and device tree.
				The Linux regulator framework will disable them in reverse order of enabling.
	Example device tree snippet:			
	tas6754: audio-codec@70 {
    compatible = "ti,tas6754";
    reg = <0x70>;
    
    pvdd-supply = <&reg_audio_pvdd>;
    vbat-supply = <&reg_audio_vbat>;
    dvdd-supply = <&reg_audio_dvdd>;
    
    pd-gpio = <&gpio1 15 GPIO_ACTIVE_HIGH>;
    stby-gpio = <&gpio1 16 GPIO_ACTIVE_HIGH>;
    // other properties 
	};*/
	

	/* Disable all regulators in the recommended sequence. PVDD -> VBAT -> DVDD */
	ret = regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
	if (ret < 0) {
		dev_err(component->dev, "failed to disable supplies: %d\n", ret);
		return ret;
	}

	//dev_dbg(component->dev, "%s: TAS6754 powered off\n", __func__);
	return 0;
}

static int tas6754_power_on(struct snd_soc_component *component)
{
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
	int ret;
	u8 chan_states;
	int no_auto_diags = 0;
	unsigned int reg_val;

	
	if (!regmap_read(tas6754->regmap, TAS6754_DC_LDG_CTRL, &reg_val))
		no_auto_diags = reg_val & TAS6754_DC_LDG_BYPASS_MASK;


	ret = regulator_bulk_enable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
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

static int tas6754_set_bias_level(struct snd_soc_component *component, enum snd_soc_bias_level level)
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
	.controls			= tas6754_snd_controls,
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
	struct tas6754_data *tas6754 = container_of(work, struct tas6754_data, fault_check_work.work);
	struct device *dev = tas6754->dev;
	unsigned int reg;
	int ret;

	ret = regmap_read(tas6754->regmap, TAS6754_OC_DC_FAULT_LATCHED, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read OC_DC_FAULT_LATCHED register: %d\n", ret);
		goto out;
	}

	if (!reg) {
		/* No fault detected */
		tas6754->last_oc_dc_fault = reg;
		goto check_power_fault_latched_reg;
	}

	/* Check if any Over Current (OC) or Direct Current (DC) fault is currently active and was not active in the previous reading */
	if ((reg & TAS6754_OC_FAULT_CH1_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_OC_FAULT_CH1_MASK))
		dev_crit(dev, "experienced a channel 1 overcurrent fault\n");

	if ((reg & TAS6754_OC_FAULT_CH2_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_OC_FAULT_CH2_MASK))
		dev_crit(dev, "experienced a channel 2 overcurrent fault\n");

	if ((reg & TAS6754_OC_FAULT_CH3_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_OC_FAULT_CH3_MASK))
		dev_crit(dev, "experienced a channel 3 overcurrent fault\n");

	if ((reg & TAS6754_OC_FAULT_CH4_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_OC_FAULT_CH4_MASK))
		dev_crit(dev, "experienced a channel 4 overcurrent fault\n");

	if ((reg & TAS6754_DC_FAULT_CH1_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_DC_FAULT_CH1_MASK))
		dev_crit(dev, "experienced a channel 1 DC fault\n");

	if ((reg & TAS6754_DC_FAULT_CH2_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_DC_FAULT_CH2_MASK))
		dev_crit(dev, "experienced a channel 2 DC fault\n");

	if ((reg & TAS6754_DC_FAULT_CH3_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_DC_FAULT_CH3_MASK))
		dev_crit(dev, "experienced a channel 3 DC fault\n");

	if ((reg & TAS6754_DC_FAULT_CH4_MASK) && !(tas6754->last_oc_dc_fault & TAS6754_DC_FAULT_CH4_MASK))
		dev_crit(dev, "experienced a channel 4 DC fault\n");

	/* Store current Over Current (OC) or Direct Current (DC) fault value so we can detect any changes next time */
	tas6754->last_oc_dc_fault = reg;

check_power_fault_latched_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_POWER_FAULT_LATCHED, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read POWER_FAULT_LATCHED register: %d\n", ret);
		goto out;
	}

	reg &= TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK |
		   TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK |
	  	   TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK |
		   TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK |
	  	   TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK |
		   TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK;

	if (!reg) {
		/* No fault detected */
		tas6754->last_power_fault = reg;
		goto check_ot_fault_reg;
	}

	/* Check if any power fault is currently active and was not active in the previous reading */
	if ((reg & TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK))
		dev_crit(dev, "experienced a DVDD power on reset fault\n");

	if ((reg & TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK))
		dev_crit(dev, "experienced a DVDD under voltage fault\n");

	if ((reg & TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK))
		dev_crit(dev, "experienced a PVDD over voltage fault\n");

	if ((reg & TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK))
		dev_crit(dev, "experienced a VBAT over voltage fault\n");

	if ((reg & TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK))
		dev_crit(dev, "experienced a PVDD under voltage fault\n");

	if ((reg & TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK) && !(tas6754->last_power_fault & TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK))
		dev_crit(dev, "experienced a VBAT under voltage fault\n");

	/* Store current power fault value so we can detect any changes next time */
	tas6754->last_power_fault = reg;

check_ot_fault_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_OT_FAULT, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read OT_FAULT register: %d\n", ret);
		goto out;
	}

	if (!reg) {
		/* No fault detected */
		tas6754->last_ot_fault = reg;
		goto check_cbc_fault_warn_reg;
	}

	/* Check if any Over Temperature (OT) fault is currently active and was not active in the previous reading */
	if ((reg & TAS6754_OT_FAULT_GLOBAL_WARNING_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_GLOBAL_WARNING_MASK))
		dev_crit(dev, "experienced a global warning\n");

	if ((reg & TAS6754_OT_FAULT_GLOBAL_FAULT_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_GLOBAL_FAULT_MASK))
		dev_crit(dev, "experienced a global fault\n");

	if ((reg & TAS6754_OT_FAULT_CP_FAULT_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_CP_FAULT_MASK))
		dev_crit(dev, "experienced a charge pump fault\n");

	if ((reg & TAS6754_OT_FAULT_GLOBAL_OTSD_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_GLOBAL_OTSD_MASK))
		dev_crit(dev, "experienced a global overtemperature shutdown\n");

	if ((reg & TAS6754_OT_FAULT_CH1_OTSD_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_CH1_OTSD_MASK))
		dev_crit(dev, "experienced an overtemperature shutdown on CH1\n");

	if ((reg & TAS6754_OT_FAULT_CH2_OTSD_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_CH2_OTSD_MASK))
		dev_crit(dev, "experienced an overtemperature shutdown on CH2\n");

	if ((reg & TAS6754_OT_FAULT_CH3_OTSD_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_CH3_OTSD_MASK))
		dev_crit(dev, "experienced an overtemperature shutdown on CH3\n");

	if ((reg & TAS6754_OT_FAULT_CH4_OTSD_MASK) && !(tas6754->last_ot_fault & TAS6754_OT_FAULT_CH4_OTSD_MASK))
		dev_crit(dev, "experienced an overtemperature shutdown on CH4\n");

	/* Store current Over Temperature (OT) fault value so we can detect any changes next time */
	tas6754->last_ot_fault = reg;

check_cbc_fault_warn_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_CBC_FAULT_WARN_LATCHED, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read CBC_FAULT_WARN_LATCHED register: %d\n", ret);
		goto out;
	}

	if (!reg) {
		/* No fault detected */
		tas6754->last_cbc_fault_warn = reg;
		goto check_rtldg_ol_sl_fault_reg;
	}

	/* Check if any channel load current fault or warning is currently active and was not active in the previous reading */
	if ((reg & TAS6754_CBC_WARN_CH1_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_WARN_CH1_MASK))
		dev_warn(dev, "experienced a channel 1 load current warning\n");

	if ((reg & TAS6754_CBC_WARN_CH2_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_WARN_CH2_MASK))
		dev_warn(dev, "experienced a channel 2 load current warning\n");

	if ((reg & TAS6754_CBC_WARN_CH3_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_WARN_CH3_MASK))
		dev_warn(dev, "experienced a channel 3 load current warning\n");

	if ((reg & TAS6754_CBC_WARN_CH4_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_WARN_CH4_MASK))
		dev_warn(dev, "experienced an channel 4 load current warning\n");

	if ((reg & TAS6754_CBC_FAULT_CH1_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_FAULT_CH1_MASK))
		dev_warn(dev, "experienced a channel 1 load current fault\n");

	if ((reg & TAS6754_CBC_FAULT_CH2_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_FAULT_CH2_MASK))
		dev_warn(dev, "experienced a channel 2 load current fault\n");

	if ((reg & TAS6754_CBC_FAULT_CH3_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_FAULT_CH3_MASK))
		dev_warn(dev, "experienced a channel 3 load current fault\n");
	
	if ((reg & TAS6754_CBC_FAULT_CH4_MASK) && !(tas6754->last_cbc_fault_warn & TAS6754_CBC_FAULT_CH4_MASK))
		dev_warn(dev, "experienced a channel 4 load current fault\n");

	/* Store current channel load current fault or warning value so we can detect any changes next time */
	tas6754->last_cbc_fault_warn = reg;

check_rtldg_ol_sl_fault_reg:
	ret = regmap_read(tas6754->regmap, TAS6754_RTLDG_OL_SL_FAULT_LATCHED, &reg);
	if (ret < 0) {
		dev_err(dev, "failed to read RTLDG_OL_SL_FAULT_LATCHED register: %d\n", ret);
		goto out;
	}

	if (!reg) {
		/* No fault detected */
		tas6754->last_rtldg_ol_sl_fault = reg;//TODO, Where to go now??????
		goto out;
	}

	/* Check if any shorted/open load fault is currently active and was not active in the previous reading */
	if ((reg & TAS6754_RTLDG_SL_CH1_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_SL_CH1_MASK))
		dev_warn(dev, "experienced a shorted load on channel 1 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_SL_CH2_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_SL_CH2_MASK))
		dev_warn(dev, "experienced a shorted load on channel 2 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_SL_CH3_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_SL_CH3_MASK))
		dev_warn(dev, "experienced a shorted load on channel 3 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_SL_CH4_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_SL_CH4_MASK))
		dev_warn(dev, "experienced an shorted load on channel 4 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_OL_CH1_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_OL_CH1_MASK))
		dev_warn(dev, "experienced an open load on channel 1 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_OL_CH2_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_OL_CH2_MASK))
		dev_warn(dev, "experienced an open load on channel 2 during Real-Time Load Diagnostics\n");

	if ((reg & TAS6754_RTLDG_OL_CH3_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_OL_CH3_MASK))
		dev_warn(dev, "experienced an open load on channel 3 during Real-Time Load Diagnostics\n");
	
	if ((reg & TAS6754_RTLDG_OL_CH4_MASK) && !(tas6754->last_rtldg_ol_sl_fault & TAS6754_RTLDG_OL_CH4_MASK))
		dev_warn(dev, "experienced an open load on channel 4 during Real-Time Load Diagnostics\n");

	/* Store current shorted/open load fault value so we can detect any changes next time */
	tas6754->last_rtldg_ol_sl_fault = reg;

	/* Clear any fault by toggling the CLEAR FAULT control bit */
	ret = regmap_write_bits(tas6754->regmap, TAS6754_RESET,
				TAS6754_RESET_CLEAR_FAULT_MASK, TAS6754_RESET_CLEAR_FAULT_CLEAR);
	if (ret < 0)
		dev_err(dev, "failed to write RESET register: %d\n", ret);

	ret = regmap_write_bits(tas6754->regmap, TAS6754_RESET,
				TAS6754_RESET_CLEAR_FAULT_MASK, TAS6754_RESET_CLEAR_FAULT_NORMAL);
	if (ret < 0)
		dev_err(dev, "failed to write RESET register: %d\n", ret);

out:
	/* Schedule the next fault check at the specified interval */
	schedule_delayed_work(&tas6754->fault_check_work, msecs_to_jiffies(TAS6754_FAULT_CHECK_INTERVAL));
}

static const struct reg_default tas6754_reg_defaults[] = {
	{ TAS6754_RESET,					0x00},
	{ TAS6754_OUTPUT_CTRL,              0x00},
	{ TAS6754_STATE_CTRL_CH1_CH2,       0x22},
	{ TAS6754_STATE_CTRL_CH3_CH4,       0x22},
	{ TAS6754_ISENSE_CTRL,              0x0F},
	{ TAS6754_DC_DETECT_CTRL,           0x00},
	{ TAS6754_SCLK_INV_CTRL,            0x00},
	{ TAS6754_AUDIO_INTERFACE_CTRL,     0x00},
	{ TAS6754_SDIN_CTRL,                0x0A},
	{ TAS6754_SDOUT_CTRL,               0x1A},
	{ TAS6754_SDIN_OFFSET_MSB,          0x00},
	{ TAS6754_SDIN_AUDIO_OFFSET,        0x00},
	{ TAS6754_SDIN_LL_OFFSET,           0x60},
	{ TAS6754_SDIN_CH_SWAP,             0x00},
	{ TAS6754_SDOUT_OFFSET_MSB,         0xCF},
	{ TAS6754_VPREDICT_OFFSET,          0xFF},
	{ TAS6754_ISENSE_OFFSET,            0x00},
	{ TAS6754_SDOUT_EN,                 0x00},
	{ TAS6754_LL_EN,                    0x00},
	{ TAS6754_RTLDG_EN,                 0x10},
	{ TAS6754_DC_BLOCK_BYP,             0x00},
	{ TAS6754_DSP_CTRL,                 0x00},
	{ TAS6754_PAGE_AUTO_INC,            0x00},
	{ TAS6754_DIG_VOL_CH1,              0x30},
	{ TAS6754_DIG_VOL_CH2,              0x30},
	{ TAS6754_DIG_VOL_CH3,              0x30},
	{ TAS6754_DIG_VOL_CH4,              0x30},
	{ TAS6754_DIG_VOL_RAMP_CTRL,        0x77},
	{ TAS6754_DIG_VOL_COMBINE_CTRL,     0x00},
	{ TAS6754_AUTO_MUTE_EN,             0x00},
	{ TAS6754_AUTO_MUTE_TIMING_CH1_CH2,	0x00},
	{ TAS6754_AUTO_MUTE_TIMING_CH3_CH4, 0x00},
	{ TAS6754_ANALOG_GAIN_CH1_CH2,      0x00},
	{ TAS6754_ANALOG_GAIN_CH3_CH4,      0x00},
	{ TAS6754_ANALOG_GAIN_RAMP_CTRL,    0x00},
	{ TAS6754_PULSE_INJECTION_EN,       0x03},
	{ TAS6754_CBC_CTRL,                 0x07},
	{ TAS6754_CURRENT_LIMIT_CTRL,       0x00},
	{ TAS6754_ISENSE_CAL,               0x00},
	{ TAS6754_PWM_PHASE_CTRL,           0x00},
	{ TAS6754_SS_CTRL,                  0x00},
	{ TAS6754_SS_RANGE_CTRL,            0x00},
	{ TAS6754_SS_DWELL_CTRL,            0x00},
	{ TAS6754_RAMP_PHASE_CTRL_GPO,      0x00},
	{ TAS6754_PWM_PHASE_M_CTRL_CH1,     0x00},
	{ TAS6754_PWM_PHASE_M_CTRL_CH2,     0x00},
	{ TAS6754_PWM_PHASE_M_CTRL_CH3,     0x00},
	{ TAS6754_PWM_PHASE_M_CTRL_CH4,     0x00},
	{ TAS6754_AUTO_MUTE_STATUS,         0x00},
	{ TAS6754_REPORT_ROUTING_1,         0x00},
	{ TAS6754_OTSD_RECOVERY_EN,         0x00},
	{ TAS6754_REPORT_ROUTING_2,         0xA2},
	{ TAS6754_REPORT_ROUTING_3,         0x00},
	{ TAS6754_REPORT_ROUTING_4,         0x06},
	{ TAS6754_CLIP_DETECT_CTRL,         0x00},
	{ TAS6754_REPORT_ROUTING_5,         0x00},
	{ TAS6754_GPIO1_OUTPUT_SELECT,      0x00},
	{ TAS6754_GPIO2_OUTPUT_SELECT,      0x00},
	{ TAS6754_GPIO_INPUT_SLEEP_HIZ,     0x00},
	{ TAS6754_GPIO_INPUT_PLAY_SLEEP,    0x00},
	{ TAS6754_GPIO_INPUT_MUTE,          0x00},
	{ TAS6754_GPIO_INPUT_SYNC,          0x00},
	{ TAS6754_GPIO_INPUT_SDIN2,         0x00},
	{ TAS6754_GPIO_CTRL,                0x22},
	{ TAS6754_GPIO_INVERT,              0x00},
	{ TAS6754_DC_LDG_CTRL,              0x00},
	{ TAS6754_DC_LDG_LO_CTRL,           0x00},
	{ TAS6754_DC_LDG_TIME_CTRL,         0x00},
	{ TAS6754_DC_LDG_SL_CH1_CH2_CTRL,   0x11},
	{ TAS6754_DC_LDG_SL_CH3_CH4_CTRL,   0x11},
	{ TAS6754_AC_LDG_CTRL,              0x10},
	{ TAS6754_TWEETER_DETECT_CTRL,      0x08},
	{ TAS6754_TWEETER_DETECT_THRESH,    0x00},
	{ TAS6754_AC_LDG_FREQ_CTRL,         0xC8},
	{ TAS6754_OTW_CTRL_CH1_CH2,         0x11},
	{ TAS6754_OTW_CTRL_CH3_CH4,         0x11},
};

static bool tas6754_is_writable_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case TAS6754_RESET: /* W */
	case TAS6754_OUTPUT_CTRL:
	case TAS6754_STATE_CTRL_CH1_CH2:
	case TAS6754_STATE_CTRL_CH3_CH4:
	case TAS6754_ISENSE_CTRL:
	case TAS6754_DC_DETECT_CTRL:
	case TAS6754_SCLK_INV_CTRL:
	case TAS6754_AUDIO_INTERFACE_CTRL:
	case TAS6754_SDIN_CTRL:
	case TAS6754_SDOUT_CTRL:
	case TAS6754_SDIN_OFFSET_MSB:
	case TAS6754_SDIN_AUDIO_OFFSET:
	case TAS6754_SDIN_LL_OFFSET:
	case TAS6754_SDIN_CH_SWAP:
	case TAS6754_SDOUT_OFFSET_MSB:
	case TAS6754_VPREDICT_OFFSET:
	case TAS6754_ISENSE_OFFSET:
	case TAS6754_SDOUT_EN:
	case TAS6754_LL_EN:
	case TAS6754_RTLDG_EN:
	case TAS6754_DC_BLOCK_BYP:
	case TAS6754_DSP_CTRL:
	case TAS6754_PAGE_AUTO_INC:
	case TAS6754_DIG_VOL_CH1:
	case TAS6754_DIG_VOL_CH2:
	case TAS6754_DIG_VOL_CH3:
	case TAS6754_DIG_VOL_CH4:
	case TAS6754_DIG_VOL_RAMP_CTRL:
	case TAS6754_DIG_VOL_COMBINE_CTRL:
	case TAS6754_AUTO_MUTE_EN:
	case TAS6754_AUTO_MUTE_TIMING_CH1_CH2:
	case TAS6754_AUTO_MUTE_TIMING_CH3_CH4:
	case TAS6754_ANALOG_GAIN_CH1_CH2:
	case TAS6754_ANALOG_GAIN_CH3_CH4:
	case TAS6754_ANALOG_GAIN_RAMP_CTRL:
	case TAS6754_PULSE_INJECTION_EN:
	case TAS6754_CBC_CTRL:
	case TAS6754_CURRENT_LIMIT_CTRL:
	case TAS6754_ISENSE_CAL:
	case TAS6754_PWM_PHASE_CTRL:
	case TAS6754_SS_CTRL:
	case TAS6754_SS_RANGE_CTRL:
	case TAS6754_SS_DWELL_CTRL:
	case TAS6754_RAMP_PHASE_CTRL_GPO:
	case TAS6754_PWM_PHASE_M_CTRL_CH1:
	case TAS6754_PWM_PHASE_M_CTRL_CH2:
	case TAS6754_PWM_PHASE_M_CTRL_CH3:
	case TAS6754_PWM_PHASE_M_CTRL_CH4:
	case TAS6754_AUTO_MUTE_STATUS:
	case TAS6754_REPORT_ROUTING_1:
	case TAS6754_OTSD_RECOVERY_EN:
	case TAS6754_REPORT_ROUTING_2:
	case TAS6754_REPORT_ROUTING_3:
	case TAS6754_REPORT_ROUTING_4:
	case TAS6754_CLIP_DETECT_CTRL:
	case TAS6754_REPORT_ROUTING_5:
	case TAS6754_GPIO1_OUTPUT_SELECT:
	case TAS6754_GPIO2_OUTPUT_SELECT:
	case TAS6754_GPIO_INPUT_SLEEP_HIZ:
	case TAS6754_GPIO_INPUT_PLAY_SLEEP:
	case TAS6754_GPIO_INPUT_MUTE:
	case TAS6754_GPIO_INPUT_SYNC:
	case TAS6754_GPIO_INPUT_SDIN2:
	case TAS6754_GPIO_CTRL:
	case TAS6754_GPIO_INVERT:
	case TAS6754_DC_LDG_CTRL:
	case TAS6754_DC_LDG_LO_CTRL:
	case TAS6754_DC_LDG_TIME_CTRL:
	case TAS6754_DC_LDG_SL_CH1_CH2_CTRL:
	case TAS6754_DC_LDG_SL_CH3_CH4_CTRL:
	case TAS6754_AC_LDG_CTRL:
	case TAS6754_TWEETER_DETECT_CTRL:
	case TAS6754_TWEETER_DETECT_THRESH:
	case TAS6754_AC_LDG_FREQ_CTRL:
	case TAS6754_OTW_CTRL_CH1_CH2:
	case TAS6754_OTW_CTRL_CH3_CH4:
		return true;
	default:
		return false;
	}
}

static bool tas6754_is_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case TAS6754_STATE_REPORT_CH1_CH2:
	case TAS6754_STATE_REPORT_CH3_CH4:
	case TAS6754_STATE_REPORT_CH1_CH2:
	case TAS6754_STATE_REPORT_CH3_CH4:
	case TAS6754_PVDD_SENSE:
	case TAS6754_TEMP_GLOBAL:
	case TAS6754_FS_MON:
	case TAS6754_SCLK_MON:
	case TAS6754_POWER_FAULT_STATUS_1:
	case TAS6754_POWER_FAULT_STATUS_2:
	case TAS6754_OT_FAULT:
	case TAS6754_OTW_STATUS:
	case TAS6754_CLIP_WARN_STATUS:
	case TAS6754_CBC_WARNING_STATUS:
	case TAS6754_POWER_FAULT_LATCHED:
	case TAS6754_OTSD_LATCHED:
	case TAS6754_OTW_LATCHED:
	case TAS6754_CLIP_WARN_LATCHED:
	case TAS6754_CLK_FAULT_LATCHED:
	case TAS6754_RTLDG_OL_SL_FAULT_LATCHED:
	case TAS6754_CBC_FAULT_WARN_LATCHED:
	case TAS6754_OC_DC_FAULT_LATCHED:
	case TAS6754_TEMP_CH1_CH2:
	case TAS6754_TEMP_CH3_CH4:
	case TAS6754_WARN_OT_MAX_FLAG:
	case TAS6754_DC_LDG_REPORT_CH1_CH2:
	case TAS6754_DC_LDG_REPORT_CH3_CH4:
	case TAS6754_DC_LDG_RESULT:
	case TAS6754_AC_LDG_REPORT_CH1_R:
	case TAS6754_AC_LDG_REPORT_CH1_I:
	case TAS6754_AC_LDG_REPORT_CH2_R:
	case TAS6754_AC_LDG_REPORT_CH2_I:
	case TAS6754_AC_LDG_REPORT_CH3_R:
	case TAS6754_AC_LDG_REPORT_CH3_I:
	case TAS6754_AC_LDG_REPORT_CH4_R:
	case TAS6754_AC_LDG_REPORT_CH4_I:
	case TAS6754_TWEETER_REPORT:
	case TAS6754_CH1_RTLDG_IMP_MSB:
	case TAS6754_CH1_RTLDG_IMP_LSB:
	case TAS6754_CH2_RTLDG_IMP_MSB:
	case TAS6754_CH2_RTLDG_IMP_LSB:
	case TAS6754_CH3_RTLDG_IMP_MSB:
	case TAS6754_CH3_RTLDG_IMP_LSB:
	case TAS6754_CH4_RTLDG_IMP_MSB:
	case TAS6754_CH4_RTLDG_IMP_LSB:
	case TAS6754_DC_LDG_DCR_MSB:
	case TAS6754_CH1_DC_LDG_DCR_LSB:
	case TAS6754_CH2_DC_LDG_DCR_LSB:
	case TAS6754_CH3_DC_LDG_DCR_LSB:
	case TAS6754_CH4_DC_LDG_DCR_LSB:
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
	{ .compatible = "ti,tas6754" },
	{},
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
	/* tas6754: Enables low power DEEP SLEEP state (active low), 
				110kΩ internal pull-down resistor*/

	/* tas6424: Enables low power standby state (active Low),
				100-kΩ internal pulldown resistor */
	
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


	/* tas6424: Mutes the device outputs (active low), 100-kΩ internal pulldown resistor */

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
	ret = regmap_update_bits(tas6754->regmap, TAS6754_RESET,
				 TAS6754_RESET_DEVICE_RESET_MASK, TAS6754_RESET_DEVICE_RESET);
	if (ret) {
		dev_err(dev, "unable to reset device: %d\n", ret);
		goto disable_regs;
	}
	ret = regmap_update_bits(tas6754->regmap, TAS6754_RESET,
				 TAS6754_RESET_REGISTER_RESET_MASK, TAS6754_RESET_REGISTER_RESET);
	if (ret) {
		dev_err(dev, "unable to reset registers: %d\n", ret);
		goto disable_regs;
	}

	INIT_DELAYED_WORK(&tas6754->fault_check_work, tas6754_fault_check_work);

	/* Register codec with ALSA  */
	ret = devm_snd_soc_register_component(dev, &soc_codec_dev_tas6754, tas6754_dai, ARRAY_SIZE(tas6754_dai));
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