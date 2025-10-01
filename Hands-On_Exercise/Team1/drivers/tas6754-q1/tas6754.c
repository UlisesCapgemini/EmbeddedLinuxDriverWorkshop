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
	//struct gpio_desc *mute_gpio;//TODO: Check mute handling via registers or GPIO additional config?

    /* Audio interface configuration (ENHANCED)*/
    bool tdm_mode;                		/* Whether in TDM mode */
    bool dsp_a_mode;              		/* Whether in DSP_A mode (needs 1-bit offset) */
    bool short_fsync;             		/* Whether FSYNC pulse is < 8 SCLK cycles */
    bool use_sdin2_for_ch34;      		/* Whether to use SDIN2 for channels 3-4 */
    bool ll_enabled;              		/* Whether low latency channels are enabled */
    unsigned int bit_depth;       		/* Current bit depth */
    unsigned int sample_rate;     		/* Current sample rate */
    unsigned int channels;        		/* Current channel count */
    unsigned int dai_fmt;         		/* Current DAI format */
    unsigned int channel_offset;  		/* Custom channel offset (in bits) */
    unsigned int ll_offset;       		/* Low latency channel offset (in bits) */
    unsigned int sdin2_gpio_num;  		/* GPIO number to use for SDIN2 (1 or 2) */
    unsigned int tdm_slots;       		/* Number of TDM slots (4, 8, or 16) */
    unsigned int tdm_slot_width;  		/* TDM slot width (16, 20, 24, or 32) */
    unsigned int channel_map[4];  		/* Mapping of channels to TDM slots */
    unsigned int audio_swap;      		/* Audio channel swap option */
    unsigned int ll_swap;         		/* Low latency channel swap option */
	//bool support_rate_change;  		/* TODO: [Desired] to support on-the-fly rate changes */
	//unsigned int detected_sclk_ratio; /* TODO: [Optional] SCLK ratio detected by the device. Useful for:*/
															/*-Debugging
															-Potentially adjusting settings based on the detected ratio
															-Reporting the ratio through sysfs or debugfs*/

};

/*
 * DAC digital volumes. From 0 dB to -103 dB in -0.5 dB steps, plus a mute setting.
 * Register values: 0x30 = 0dB, 0x31 = -0.5dB, ..., 0xFE = -103dB, 0xFF = Mute
 */
static DECLARE_TLV_DB_SCALE(dac_tlv, 0, -50, 1);
/*
 * DAC digital volumes. From -103.5 to 24 dB in 0.5 dB steps. Note that
 * setting the gain below -100 dB (register value <0x7) is effectively a MUTE
 * as per device datasheet.
 */
/*static DECLARE_TLV_DB_SCALE(dac_tlv, -10350, 50, 0);

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
};*/

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


/**
 * tas6754_hw_params
 * @brief: Configure hardware parameters for audio stream (sample rate, bit depth, channels)
 * 
 * @substream:
 * @params:
 * @dai:
 * 
 * @return
 */
static int tas6754_hw_params(struct snd_pcm_substream *substream,
                            struct snd_pcm_hw_params *params,
                            struct snd_soc_dai *dai)
{
    struct snd_soc_component *component = dai->component;
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    unsigned int rate, channels, format;
    u8 audio_intf_ctrl, sdin_ctrl, offset_msb = 0, audio_offset = 0;
    int ret;
    
    rate = params_rate(params);
    channels = params_channels(params);
    format = params_format(params);
    
    /* Store current configuration */
    tas6754->bit_depth = snd_pcm_format_width(format);
    tas6754->sample_rate = rate;
    tas6754->channels = channels;
    
    /* Read current audio interface control register */
    ret = snd_soc_component_read(component, TAS6754_AUDIO_INTERFACE_CTRL, &audio_intf_ctrl);
    if (ret < 0)
        return ret;
    
    /* Configure TDM mode if needed */
    if (channels > 4) {
        /* Enable TDM mode for > 4 channels */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_TDM_MODE;;
        tas6754->tdm_mode = true;
        
        /* Make sure DSP format is set */
        audio_intf_ctrl &= ~(0x0C); /* Clear format bits */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_TDM_DSP;
    }
    
    /* Write updated audio interface control register */
    ret = snd_soc_component_write(component, TAS6754_AUDIO_INTERFACE_CTRL, audio_intf_ctrl);
    if (ret < 0)
        return ret;
    
    /* Configure SDIN_CTRL register for data length */
    sdin_ctrl = 0;
    
    /* Set data length for channels 1-2 or audio path in TDM mode */
    switch (tas6754->bit_depth) {
    case 16:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_16BIT;
        break;
    case 20:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_20BIT;
        break;
    case 24:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_24BIT;
        break;
    case 32:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_32BIT;
        break;
    default:
        dev_err(component->dev, "Unsupported bit depth: %u\n", tas6754->bit_depth);
        return -EINVAL;
    }
    
    /* Set data length for channels 3-4 or low latency path in TDM mode */
    switch (tas6754->bit_depth) {
    case 16:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_2_16BIT;
        break;
    case 20:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_2_20BIT;
        break;
    case 24:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_2_24BIT;
        break;
    case 32:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_2_32BIT;
        break;
    }
    
    /* Configure SDIN source */
    if (tas6754->tdm_mode) {
        /* In TDM mode, typically all channels come from SDIN_1 */
        sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN1;
        sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_LL_SDIN1;
    } else if (channels > 2 && tas6754->use_sdin2_for_ch34) {
        /* In I2S/LJ/RJ mode with >2 channels, optionally use SDIN_2 for channels 3-4 */
        sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN2;
        
        /* Configure GPIO as SDIN2 */
        ret = tas6754_configure_sdin2(component, tas6754->sdin2_gpio_num);
        if (ret < 0)
            return ret;
    } else {
        /* Default: use SDIN_1 for all channels */
        sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN1;
    }
    
    /* Write SDIN_CTRL register */
    ret = snd_soc_component_write(component, TAS6754_SDIN_CTRL, sdin_ctrl);
    if (ret < 0)
        return ret;
    
    /* Configure channel offsets if needed */
    if (tas6754->dsp_a_mode || tas6754->tdm_mode || tas6754->channel_offset != 0) {
        /* Calculate offset value */
        unsigned int offset = tas6754->channel_offset;
        
        /* For DSP_A mode, add 1-bit offset */
        if (tas6754->dsp_a_mode)
            offset += 1;
        
        /* Set offset MSB (bits 8-9) in bits 7-6 of SDIN_OFFSET_MSB */
        offset_msb |= ((offset >> 8) & 0x03) << 6;
        
        /* Set offset LSB (bits 0-7) */
        audio_offset = offset & 0xFF;
        
        /* Write offset registers */
        ret = snd_soc_component_write(component, TAS6754_SDIN_OFFSET_MSB, offset_msb);
        if (ret < 0)
            return ret;
        
        ret = snd_soc_component_write(component, TAS6754_SDIN_AUDIO_OFFSET, audio_offset);
        if (ret < 0)
            return ret;
        
        /* If low latency is enabled in TDM mode, configure its offset */
        if (tas6754->tdm_mode && tas6754->ll_enabled) {
            u8 ll_offset;
            unsigned int ll_offset_value = tas6754->ll_offset;
            
            /* Set LL offset MSB (bits 8-9) in bits 5-4 of SDIN_OFFSET_MSB */
            offset_msb &= ~(0x30); /* Clear bits 5-4 */
            offset_msb |= ((ll_offset_value >> 8) & 0x03) << 4;
            
            /* Set LL offset LSB (bits 0-7) */
            ll_offset = ll_offset_value & 0xFF;
            
            /* Write updated offset MSB register */
            ret = snd_soc_component_write(component, TAS6754_SDIN_OFFSET_MSB, offset_msb);
            if (ret < 0)
                return ret;
            
            /* Write LL offset LSB register */
            ret = snd_soc_component_write(component, TAS6754_SDIN_LL_OFFSET, ll_offset);
            if (ret < 0)
                return ret;
        }
    }
    
    /* Wait for the device to detect the sample rate */
    msleep(10);
    
    /* Verify that the device has detected the correct sample rate */
    ret = tas6754_verify_sample_rate(component, rate);
    if (ret < 0)
        return ret;
    
    return 0;
}

/**
 * tas6754_verify_sample_rate
 * @brief: Verify that the device has detected the correct sample rate and SCLK ratio
 * 
 * @component:
 * @rate:
 * 
 * @return:
 */
static int tas6754_verify_sample_rate(struct snd_soc_component *component, unsigned int rate)
{
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    u8 fs_mon, sclk_mon;
    u8 expected_fs_code;
    u16 sclk_ratio;
    int ret;
    
    /* Read FS_MON register to check detected sample rate */
    ret = snd_soc_component_read(component, TAS6754_FS_MON, &fs_mon);
    if (ret < 0)
        return ret;
    
    /* Read SCLK_MON register to check detected SCLK ratio */
    ret = snd_soc_component_read(component, TAS6754_SCLK_MON, &sclk_mon);
    if (ret < 0)
        return ret;
    
    /* Extract detected sample rate code */
    u8 detected_fs_code = fs_mon & 0x0F;
    
    /* Calculate expected sample rate code based on input rate */
    switch (rate) {
    case 48000:
        expected_fs_code = TAS6754_FS_48KHZ;
        break;
    case 96000:
        expected_fs_code = TAS6754_FS_96KHZ;
        break;
    case 192000:
        expected_fs_code = TAS6754_FS_192KHZ;
        break;
		/* Handling Approach:
		 * Since the datasheet mentions support for 44.1kHz but doesn't list its detection code,
		 * the function takes a pragmatic approach:
		 * - Accept whatever code the device reports for 44.1kHz family rates (Compatibility).
		 * - This allows the driver to work with 44.1kHz even though we don't know the exact expected code
		 * */
    case 44100:
    case 88200:
    case 176400:
		/* Add more detailed logging when 44.1kHz family rates are detected */
		dev_info(component->dev, "44.1kHz family rate %u Hz detected with code 0x%x\n",
            rate, detected_fs_code);
        /* The datasheet doesn't explicitly list codes for 44.1kHz family,
         * but we'll assume they're supported as mentioned in the description */
        expected_fs_code = detected_fs_code; /* Accept whatever was detected */
        break;
    default:
        dev_err(component->dev, "Unsupported sample rate: %u Hz\n", rate);
        return -EINVAL;
    }
    
    /* Check if detected sample rate matches expected */
    if (detected_fs_code == TAS6754_FS_MON_SAMPLE_RATE_ERROR) {
        dev_err(component->dev, "Sample rate detection error\n");
        return -EIO;
    }
    
    if (detected_fs_code != expected_fs_code) {
        dev_warn(component->dev, 
                "Sample rate mismatch: expected %u Hz (code 0x%x), detected code 0x%x\n",
                rate, expected_fs_code, detected_fs_code);
    /* We'll continue anyway, as the device might have detected a valid rate */
    } else {
        dev_dbg(component->dev, "Sample rate correctly detected: %u Hz (code 0x%x)\n",
               rate, detected_fs_code);
    }

	/* Calculate full SCLK ratio for a total of 10 bits SCLK ratio range between 32Fs to 512Fs.*/
    sclk_ratio = ((fs_mon & TAS6754_FS_MON_SCLK_RATIO_MSB_MASK) >> TAS6754_FS_MON_SCLK_RATIO_MSB_SHIFT) << 8;
    sclk_ratio |= sclk_mon;
    
    dev_dbg(component->dev, "Detected SCLK ratio: %u x Fs\n", sclk_ratio);	   
    
    /* Verify SCLK ratio is within supported range */
    if (sclk_ratio < 32 || sclk_ratio > 512) {
        dev_err(component->dev, "SCLK ratio out of range: %u x Fs\n", sclk_ratio);
        return -EIO;
    }
    
    /* Store the detected SCLK ratio for reference 
	TODO: [Optional] SCLK ratio detected by the device. Useful for:
		- Debugging
		- Potentially adjusting settings based on the detected ratio
		- Reporting the ratio through sysfs or debugfs*/
    //tas6754->detected_sclk_ratio = sclk_ratio;
    
    return 0;
}

/**
 * tas6754_set_dai_fmt
 * @brief: Set DAI format, Clock polarity and related settings.
 * 
 * @dai:
 * @fmt:
 * 
 * @return:
 */
static int tas6754_set_dai_fmt(struct snd_soc_dai *dai, unsigned int fmt)
{
    struct snd_soc_component *component = dai->component;
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    u8 audio_intf_ctrl = 0;
	u8 sclk_inv_ctrl = 0;
    int ret;
    
    /* Store the DAI format for later use */
    tas6754->dai_fmt = fmt & SND_SOC_DAIFMT_FORMAT_MASK;
    
    /* Read current audio interface control register */
    ret = snd_soc_component_read(component, TAS6754_AUDIO_INTERFACE_CTRL, &audio_intf_ctrl);
    if (ret < 0)
        return ret;

    /* Read current SCLK inversion control register */
    ret = snd_soc_component_read(component, TAS6754_SCLK_INV_CTRL, &sclk_inv_ctrl);
    if (ret < 0)
        return ret;
    
    /* Clear SCLK inversion bits */
    sclk_inv_ctrl &= ~(TAS6754_SCLK_INV_CTRL_SCLK_INV_TX_MASK | TAS6754_SCLK_INV_CTRL_SCLK_INV_MASK);
    
    /* Clear format bits while preserving other settings */
    audio_intf_ctrl &= ~(0x0C); /* Clear ASI FORMAT bits (3-2) */
    
    /* Format setting */
    switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
    case SND_SOC_DAIFMT_I2S:
        /* Set I2S format (00) */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_I2S;
        tas6754->tdm_mode = false;
        break;
        
    case SND_SOC_DAIFMT_LEFT_J:
        /* Set Left-Justified format (11) */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_LTJ;
        tas6754->tdm_mode = false;
        break;
        
    case SND_SOC_DAIFMT_RIGHT_J:
        /* Set Right-Justified format (10) */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_RTJ;
        tas6754->tdm_mode = false;
        break;
        
    case SND_SOC_DAIFMT_DSP_A:
    case SND_SOC_DAIFMT_DSP_B:
        /* Set DSP/TDM format (01) */
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_TDM_DSP;
        
        /* For DSP_A, we need to configure the offset */
        if ((fmt & SND_SOC_DAIFMT_FORMAT_MASK) == SND_SOC_DAIFMT_DSP_A) {
            /* DSP_A has a 1-bit offset, configure in SDIN_AUDIO_OFFSET */
            tas6754->dsp_a_mode = true;
        } else {
            tas6754->dsp_a_mode = false;
        }
        
        /* Clear TDM bit - we'll set it in hw_params if needed */
        audio_intf_ctrl &= ~(0x10); /* Clear TDM bit (4) */
        tas6754->tdm_mode = false;
        break;
        
    default:
        dev_err(component->dev, "Unsupported DAI format %d\n",
                fmt & SND_SOC_DAIFMT_FORMAT_MASK);
        return -EINVAL;
    }

	/* Clock polarity setting */
    switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
    case SND_SOC_DAIFMT_NB_NF:
        /* BCLK not inverted, FSYNC not inverted */
        /* Normal SCLK mode - default, no need to set bits */
        break;
    case SND_SOC_DAIFMT_IB_NF:
        /* BCLK inverted, FSYNC not inverted */
        /* Inverted SCLK mode */
        sclk_inv_ctrl |= TAS6754_SCLK_INV_CTRL_TX_INVERTED | TAS6754_SCLK_INV_CTRL_INVERTED;
        break;
    case SND_SOC_DAIFMT_NB_IF:
        /* BCLK not inverted, FSYNC inverted */
        /* Handle FSYNC inversion if needed */
        break;
    case SND_SOC_DAIFMT_IB_IF:
        /* BCLK inverted, FSYNC inverted */
        /* Inverted SCLK mode */
        sclk_inv_ctrl |= TAS6754_SCLK_INV_CTRL_TX_INVERTED | TAS6754_SCLK_INV_CTRL_INVERTED;
        /* Handle FSYNC inversion if needed */
        break;
    default:
        dev_err(component->dev, "Unsupported clock polarity setting: 0x%x\n",
                fmt & SND_SOC_DAIFMT_INV_MASK);
        return -EINVAL;
    }
    
    /* Write SCLK inversion control register */
    ret = snd_soc_component_write(component, TAS6754_SCLK_INV_CTRL, sclk_inv_ctrl);
    if (ret < 0)
        return ret;
    
    /* FSYNC pulse width for DSP/TDM mode */
    if ((fmt & SND_SOC_DAIFMT_FORMAT_MASK) == SND_SOC_DAIFMT_DSP_A ||
        (fmt & SND_SOC_DAIFMT_FORMAT_MASK) == SND_SOC_DAIFMT_DSP_B) {
        
        /* Check if we need to set the FSYNC pulse width bit */
        if (tas6754->short_fsync) {
            /* FSYNC pulse < 8 SCLK cycles */
            audio_intf_ctrl &= ~(0x03); /* Clear FS PULSE WIDTH bits (1-0) */
            audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_SHORT;//TAS6754_FS_PULSE_WIDTH_SHORT;
        } else {
            /* FSYNC pulse >= 8 SCLK cycles (default) */
            audio_intf_ctrl &= ~(0x03); /* Clear FS PULSE WIDTH bits (1-0) */
            audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_LONG;// TAS6754_FS_PULSE_WIDTH_LONG;
        }
    }
    
    /* Enable last sample hold for better audio quality during clock errors */
    audio_intf_ctrl &= ~(0x80); /* Clear last sample hold bit (7) */
    audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_ENABLE;//TAS6754_LAST_SAMPLE_HOLD_EN;
    
    /* Write audio interface control register */
    ret = snd_soc_component_write(component, TAS6754_AUDIO_INTERFACE_CTRL, audio_intf_ctrl);
    if (ret < 0)
        return ret;
    
    return 0;
}

/**
 * tas6754_set_dai_tdm_slot
 * @brief: Wrapper function that matches the ALSA SoC signature and calls tas6754_configure_tdm
 * The ALSA SoC set_tdm_slot callback has a specific signature:
 * int (*set_tdm_slot) (struct snd_soc_dai *dai, unsigned int tx_mask, 
 * 							unsigned int rx_mask, int slots, int slot_width);
 * @dai: 
 * @tx_mask:
 * @rx_mask:
 * @slots:
 * @slot_width:
 * 
 * @return:
 */
static int tas6754_set_dai_tdm_slot(struct snd_soc_dai *dai, unsigned int tx_mask,
                            unsigned int rx_mask, int slots, int slot_width)

{
    struct snd_soc_component *component = dai->component;
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    unsigned int channel_map[4] = {0};
    int i, count = 0;
    
    /* Convert tx_mask to channel_map */
    for (i = 0; i < 32 && count < 4; i++) {
        if (tx_mask & (1 << i)) {
            channel_map[count++] = i;
        }
    }
    
    /* Use the stored audio_swap and ll_swap values */
    return tas6754_configure_tdm(component, slot_width, slots, 
                               channel_map, tas6754->audio_swap, tas6754->ll_swap);
}

/**
 * tas6754_configure_tdm
 * @brief: Configures the TAS6754 for TDM operation with the specified
 * parameters. It sets up the proper offsets for each channel group and
 * configures the channel mapping. 
 * 
 * @component: The component instance
 * @slot_width: TDM slot width (16, 20, 24, or 32 bits)
 * @slots: Number of TDM slots (4, 8, or 16)
 * @channel_map: Array mapping physical channels to TDM slots
 * @audio_swap: Audio channel swap option (0-31)
 * @ll_swap: Low latency channel swap option (0-7)
 *
 * @return: Returns 0 on success, or a negative error code.
 */
static int tas6754_configure_tdm(struct snd_soc_component *component,
                               int slot_width, int slots,
                               const unsigned int *channel_map,
                               unsigned int audio_swap, unsigned int ll_swap)
{
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    u8 audio_intf_ctrl, sdin_ctrl, offset_msb = 0;
    u8 audio_offset = 0, ll_offset = 0, ch_swap = 0;
    int ret;
    
    /* Validate parameters */
    if (slot_width != 16 && slot_width != 20 && 
        slot_width != 24 && slot_width != 32) {
        dev_err(component->dev, "Invalid TDM slot width: %d\n", slot_width);
        return -EINVAL;
    }
    
    if (slots != 4 && slots != 8 && slots != 16) {
        dev_err(component->dev, "Invalid TDM slot count: %d\n", slots);
        return -EINVAL;
    }
    
    if (audio_swap > 31) {
        dev_err(component->dev, "Invalid audio swap option: %u\n", audio_swap);
        return -EINVAL;
    }
    
    if (ll_swap > 7) {
        dev_err(component->dev, "Invalid low latency swap option: %u\n", ll_swap);
        return -EINVAL;
    }
    
    /* Read current audio interface control register */
    ret = snd_soc_component_read(component, TAS6754_AUDIO_INTERFACE_CTRL, &audio_intf_ctrl);
    if (ret < 0)
        return ret;
    
    /* Set DSP format and enable TDM mode,
	   i.e., enable TDM mode when format is set to TDM/DSP */
    audio_intf_ctrl &= ~(0x1C); /* Clear format and TDM bits */
    audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FORMAT_TDM_DSP | TAS6754_AUDIO_INTERFACE_TDM_MODE;
    
    /* Configure FSYNC pulse width */
    audio_intf_ctrl &= ~(0x03); /* Clear FS PULSE WIDTH bits */
    if (tas6754->short_fsync) {
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_SHORT;
    } else {
        audio_intf_ctrl |= TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_LONG;
    }
    
    /* Write audio interface control register */
    ret = snd_soc_component_write(component, TAS6754_AUDIO_INTERFACE_CTRL, audio_intf_ctrl);
    if (ret < 0)
        return ret;
    
    /* Configure SDIN_CTRL register for data length and source */
    sdin_ctrl = 0;

    /* Set data length based on slot width */
    switch (slot_width) {
    case 16:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_16BIT | TAS6754_SDIN_CTRL_WL_SELECT_2_16BIT;
        break;
    case 20:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_20BIT | TAS6754_SDIN_CTRL_WL_SELECT_2_20BIT;
        break;
    case 24:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_24BIT | TAS6754_SDIN_CTRL_WL_SELECT_2_24BIT;
        break;
    case 32:
        sdin_ctrl |= TAS6754_SDIN_CTRL_WL_SELECT_1_32BIT | TAS6754_SDIN_CTRL_WL_SELECT_2_32BIT;
        break;
    }
    
    /* In TDM mode, typically all channels come from SDIN_1 */
    sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN1;
    sdin_ctrl |= TAS6754_SDIN_CTRL_TDM_LL_SDIN1;
    
    /* Write SDIN_CTRL register */
    ret = snd_soc_component_write(component, TAS6754_SDIN_CTRL, sdin_ctrl);
    if (ret < 0)
        return ret;
    
    /* Calculate offsets for audio channels based on channel map */
    if (channel_map) {
        /* Find the offset for the first used channel */
        unsigned int audio_ch_offset = channel_map[0] * slot_width;
        
        /* Set audio channel offset */
        offset_msb |= ((audio_ch_offset >> 8) & 0x03) << 6;
        audio_offset = audio_ch_offset & 0xFF;
        
        /* If low latency channels are enabled, set their offset too */
        if (tas6754->ll_enabled) {
            unsigned int ll_ch_offset = 0;
            
            /* Find the offset for the first low latency channel */
            /* This is typically after the regular audio channels */
            ll_ch_offset = slots * slot_width;
            
            /* Set low latency channel offset */
            offset_msb |= ((ll_ch_offset >> 8) & 0x03) << 4;
            ll_offset = ll_ch_offset & 0xFF;
        }
    }
    
    /* Configure channel swap settings */
    ch_swap = audio_swap & 0x1F;           /* Audio channel swap (bits 4-0) */
    ch_swap |= (ll_swap & 0x07) << 5;      /* LL channel swap (bits 7-5) */
    
    /* Set channel swap MSB in SDIN_OFFSET_MSB register */
    offset_msb |= ((ll_swap >> 2) & 0x01) << 3;  /* LL CH SWAP MSB (bit 3) */
    offset_msb |= ((ll_swap >> 3) & 0x01) << 2;  /* LL CH SWAP MSB (bit 2) */
    
    /* Write offset registers */
    ret = snd_soc_component_write(component, TAS6754_SDIN_OFFSET_MSB, offset_msb);
    if (ret < 0)
        return ret;
    
    ret = snd_soc_component_write(component, TAS6754_SDIN_AUDIO_OFFSET, audio_offset);
    if (ret < 0)
        return ret;
    
    /* Write low latency offset if enabled */
    if (tas6754->ll_enabled) {
        ret = snd_soc_component_write(component, TAS6754_SDIN_LL_OFFSET, ll_offset);
        if (ret < 0)
            return ret;
    }
    
    /* Write channel swap register */
    ret = snd_soc_component_write(component, TAS6754_SDIN_CH_SWAP, ch_swap);
    if (ret < 0)
        return ret;
    
    /* Store TDM configuration */
    tas6754->tdm_mode = true;
    tas6754->bit_depth = slot_width;
    tas6754->tdm_slots = slots;
    tas6754->tdm_slot_width = slot_width;
    tas6754->audio_swap = audio_swap;
    tas6754->ll_swap = ll_swap;
    
    return 0;
}


/**
 * tas6754_configure_low_latency
 * @brief: Enables or disables the low latency channels in TDM mode
 * and configures their offset.
 * 
 * @component: The component instance
 * @enable: Whether to enable low latency channels
 * @offset: Offset in SCLKs for low latency channels (0-511)
 * 
 * @return: Returns 0 on success, or a negative error code.
 */
static int tas6754_configure_low_latency(struct snd_soc_component *component,
                                       bool enable, unsigned int offset)
{
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    u8 offset_msb;
    int ret;
    
    /* Validate offset */
    if (offset > 511) {
        dev_err(component->dev, "Invalid low latency offset: %u (max 511)\n", offset);
        return -EINVAL;
    }
    
    /* Enable/disable low latency channels */
    ret = snd_soc_component_write(component, TAS6754_LL_EN, enable ? 0x0F : 0x00);
    if (ret < 0)
        return ret;
    
    /* Store the settings */
    tas6754->ll_enabled = enable;
    tas6754->ll_offset = offset;
    
    /* If TDM mode is active, update the offset registers */
    if (tas6754->tdm_mode) {
        /* Read current offset MSB register */
        ret = snd_soc_component_read(component, TAS6754_SDIN_OFFSET_MSB, &offset_msb);
        if (ret < 0)
            return ret;
        
        /* Update LL offset MSB (bits 8-9) in bits 5-4 of SDIN_OFFSET_MSB */
        offset_msb &= ~(0x30); /* Clear bits 5-4 */
        offset_msb |= ((offset >> 8) & 0x03) << 4;
        
        /* Write updated offset MSB register */
        ret = snd_soc_component_write(component, TAS6754_SDIN_OFFSET_MSB, offset_msb);
        if (ret < 0)
            return ret;
        
        /* Write LL offset LSB register */
        ret = snd_soc_component_write(component, TAS6754_SDIN_LL_OFFSET, offset & 0xFF);
        if (ret < 0)
            return ret;
    }
    
    return 0;
}

/**
 * tas6754_configure_sdin2
 * @brief: Configures one of the GPIO pins as SDIN2 for receiving
 * audio data for channels 3 and 4 in I2S mode or as an alternative
 * input in TDM mode.
 * 
 * @component: The component instance
 * @gpio_num: GPIO number to use (1 or 2) 
 *
 * @return: Returns 0 on success, or a negative error code.
 */
static int tas6754_configure_sdin2(struct snd_soc_component *component, int gpio_num)
{
    u8 gpio_input_sdin2 = 0;
    u8 gpio_ctrl;
    int ret;
    
    /* Read current GPIO_CTRL register value */
    ret = snd_soc_component_read(component, TAS6754_GPIO_CTRL, &gpio_ctrl);
    if (ret < 0)
        return ret;
    
    /* Select which GPIO to use as SDIN2 */
    switch (gpio_num) {
    case 1:
        gpio_input_sdin2 |= TAS6754_GPIO_INPUT_SDIN2_GPIO1; /* GPIO1 as SDIN2 */
        /* Set GPIO1 as input */
        gpio_ctrl &= ~(0x01 << 7);
        break;
    case 2:
        gpio_input_sdin2 |= TAS6754_GPIO_INPUT_SDIN2_GPIO2; /* GPIO2 as SDIN2 */
        /* Set GPIO2 as input */
        gpio_ctrl &= ~(0x01 << 6);
        break;
    default:
        dev_err(component->dev, "Invalid GPIO number for SDIN2: %d\n", gpio_num);
        return -EINVAL;
    }
    
    /* Write GPIO_INPUT_SDIN2 register */
    ret = snd_soc_component_write(component, TAS6754_GPIO_INPUT_SDIN2, gpio_input_sdin2);
    if (ret < 0)
        return ret;
    
    /* Write updated GPIO_CTRL register */
    ret = snd_soc_component_write(component, TAS6754_GPIO_CTRL, gpio_ctrl);
    if (ret < 0)
        return ret;
    
    return 0;
}


/* ALSA Controls for Low Latency Configuration */

//TODO: understand the purpose of this function and adapt as needed
static int tas6754_low_latency_get(struct snd_kcontrol *kcontrol,
                                 struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    
    ucontrol->value.integer.value[0] = tas6754->ll_enabled ? 1 : 0;
    
    return 0;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_low_latency_put(struct snd_kcontrol *kcontrol,
                                 struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    bool enable = !!ucontrol->value.integer.value[0];
    int ret;
    
    if (tas6754->ll_enabled == enable)
        return 0;
    
    ret = tas6754_configure_low_latency(component, enable, tas6754->ll_offset);
    if (ret < 0)
        return ret;
    
    return 1;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_ll_offset_get(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    
    ucontrol->value.integer.value[0] = tas6754->ll_offset;
    
    return 0;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_ll_offset_put(struct snd_kcontrol *kcontrol,
                               struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    unsigned int offset = ucontrol->value.integer.value[0];
    int ret;
    
    if (offset > 511)
        return -EINVAL;
    
    if (tas6754->ll_offset == offset)
        return 0;
    
    tas6754->ll_offset = offset;
    
    if (tas6754->ll_enabled && tas6754->tdm_mode) {
        ret = tas6754_configure_low_latency(component, true, offset);
        if (ret < 0)
            return ret;
    }
    
    return 1;
}
/* ALSA Controls for Low Latency Configuration */

/* ALSA Controls for Channel Swap Configuration */

//TODO: understand the purpose of this function and adapt as needed
static int tas6754_audio_swap_get(struct snd_kcontrol *kcontrol,
                                struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    
    ucontrol->value.integer.value[0] = tas6754->audio_swap;
    
    return 0;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_audio_swap_put(struct snd_kcontrol *kcontrol,
                                struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    unsigned int val = ucontrol->value.integer.value[0];
    u8 ch_swap;
    int ret;
    
    if (val > 31)
        return -EINVAL;
    
    if (tas6754->audio_swap == val)
        return 0;
    
    tas6754->audio_swap = val;
    
    if (tas6754->tdm_mode) {
        /* Update channel swap register */
        ret = snd_soc_component_read(component, TAS6754_SDIN_CH_SWAP, &ch_swap);
        if (ret < 0)
            return ret;
        
        ch_swap &= 0xE0; /* Clear audio swap bits (4-0) */
        ch_swap |= val & 0x1F;
        
        ret = snd_soc_component_write(component, TAS6754_SDIN_CH_SWAP, ch_swap);
        if (ret < 0)
            return ret;
    }
    
    return 1;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_ll_swap_get(struct snd_kcontrol *kcontrol,
                             struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    
    ucontrol->value.integer.value[0] = tas6754->ll_swap;
    
    return 0;
}
//TODO: understand the purpose of this function and adapt as needed
static int tas6754_ll_swap_put(struct snd_kcontrol *kcontrol,
                             struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_component *component = snd_soc_kcontrol_component(kcontrol);
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    unsigned int val = ucontrol->value.integer.value[0];
    u8 ch_swap, offset_msb;
    int ret;
    
    if (val > 7)
        return -EINVAL;
    
    if (tas6754->ll_swap == val)
        return 0;
    
    tas6754->ll_swap = val;
    
    if (tas6754->tdm_mode && tas6754->ll_enabled) {
        /* Update channel swap register */
        ret = snd_soc_component_read(component, TAS6754_SDIN_CH_SWAP, &ch_swap);
        if (ret < 0)
            return ret;
        
        ch_swap &= 0x1F; /* Clear LL swap bits (7-5) */
        ch_swap |= (val & 0x07) << 5;
        
        ret = snd_soc_component_write(component, TAS6754_SDIN_CH_SWAP, ch_swap);
        if (ret < 0)
            return ret;
        
        /* Update LL CH SWAP MSB in SDIN_OFFSET_MSB register */
        ret = snd_soc_component_read(component, TAS6754_SDIN_OFFSET_MSB, &offset_msb);
        if (ret < 0)
            return ret;
        
        /* Clear bits 3-2 (LL CH SWAP MSB) */
        offset_msb &= ~(0x0C);
        
        /* Set LL CH SWAP MSB bits */
        offset_msb |= ((val >> 2) & 0x01) << 3;  /* Bit 2 of val -> bit 3 of offset_msb */
        offset_msb |= ((val >> 3) & 0x01) << 2;  /* Bit 3 of val -> bit 2 of offset_msb */
        
        ret = snd_soc_component_write(component, TAS6754_SDIN_OFFSET_MSB, offset_msb);
        if (ret < 0)
            return ret;
    }
    
    return 1;
}


/**
 * @brief: ALSA Controls for TAS6754
 * 
 */
static const struct snd_kcontrol_new tas6754_snd_controls[] = {
	/* ALSA Controls for Volume Controls */
    SOC_SINGLE_TLV("Speaker Driver CH1 Playback Volume",
                  TAS6754_DIG_VOL_CH1, 0, TAS6754_DIG_VOL_CH1_MINUS_103DB_MIN, 0, dac_tlv),
    SOC_SINGLE_TLV("Speaker Driver CH2 Playback Volume",
                  TAS6754_DIG_VOL_CH2, 0, TAS6754_DIG_VOL_CH2_MINUS_103DB_MIN, 0, dac_tlv),
    SOC_SINGLE_TLV("Speaker Driver CH3 Playback Volume",
                  TAS6754_DIG_VOL_CH3, 0, TAS6754_DIG_VOL_CH3_MINUS_103DB_MIN, 0, dac_tlv),
    SOC_SINGLE_TLV("Speaker Driver CH4 Playback Volume",
                  TAS6754_DIG_VOL_CH4, 0, TAS6754_DIG_VOL_CH4_MINUS_103DB_MIN, 0, dac_tlv)

    /* ALSA Controls for DC Load Diagnostics Controls */
    SOC_SINGLE("Auto DC Diagnostics", TAS6754_DC_LDG_CTRL,
              TAS6754_DC_LDG_BYPASS_SHIFT, 1, TAS6754_INVERT_CONTROL),
    SOC_SINGLE("Short/Open Load Detection", TAS6754_DC_LDG_CTRL,
              TAS6754_DC_LDG_SLOL_DISABLE_SHIFT, 1, TAS6754_INVERT_CONTROL),
    SOC_SINGLE("Bypass Diagnostic Wait Loop", TAS6754_DC_LDG_CTRL,
              TAS6754_DC_LDG_WAIT_BYPASS_SHIFT, 1, TAS6754_NORMAL_CONTROL),
	SOC_ENUM("Diagnostic Buffer Wait Time", tas6754_ldg_buffer_wait_enum),	
    SOC_SINGLE_STROBE("Abort Diagnostics", TAS6754_DC_LDG_CTRL,
                     TAS6754_DC_LDG_ABORT_SHIFT, 1),
    
    /* ALSA Controls from Line-Out Detection Controls */
    SOC_SINGLE("CH1 Line-Out Detection", TAS6754_DC_LDG_LO_CTRL,
              TAS6754_CH1_LO_LDG_ENABLE_SHIFT, 1, TAS6754_NORMAL_CONTROL),
    SOC_SINGLE("CH2 Line-Out Detection", TAS6754_DC_LDG_LO_CTRL,
              TAS6754_CH2_LO_LDG_ENABLE_SHIFT, 1, TAS6754_NORMAL_CONTROL),
    SOC_SINGLE("CH3 Line-Out Detection", TAS6754_DC_LDG_LO_CTRL,
              TAS6754_CH3_LO_LDG_ENABLE_SHIFT, 1, TAS6754_NORMAL_CONTROL),
    SOC_SINGLE("CH4 Line-Out Detection", TAS6754_DC_LDG_LO_CTRL,
              TAS6754_CH4_LO_LDG_ENABLE_SHIFT, 1, TAS6754_NORMAL_CONTROL),

    /* ALSA Controls for Low Latency Configuration */
    SOC_SINGLE_BOOL_EXT("Low Latency Enable", 0,
                       tas6754_low_latency_get, tas6754_low_latency_put),
    SOC_SINGLE_EXT("Low Latency Offset", SND_SOC_NOPM, 0, 511, 0,
                  tas6754_ll_offset_get, tas6754_ll_offset_put),

	/* ALSA Controls for Channel Swap Configuration */
	/* These options were added upon considering register TAS6754_SDIN_CH_SWAP (0x2A), if too complex, remove them! */
    SOC_SINGLE_EXT("Audio Channel Swap", SND_SOC_NOPM, 0, 31, 0,
                  tas6754_audio_swap_get, tas6754_audio_swap_put),
    SOC_SINGLE_EXT("Low Latency Channel Swap", SND_SOC_NOPM, 0, 7, 0,
                  tas6754_ll_swap_get, tas6754_ll_swap_put),
};




/* TODO: double check how to implemente mute in tas6754 */
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
	.set_bias_level		= tas6754_set_bias_level,//ok
	.controls			= tas6754_snd_controls,//ok
	.num_controls		= ARRAY_SIZE(tas6754_snd_controls),
	.dapm_widgets		= tas6754_dapm_widgets,//double check
	.num_dapm_widgets	= ARRAY_SIZE(tas6754_dapm_widgets),
	.dapm_routes		= tas6754_audio_map,//double check
	.num_dapm_routes	= ARRAY_SIZE(tas6754_audio_map),
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static const struct snd_soc_dai_ops tas6754_speaker_dai_ops = {
	.hw_params	= tas6754_hw_params,//ok
	.set_fmt	= tas6754_set_dai_fmt,//ok
	.set_tdm_slot	= tas6754_set_dai_tdm_slot,//ok
	.mute_stream	= tas6754_mute,//pending
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



    /* TODO: Double check this -> Initialize audio interface settings */
    tas6754->tdm_mode = false;
    tas6754->dsp_a_mode = false;
    tas6754->short_fsync = false;
    tas6754->use_sdin2_for_ch34 = false;
    tas6754->ll_enabled = false;
    tas6754->bit_depth = 24;  /* Default bit depth */
    tas6754->sample_rate = 0;
    tas6754->channels = 0;
    tas6754->channel_offset = 0;
    tas6754->ll_offset = 96;  /* Default LL offset is 96 SCLKs */
    tas6754->sdin2_gpio_num = 1;  /* Default to GPIO1 for SDIN2 */
    tas6754->tdm_slots = 4;   /* Default to 4 TDM slots */
    tas6754->tdm_slot_width = 24;  /* Default to 24-bit slots */
    tas6754->audio_swap = 0;  /* Default audio channel mapping */
    tas6754->ll_swap = 0;     /* Default LL channel mapping */


	/* Initialize DC Load Diagnostics */
    dev_dbg(component->dev, "Initializing DC Load Diagnostics\n");
    snd_soc_component_write(component, TAS6754_DC_LDG_CTRL, TAS6754_DC_LDG_CTRL_DEFAULT);
    snd_soc_component_write(component, TAS6754_DC_LDG_LO_CTRL, TAS6754_DC_LDG_LO_CTRL_DEFAULT);

	/* Initialize volumes to default moderate levels */
    dev_dbg(component->dev, "Setting default volume levels to -20 dB\n");
    snd_soc_component_write(component, TAS6754_DIG_VOL_CH1, TAS6754_DIG_VOL_CH1_DEFAULT);
    snd_soc_component_write(component, TAS6754_DIG_VOL_CH2, TAS6754_DIG_VOL_CH2_DEFAULT);
    snd_soc_component_write(component, TAS6754_DIG_VOL_CH3, TAS6754_DIG_VOL_CH3_DEFAULT);
    snd_soc_component_write(component, TAS6754_DIG_VOL_CH4, TAS6754_DIG_VOL_CH4_DEFAULT);







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
	 * Get control of the PD pin and set it HIGH to take the codec
	 * out of the standby.
	 */
	tas6754->pd_gpio = devm_gpiod_get_optional(tas6754->dev, "pd", GPIOD_OUT_HIGH);
	if (IS_ERR(tas6754->pd_gpio)) {
		if (PTR_ERR(tas6754->pd_gpio) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_info(dev, "failed to get PD GPIO: %ld\n",
			PTR_ERR(tas6754->pd_gpio));
		tas6754->pd_gpio = NULL;
	}

	/*
	 * Get control of the STBY pin and set it LOW to take the codec
	 * out of the standby mode (DEEP SLEEP mode).
	 */
	tas6754->stby_gpio = devm_gpiod_get_optional(tas6754->dev, "standby", GPIOD_OUT_HIGH);
	if (IS_ERR(tas6754->stby_gpio)) {
		if (PTR_ERR(tas6754->stby_gpio) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_info(dev, "failed to get STBY GPIO: %ld\n",
			PTR_ERR(tas6754->stby_gpio));
		tas6754->stby_gpio = NULL;
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
	
	tas6754->pd_gpio = devm_gpiod_get_optional(tas6754->dev, "pd", GPIOD_OUT_HIGH);							  
	if (IS_ERR(tas6754->pd_gpio)) {
		if (PTR_ERR(tas6754->pd_gpio) == -EPROBE_DEFER)
			return -EPROBE_DEFER;
		dev_info(dev, "failed to get PD GPIO: %ld\n",
			PTR_ERR(tas6754->pd_gpio));
		tas6754->pd_gpio = NULL;
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