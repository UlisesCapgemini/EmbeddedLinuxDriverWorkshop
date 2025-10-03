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
#define TAS6754_FAULT_CHECK_50MS 	 	(50)
#define TAS6754_FAULT_CHECK_100MS 	 	(100)
#define TAS6754_FAULT_CHECK_200MS 	 	(200)
#define TAS6754_FAULT_CHECK_300MS 	 	(300)
#define TAS6754_FAULT_CHECK_500MS 	 	(500)
#define TAS6754_FAULT_CHECK_INTERVAL 	TAS6754_FAULT_CHECK_200MS

/** 
* @brief: When using the Linux regulator framework APIs, regulators are typically enabled in the order they appear
* in the array and disabled in reverse order.
* So, if your array has indices 0, 1, 2, the power-up sequence would access them in order 0→1→2,
* and the power-down sequence would access them in order 2→1→0.
*
* This ensures that during power-up, VBAT is applied before DVDD, avoiding the documented fault condition from the datasheet.
* 
* Recommended power-down sequence from the datasheet (PVDD and VBAT first, then DVDD).
* But, he power-down sequence isn't perfect according to the recommendation from the datasheer, but it's likely less critical than
* the power-up sequence.
* If the power-down sequence is absolutely critical, you might need to implement custom power management logic beyond just 
* using the regulator framework's default behavior.
*/
static const char * const tas6754_supply_names[] = {
    "pvdd",		/* Class-D amp output FETs supply. */
    "vbat", 	/* Supply used for higher voltage analog circuits. */
    "dvdd" 		/* Digital power supply. Connect to 3.3-V supply. */
};

#define TAS6754_NUM_SUPPLIES ARRAY_SIZE(tas6754_supply_names)


struct tas6754_data {
    struct device *dev;
    struct regmap *regmap;
    struct regulator_bulk_data supplies[TAS6754_NUM_SUPPLIES];
    struct delayed_work fault_check_work;
    struct mutex mutex;                /* Mutex for thread safety */
    
    /* GPIO control */
    struct gpio_desc *pd_gpio;
    struct gpio_desc *stby_gpio;
    struct gpio_desc *mute_gpio;       /* GPIO for external mute circuit (optional) */
    
    /* State tracking */
    bool powered;                      /* TODO: [Desired] to implement Power State Tracking -> adding a state variable to track power state */
	bool cache_sync;  				   /* TODO: [Optional] to implement whether regcache needs syncing */
    bool playback_active;              /* Whether audio playback is active */
    bool muted;                        /* Whether the device is currently muted */
    
    /* Fault tracking */
    unsigned int last_oc_dc_fault;
    unsigned int last_power_fault;
    unsigned int last_ot_fault;
    unsigned int last_cbc_fault_warn;
    unsigned int last_rtldg_ol_sl_fault;
    
    /* Audio interface configuration */
    bool tdm_mode;                     /* Whether in TDM mode */
    bool dsp_a_mode;                   /* Whether in DSP_A mode (needs 1-bit offset) */
    bool short_fsync;                  /* Whether FSYNC pulse is < 8 SCLK cycles */
    bool use_sdin2_for_ch34;           /* Whether to use SDIN2 for channels 3-4 */
    bool ll_enabled;                   /* Whether low latency channels are enabled */
    bool support_rate_change;          /* TODO: [Desired] to implement support on-the-fly rate changes */
    
    /* Audio parameters */
    unsigned int bit_depth;            /* Current bit depth */
    unsigned int sample_rate;          /* Current sample rate */
    unsigned int channels;             /* Current channel count */
    unsigned int dai_fmt;              /* Current DAI format */
    unsigned int channel_offset;       /* Custom channel offset (in bits) */
    unsigned int ll_offset;            /* Low latency channel offset (in bits) */
    unsigned int sdin2_gpio_num;       /* GPIO number to use for SDIN2 (1 or 2) */
    unsigned int tdm_slots;            /* Number of TDM slots (4, 8, or 16) */
    unsigned int tdm_slot_width;       /* TDM slot width (16, 20, 24, or 32) */
    unsigned int channel_map[4];       /* Mapping of channels to TDM slots */
    unsigned int audio_swap;           /* Audio channel swap option */
    unsigned int ll_swap;              /* Low latency channel swap option */
    unsigned int detected_sclk_ratio;  /* SCLK ratio detected by the device */
	//unsigned int detected_sclk_ratio; /* TODO: [Optional] SCLK ratio detected by the device. Useful for:*/
															/*-Debugging
															-Potentially adjusting settings based on the detected ratio
															-Reporting the ratio through sysfs or debugfs*/
    /* Volume and audio settings */
    unsigned int volume[4];            /* Current volume settings for each channel */
    unsigned int dc_load_diag_config;  /* DC load diagnostics configuration */
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

/**
 * TODO:
 * Ensure tas6754_dac_event function properly manages the power states and fault handling for the TAS6754
 * 		OK- default fault handling capabilities are implemented.
 * 		PENDING - Optionally add more fault types monitoring as required.
 * 		PENDING - Double check if power states management are correctly implemented or is missing.
 */
 /**
 * tas6754_dac_event - DAPM event handler for TAS6754 DAC widgets
 * @brief: Handles power management events for the TAS6754 DAC widgets.
 * 		   It manages the amplifier's power state transitions and fault monitoring system.
 * @w: The DAPM widget
 * @kcontrol: The mixer control that triggered the event
 * @event: The DAPM event type (e.g., SND_SOC_DAPM_POST_PMU or SND_SOC_DAPM_PRE_PMD)
 *
 * On power-up (SND_SOC_DAPM_POST_PMU):
 * - Waits 12ms for the codec to stabilize after shutdown-to-active transition
 * - Resets all fault status tracking variables
 * - Initiates periodic fault checking by scheduling the fault_check_work
 *
 * On power-down (SND_SOC_DAPM_PRE_PMD):
 * - Disables the periodic fault checking by canceling the fault_check_work
 *
 * The fault monitoring system tracks various fault conditions including:
 * - Over-current and DC faults
 * - Power supply faults
 * - Over-temperature conditions
 * - Current boost converter faults/warnings
 * - Real-time load diagnostics, open load, and short load faults
 *
 * @return: Return 0 on success, negative error code on failure
 */
static int tas6754_dac_event(struct snd_soc_dapm_widget *w,
			     struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *component = snd_soc_dapm_to_component(w->dapm);
	struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);

	dev_dbg(component->dev, "%s() event=0x%0x\n", __func__, event);

	/* On Power-Up (POST_PMU) */
	if (event & SND_SOC_DAPM_POST_PMU) {
		/* Observe codec shutdown-to-active time 
		   Waits 12ms after power-up.*/
		msleep(12);

		/** 
		* Turn on TAS6754 periodic fault checking/handling
		* By default the TAS6754 FAULT pin reports fault events and is active low under any of the following conditions:
		*   - Overtemperature shutdown (OTSD) - Latching and non-latching
		*   - Overcurrent Limit and Shutdown events - Latching
		*   - DC Detect - Latching 
		*/
		tas6754->last_oc_dc_fault = 0;
		tas6754->last_power_fault = 0;
		tas6754->last_ot_fault = 0;
		tas6754->last_cbc_fault_warn = 0;
		tas6754->last_rtldg_ol_sl_fault = 0;
		/**
		* TODO:
		* @ref: TRM[4.3.7.1 FAULT Pin, p-50]
		* Additional fault events can be assigned to be reported by the TAS6754 FAULT pin. These include:
		*   - Power Faults - Latching and non-latching
		*   - DC Load Diagnostic faults
		*   - Real-time Load Diagnostic reports - Latching and non-latching
		*   - Clock Errors - Latching
		*   - Charge Pump faults - Latching and non-latching
		*   - Warning events
		*/
		schedule_delayed_work(&tas6754->fault_check_work,
				      msecs_to_jiffies(TAS6754_FAULT_CHECK_INTERVAL));
	/* On Power-Down (PRE_PMD) */
	} else if (event & SND_SOC_DAPM_PRE_PMD) {
		/* Disable TAS6754 periodic fault checking/handling */
		cancel_delayed_work_sync(&tas6754->fault_check_work);
	}

	return 0;
}

/**
 * TAS6754 Channel Mapping Configuration Options
 *
 * This array defines the text labels for the 24 predefined channel mapping configurations
 * available in the TAS6754 amplifier. Each configuration represents a specific routing
 * pattern between input slots and output channels, as defined in the TAS6754 datasheet.
 *
 * These configurations are controlled by the SDIN_CH_SWAP register (Address = 0x2A),
 * bits 0-4, which can be set to values 0-23 to select different mapping patterns.
 *
 * The mapping configurations are as follows:
 *
 * - "Config 0 (1-2-3-4)": Default mapping
 *   Slot 1 → Channel 1, Slot 2 → Channel 2, Slot 3 → Channel 3, Slot 4 → Channel 4
 *
 * - "Config 1 (1-2-4-3)": Rear channels swapped
 *   Slot 1 → Channel 1, Slot 2 → Channel 2, Slot 3 → Channel 4, Slot 4 → Channel 3
 *
 * - "Config 2 (1-3-2-4)": Middle channels swapped
 *   Slot 1 → Channel 1, Slot 2 → Channel 3, Slot 3 → Channel 2, Slot 4 → Channel 4
 *
 * [... continues through all 24 configurations ...]
 *
 * - "Config 23 (4-3-1-2)": Complex rearrangement
 *   Slot 1 → Channel 4, Slot 2 → Channel 3, Slot 3 → Channel 1, Slot 4 → Channel 2
 *
 * These predefined configurations enable:
 *
 * 1. Easy selection of common channel mapping patterns
 * 2. Quick switching between different speaker configurations
 * 3. Support for various audio routing scenarios:
 *    - Standard stereo (left-right)
 *    - Reversed stereo (right-left)
 *    - Front-rear speaker arrangements
 *    - Custom channel arrangements for specific applications
 *
 * The configurations are exposed to user applications through an ALSA enumeration
 * control, allowing selection of the desired mapping pattern without having to
 * individually configure each channel's source.
 *
 * This approach provides a balance between flexibility and ease of use, allowing
 * both simple selection of common patterns and detailed customization when needed.
 */
static const char * const tas6754_ch_map_config_text[] = {
    "Config 0", "Config 1", "Config 2", "Config 3", "Config 4", "Config 5",
    "Config 6", "Config 7", "Config 8", "Config 9", "Config 10", "Config 11",
    "Config 12", "Config 13", "Config 14", "Config 15", "Config 16", "Config 17",
    "Config 18", "Config 19", "Config 20", "Config 21", "Config 22", "Config 23"
};

static const struct soc_enum tas6754_ch_map_config_enum =
    SOC_ENUM_SINGLE(TAS6754_SDIN_CH_SWAP, 0, 24, tas6754_ch_map_config_text);

/**
 * tas6754_dapm_widgets[]
 * @brief: This array defines the audio signal path through the TAS6754 Class-D amplifier using the
 * ALSA DAPM (Dynamic Audio Power Management) framework. The widgets represent the
 * functional blocks in the audio path from digital input to analog output, with per-channel
 * processing capabilities.
 *
 * Widget Types and Functions:
 *
 * 1. AIF_IN ("AIF IN"):
 *    - Represents the digital audio input interface
 *    - Receives I2S or TDM format audio data from the system
 *    - Supports 2-4 channels via I2S or 4-16 channels via TDM
 *    - Handles sample rates from 44.1kHz to 192kHz
 *
 * 2. MUX ("CHx Map"):
 *    - Implements channel mapping functionality for each output channel
 *    - Allows selection of input source (Slot 1-4 or LL Slot 5-8) for each channel
 *    - Enables flexible routing between input slots and output channels
 *    - Connected to the tas6754_chx_mux controls for user configuration
 *
 * 3. DSP ("DSP"):
 *    - Represents the digital signal processing block
 *    - Handles audio processing for all channels
 *    - Implements the Dual Audio DSP Subsystem functionality
 *    - Currently implemented without an event handler (power managed by DAC widgets)
 *    - TODO note discusses potential for dedicated event handler implementation
 *
 * 4. PGA ("CHx Volume"):
 *    - Programmable Gain Amplifier widgets for volume control
 *    - Provides independent digital volume control for each channel
 *    - Maps to the digital volume control registers (0x40-0x43)
 *    - Range from 0dB to -103dB in 0.5dB steps
 *
 * 5. DAC_E ("CHx DAC"):
 *    - Digital-to-PWM converter widgets with event handlers
 *    - Converts digital audio to PWM signals for Class-D amplification
 *    - Uses tas6754_dac_event handler for power management
 *    - Handles POST_PMU (power-up) and PRE_PMD (power-down) events
 *    - Manages fault monitoring and recovery
 *
 * 6. OUT_DRV ("CHx DRV"):
 *    - Output driver stage widgets
 *    - Represents the gate drivers and power FET stages
 *    - Delivers high-efficiency Class-D amplification
 *    - One driver per channel for independent operation
 *
 * 7. OUTPUT ("OUTx"):
 *    - Physical output widgets
 *    - Represents the bridge-tied load (BTL) outputs to speakers
 *    - Four independent output channels
 *    - Capable of delivering up to 30W per channel into 4Ω loads
 *
 * Power Management:
 * - The widgets use SND_SOC_NOPM (No Power Management) for register control
 * - Actual power management is handled by the tas6754_dac_event handler
 * - This handler manages power sequencing and fault monitoring
 * - The handler is called during power-up (POST_PMU) and power-down (PRE_PMD)
 *
 * Note on DSP Event Handler:
 * The TODO comment discusses whether a separate tas6754_dsp_event handler should be
 * implemented for the DSP widget. Currently, power management for the entire device
 * is handled by the DAC event handlers. A separate DSP handler could be added for
 * more granular control if needed in the future.
 */
static const struct snd_soc_dapm_widget tas6754_dapm_widgets[] = {
    /* Digital Input */
    SND_SOC_DAPM_AIF_IN("AIF IN", "Playback", 0, SND_SOC_NOPM, 0, 0),
    
    /* Channel Mapping */
    SND_SOC_DAPM_MUX("CH1 Map", SND_SOC_NOPM, 0, 0, &tas6754_ch1_mux),
    SND_SOC_DAPM_MUX("CH2 Map", SND_SOC_NOPM, 0, 0, &tas6754_ch2_mux),
    SND_SOC_DAPM_MUX("CH3 Map", SND_SOC_NOPM, 0, 0, &tas6754_ch3_mux),
    SND_SOC_DAPM_MUX("CH4 Map", SND_SOC_NOPM, 0, 0, &tas6754_ch4_mux),
    
	/* DSP Processing (without an event handler)*/
	SND_SOC_DAPM_DSP("DSP", SND_SOC_NOPM, 0, 0, NULL),

    /**
     * @TODO:
	 * tas6754_dsp_event - Handler to manage power sequencing
	 * Check if a DSP event handler tas6754_dsp_event is needed for specific power management or configuration tasks 
	 * or we can reuse the existing handler tas6754_dac_event? 
	 * Check is we can use the same event handler, the existing tas6754_dac_event function for both the 
	 * DAC and DSP widgets?
	 * separate handler implementation is better? Yes, to keep things modular.
	 */
     /* SND_SOC_DAPM_DSP_E("DSP", SND_SOC_NOPM, 0, 0, NULL, 0,
                     tas6754_dsp_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),*/
    
    /* Volume Controls */
    SND_SOC_DAPM_PGA("CH1 Volume", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("CH2 Volume", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("CH3 Volume", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_PGA("CH4 Volume", SND_SOC_NOPM, 0, 0, NULL, 0),
    
    /* DACs (Digital to PWM) */
    SND_SOC_DAPM_DAC_E("CH1 DAC", NULL, SND_SOC_NOPM, 0, 0,
                     tas6754_dac_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
    SND_SOC_DAPM_DAC_E("CH2 DAC", NULL, SND_SOC_NOPM, 0, 0,
                     tas6754_dac_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
    SND_SOC_DAPM_DAC_E("CH3 DAC", NULL, SND_SOC_NOPM, 0, 0,
                     tas6754_dac_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
    SND_SOC_DAPM_DAC_E("CH4 DAC", NULL, SND_SOC_NOPM, 0, 0,
                     tas6754_dac_event, SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD),
    
    /* Output Drivers */
    SND_SOC_DAPM_OUT_DRV("CH1 DRV", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_OUT_DRV("CH2 DRV", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_OUT_DRV("CH3 DRV", SND_SOC_NOPM, 0, 0, NULL, 0),
    SND_SOC_DAPM_OUT_DRV("CH4 DRV", SND_SOC_NOPM, 0, 0, NULL, 0),
    
    /* Outputs */
    SND_SOC_DAPM_OUTPUT("OUT1"),
    SND_SOC_DAPM_OUTPUT("OUT2"),
    SND_SOC_DAPM_OUTPUT("OUT3"),
    SND_SOC_DAPM_OUTPUT("OUT4")
};

/**
 * TAS6754 Channel Mapping Text Options
 *
 * This array defines the text labels for the input slot options available in the
 * channel mapping controls of the TAS6754 Class-D amplifier. These labels represent the
 * possible audio sources that can be routed to each amplifier channel through
 * the DAPM MUX widgets.
 *
 * The array includes two categories of input slots:
 *
 * 1. Standard Audio Slots (1-4):
 *    - "Slot 1": First slot in the I2S/TDM frame, typically left front channel
 *    - "Slot 2": Second slot in the I2S/TDM frame, typically right front channel
 *    - "Slot 3": Third slot in the I2S/TDM frame, typically left rear channel
 *    - "Slot 4": Fourth slot in the I2S/TDM frame, typically right rear channel
 *
 * 2. Low Latency Slots (5-8):
 *    - "LL Slot 5": First low latency slot in the TDM frame
 *    - "LL Slot 6": Second low latency slot in the TDM frame
 *    - "LL Slot 7": Third low latency slot in the TDM frame
 *    - "LL Slot 8": Fourth low latency slot in the TDM frame
 *
 * These text labels are used in several contexts:
 *
 * - As options in the ALSA mixer controls for channel mapping
 * - As connection identifiers in the DAPM audio map
 * - In user interfaces like alsamixer to display available routing options
 *
 * The flexible routing enabled by these options allows:
 * - Any input slot to be routed to any amplifier channel
 * - Support for both standard and low latency audio paths
 * - Dynamic reconfiguration of the audio routing at runtime
 * - Implementation of various channel mapping scenarios (stereo, quad, etc.)
 *
 * When used with the channel mapping MUX widgets, these options create a
 * fully configurable routing matrix between input slots and output channels.
 */
static const char * const tas6754_ch_map_text[] = {
    "Slot 1", "Slot 2", "Slot 3", "Slot 4",
    "LL Slot 5", "LL Slot 6", "LL Slot 7", "LL Slot 8"
};

static const struct soc_enum tas6754_ch1_map_enum =
    SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, ARRAY_SIZE(tas6754_ch_map_text), tas6754_ch_map_text);
static const struct soc_enum tas6754_ch2_map_enum =
    SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, ARRAY_SIZE(tas6754_ch_map_text), tas6754_ch_map_text);
static const struct soc_enum tas6754_ch3_map_enum =
    SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, ARRAY_SIZE(tas6754_ch_map_text), tas6754_ch_map_text);
static const struct soc_enum tas6754_ch4_map_enum =
    SOC_ENUM_SINGLE(SND_SOC_NOPM, 0, ARRAY_SIZE(tas6754_ch_map_text), tas6754_ch_map_text);

static const struct snd_kcontrol_new tas6754_ch1_mux =
    SOC_DAPM_ENUM("CH1 Source", tas6754_ch1_map_enum);
static const struct snd_kcontrol_new tas6754_ch2_mux =
    SOC_DAPM_ENUM("CH2 Source", tas6754_ch2_map_enum);
static const struct snd_kcontrol_new tas6754_ch3_mux =
    SOC_DAPM_ENUM("CH3 Source", tas6754_ch3_map_enum);
static const struct snd_kcontrol_new tas6754_ch4_mux =
    SOC_DAPM_ENUM("CH4 Source", tas6754_ch4_map_enum);

/**
 * TAS6754 DAPM Audio Map
 *
 * This array defines the connections between the DAPM widgets in the TAS6754 Class-D amplifier's
 * audio signal path. Each entry represents a connection from a source widget to a sink
 * widget, optionally with a control that enables/disables the connection.
 *
 * The audio map creates a complete directed graph of the audio signal flow from input
 * to output, with the following key sections:
 *
 * 1. Input to Channel Mapping Connections:
 *    These connections define how input audio slots from the digital interface (AIF IN)
 *    can be routed to each amplifier channel. The flexible routing allows:
 *    - Any input slot (1-4) to be routed to any amplifier channel (1-4)
 *    - Low latency slots (5-8) to be routed to any amplifier channel
 *    - Each amplifier channel to select its input source independently
 *    - Dynamic switching between different input sources via ALSA controls
 *
 *    For example: {"CH1 Map", "Slot 2", "AIF IN"} creates a connection from the
 *    digital input (AIF IN) to channel 1's input selector (CH1 Map), when "Slot 2"
 *    is selected as the source.
 *
 * 2. DSP Processing Connections:
 *    These connections route the mapped input signals to the DSP processing block,
 *    which handles digital audio processing for all channels.
 *
 * 3. Volume Control Connections:
 *    These connections route the processed audio from the DSP to individual volume
 *    controls for each channel, enabling independent volume adjustment.
 *
 * 4. DAC Connections:
 *    These connections route the volume-adjusted signals to the Digital-to-PWM
 *    converters for each channel, which transform the digital audio into PWM signals
 *    for Class-D amplification.
 *
 * 5. Output Driver Connections:
 *    These connections route the PWM signals to the output driver stages, which
 *    include the gate drivers and power FETs for each channel.
 *
 * 6. Output Connections:
 *    These connections route the amplified signals to the physical output pins
 *    of the TAS6754 amplifier.
 *
 * The complete audio path for each channel is:
 * AIF IN → CHx Map → DSP → CHx Volume → CHx DAC → CHx DRV → OUTx
 *
 * This flexible routing architecture allows for:
 * - Dynamic channel mapping through ALSA controls
 * - Independent processing and volume control for each channel
 * - Support for both regular and low latency audio paths
 * - Proper power sequencing through the DAPM framework
 */
static const struct snd_soc_dapm_route tas6754_audio_map[] = {
    /* Input to Channel Mapping */
    {"CH1 Map", "Slot 1", "AIF IN"},
    {"CH1 Map", "Slot 2", "AIF IN"},
    {"CH1 Map", "Slot 3", "AIF IN"},
    {"CH1 Map", "Slot 4", "AIF IN"},
    {"CH1 Map", "LL Slot 5", "AIF IN"},
    {"CH1 Map", "LL Slot 6", "AIF IN"},
    {"CH1 Map", "LL Slot 7", "AIF IN"},
    {"CH1 Map", "LL Slot 8", "AIF IN"},
    
    {"CH2 Map", "Slot 1", "AIF IN"},
    {"CH2 Map", "Slot 2", "AIF IN"},
    {"CH2 Map", "Slot 3", "AIF IN"},
    {"CH2 Map", "Slot 4", "AIF IN"},
    {"CH2 Map", "LL Slot 5", "AIF IN"},
    {"CH2 Map", "LL Slot 6", "AIF IN"},
    {"CH2 Map", "LL Slot 7", "AIF IN"},
    {"CH2 Map", "LL Slot 8", "AIF IN"},
    
    {"CH3 Map", "Slot 1", "AIF IN"},
    {"CH3 Map", "Slot 2", "AIF IN"},
    {"CH3 Map", "Slot 3", "AIF IN"},
    {"CH3 Map", "Slot 4", "AIF IN"},
    {"CH3 Map", "LL Slot 5", "AIF IN"},
    {"CH3 Map", "LL Slot 6", "AIF IN"},
    {"CH3 Map", "LL Slot 7", "AIF IN"},
    {"CH3 Map", "LL Slot 8", "AIF IN"},
    
    {"CH4 Map", "Slot 1", "AIF IN"},
    {"CH4 Map", "Slot 2", "AIF IN"},
    {"CH4 Map", "Slot 3", "AIF IN"},
    {"CH4 Map", "Slot 4", "AIF IN"},
    {"CH4 Map", "LL Slot 5", "AIF IN"},
    {"CH4 Map", "LL Slot 6", "AIF IN"},
    {"CH4 Map", "LL Slot 7", "AIF IN"},
    {"CH4 Map", "LL Slot 8", "AIF IN"},
    
    /* DSP Processing */
    {"DSP", NULL, "CH1 Map"},
    {"DSP", NULL, "CH2 Map"},
    {"DSP", NULL, "CH3 Map"},
    {"DSP", NULL, "CH4 Map"},
    
    /* Volume Controls */
    {"CH1 Volume", NULL, "DSP"},
    {"CH2 Volume", NULL, "DSP"},
    {"CH3 Volume", NULL, "DSP"},
    {"CH4 Volume", NULL, "DSP"},
    
    /* DACs */
    {"CH1 DAC", NULL, "CH1 Volume"},
    {"CH2 DAC", NULL, "CH2 Volume"},
    {"CH3 DAC", NULL, "CH3 Volume"},
    {"CH4 DAC", NULL, "CH4 Volume"},
    
    /* Output Drivers */
    {"CH1 DRV", NULL, "CH1 DAC"},
    {"CH2 DRV", NULL, "CH2 DAC"},
    {"CH3 DRV", NULL, "CH3 DAC"},
    {"CH4 DRV", NULL, "CH4 DAC"},
    
    /* Outputs */
    {"OUT1", NULL, "CH1 DRV"},
    {"OUT2", NULL, "CH2 DRV"},
    {"OUT3", NULL, "CH3 DRV"},
    {"OUT4", NULL, "CH4 DRV"}
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
 * TAS6754 ALSA Controls
 *
 * This array defines the mixer controls exposed to user applications through the ALSA
 * framework. These controls allow configuration of various aspects of the TAS6754
 * Class-D amplifier, including volume levels, diagnostics features, and channel mapping.
 *
 * Control Categories:
 *
 * 1. Volume Controls:
 *    Per-channel digital volume controls with TLV (Threshold Limit Value) scaling.
 *    - Range: 0 dB (maximum volume) to -103 dB (minimum volume), plus mute
 *    - Step size: 0.5 dB
 *    - Each channel can be independently controlled
 *    - TLV scaling ensures proper dB representation in ALSA applications
 *
 * 2. DC Load Diagnostics Controls:
 *    Controls for the amplifier's built-in diagnostic capabilities.
 *    - Auto DC Diagnostics: Enables/disables automatic diagnostics after faults
 *    - Short/Open Load Detection: Enables/disables detection of shorted/open loads
 *    - Bypass Diagnostic Wait Loop: Controls waiting behavior during diagnostics
 *    - Diagnostic Buffer Wait Time: Sets buffer wait time (1ms, 2ms, 5ms, or 10ms)
 *    - Abort Diagnostics: Momentary control to abort an ongoing diagnostic operation
 *
 * 3. Line-Out Detection Controls:
 *    Per-channel controls for detecting line-out (headphone) connections.
 *    - Enables DC Load Diagnostics to check for line-out loads on each channel
 *    - Useful for automatic switching between speaker and headphone modes
 *    - Can be independently enabled for each channel
 *
 * 4. Low Latency Configuration:
 *    Controls for the TAS6754's low latency audio path.
 *    - Low Latency Enable: Activates the separate low latency signal path
 *    - Low Latency Offset: Sets the offset (in SCLKs) for low latency channels
 *
 * 5. Channel Mapping Configuration:
 *    Controls for flexible routing between input slots and output channels.
 *    - Audio Channel Swap: Configures the mapping between input slots and output channels
 *    - Low Latency Channel Swap: Configures mapping for low latency channels
 *    - Channel Mapping Config: Selects from 24 predefined mapping configurations
 *    - LL Channel Mapping Config: Selects mapping configuration for low latency channels
 *
 * Implementation Notes:
 * - Some controls use inverted logic (TAS6754_INVERT_CONTROL) where the register bit
 *   meaning is opposite to the intuitive control behavior
 * - Extended controls (SOC_SINGLE_EXT) use custom get/put handlers for complex operations
 * - The STROBE control (Abort Diagnostics) automatically returns to inactive state
 * - TLV controls provide proper dB scaling for volume controls in ALSA applications
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
    SOC_SINGLE_EXT("Audio Channel Swap", SND_SOC_NOPM, 0, 31, 0,
                  tas6754_audio_swap_get, tas6754_audio_swap_put),
    SOC_SINGLE_EXT("Low Latency Channel Swap", SND_SOC_NOPM, 0, 7, 0,
                  tas6754_ll_swap_get, tas6754_ll_swap_put),

	/* ALSA Controls for Channel Mapping Configuration */
    SOC_ENUM("Channel Mapping Config", tas6754_ch_map_config_enum),
    
    /* ALSA Controls for Low Latency Channel Mapping Configuration */
    SOC_SINGLE("LL Channel Mapping Config", TAS6754_SDIN_CH_SWAP, 5, 7, 0),
};


/**
 * tas6754_mute - Mute or unmute all channels
 * @brief: Mutes or unmutes all channels of the TAS6754 amplifier.
 * Unlike the TAS6424, the TAS6754 doesn't have a dedicated MUTE pin,
 * so we control muting through the channel state registers.
 * 
 * When muting, we keep the channels in PLAY state but set the mute bit.
 * When unmuting, we keep the channels in PLAY state and clear the mute bit.
 * 
 * @dai: DAI instance
 * @mute: Mute state (1 = mute, 0 = unmute)
 * @direction: Stream direction (not used)
 *
 * @return: Return 0 on success, negative error code on failure
 */
static int tas6754_mute(struct snd_soc_dai *dai, int mute, int direction)
{
    struct snd_soc_component *component = dai->component;
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);

    dev_dbg(component->dev, "%s() mute=%d\n", __func__, mute);

    /* If a mute GPIO is defined (for external muting circuit), use it */
    if (tas6754->mute_gpio) {
        gpiod_set_value_cansleep(tas6754->mute_gpio, mute);
        return 0;
    }

    /* Otherwise use register control to mute/unmute all channels */
    if (mute) {
        /* Set channels to PLAY state but with mute bit set */
        snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_PLAY__MUTE);
        snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_PLAY__MUTE);
    } else {
        /* Set channels to PLAY state with normal volume (no mute) */
        snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_PLAY__NORMAL_VOLUME);
        snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_PLAY__NORMAL_VOLUME);
    }

    return 0;
}

/**
 * tas6754_power_off - Power down the TAS6754 amplifier
 * @component: The component instance
 *
 * This function powers down the TAS6754 amplifier following the recommended
 * power-down sequence from the datasheet:
 * 1. Put channels in HI-Z state to prevent pops/clicks
 * 2. Put device in standby (DEEP SLEEP) mode
 * 3. Wait 10ms before removing power
 * 4. Assert PD pin for complete shutdown
 * 5. Disable power supplies in the correct sequence
 *
 * Return: 0 on success, negative error code on failure
 */
static int tas6754_power_off(struct snd_soc_component *component)
{
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    int ret;

    dev_dbg(component->dev, "%s: Powering off TAS6754\n", __func__);

    /* If using external mute circuit, ensure it's muted before power-off */
    if (tas6754->mute_gpio){
        gpiod_set_value_cansleep(tas6754->mute_gpio, 1);
	} else {
    /* If no external mute circuit, muting will be handled by HI-Z state */
    dev_dbg(component->dev, "No mute GPIO, using HI-Z state for muting\n");
	}

    /* Put all channels in HI-Z state to prevent pops/clicks.
	   The HI-Z state effectively disconnects the outputs, which serves the same purpose as muting. */
	ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_HIZ);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set CH1/CH2 to HI-Z: %d\n", ret);
		return ret;
	}
	ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_HIZ);
	if (ret < 0) {
		dev_err(component->dev, "Failed to set CH3/CH4 to HI-Z: %d\n", ret);
		return ret;
	}

    /* If STDY GPIO pin configured */
    if (tas6754->stby_gpio) {
        gpiod_set_value_cansleep(tas6754->stby_gpio, 0); /* Set low (active) */
    } else {
        /* If no STDY GPIO pin configured, use register control for DEEP SLEEP mode */
        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_DEEP_SLEEP);
		if (ret < 0) {
			dev_err(component->dev, "Failed to set CH1/CH2 to DEEP SLEEP: %d\n", ret);
			return ret;
		}
        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_DEEP_SLEEP);
		if (ret < 0) {
			dev_err(component->dev, "Failed to set CH3/CH4 to DEEP SLEEP: %d\n", ret);
			return ret;
		}
    }

    /* Wait at least 10ms before removing power supplies as per datasheet */
    msleep(10);

    /* Switch regmap to cache-only mode to preserve settings */
	/* regcache operations after the register writes to ensure the writes take effect */
    regcache_cache_only(tas6754->regmap, true);
    regcache_mark_dirty(tas6754->regmap);

    /* For complete shutdown, assert PD pin */
	if (tas6754->pd_gpio) {
		gpiod_set_value_cansleep(tas6754->pd_gpio, 0); /* Set low (active) */
	} else {
		/* 
		* No PD GPIO available. The device will remain in DEEP_SLEEP mode
		* until power supplies are removed. This is not a complete shutdown
		* but the lowest power state achievable without PD pin control.
		*/
		dev_dbg(component->dev, "No PD GPIO, device will remain in DEEP_SLEEP until power removed\n");
    }

	/** 
	* @TODO:[DTS]To ensure regulators are disabled in the correct sequence (PVDD and VBAT first, then DVDD),
				you should define them in the correct order in your device structure and device tree.
				
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

	/** 
	* @TODO:
	* Recommended power-down sequence from the datasheet (PVDD and VBAT first, then DVDD).
	* But, the power-down sequence isn't perfect according to the recommendation from the datasheet, but it's likely less critical than
	* the power-up sequence.
	* If the power-down sequence is absolutely critical, you might need to implement custom power management logic beyond just 
	* using the regulator framework's default behavior.
	* The Linux regulator framework will disable them in reverse order of enabling as per tas6754_supply_names[] definition */
    ret = regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
    if (ret < 0) {
        dev_err(component->dev, "failed to disable supplies: %d\n", ret);
        return ret;
    }

    dev_dbg(component->dev, "%s: TAS6754 powered off\n", __func__);
    return 0;
}


/**
 * TODO:
 * @ref: TRM[4.3.1.1.1.1 Quick-Start Sequence, p-24]
 * In some cases a quick startup time from shutdown to audio playback is needed. For the quickest startup the DC 
 * Load Diagnostics can be aborted. This allows the device to go into PLAY state without having to wait for DC 
 * Load Diagnostics to finish.
 * The procedure implementation for a quick-start sequence is PENDING. In future, evaluate the possibility of adding this feature.
 */
/**
 * tas6754_power_on - Power on the TAS6754 audio amplifier
 * @brief: Implements the power-on sequence for the TAS6754 Class-D
 * amplifier according to the datasheet specifications. The sequence includes:
 *
 * 1. Keeping PD and STBY pins low initially
 * 2. Enabling power supplies (PVDD, VBAT, DVDD)
 * 3. Releasing PD pin to power up digital circuitry
 * 4. Waiting 4ms as specified in the datasheet
 * 5. Enabling register access and synchronizing register cache
 * 6. Checking and clearing any power-on-reset (POR) faults
 * 7. Releasing STBY pin to power up analog circuitry
 * 8. Setting initial channel states (PLAY with appropriate mute settings)
 * 9. Waiting for device initialization and auto-diagnostics to complete
 *
 * The function handles both GPIO-based control (using pd_gpio and stby_gpio)
 * and register-based control when GPIOs are not available. It also properly
 * configures the initial state of the amplifier channels based on the
 * availability of an external mute circuit.
 *
 * After successful execution, the amplifier will be in a powered-on state
 * but typically still muted, ready for audio playback to begin.
 *
 * @return: Return 0 on success, negative error code on failure
 */
static int tas6754_power_on(struct snd_soc_component *component)
{
    struct tas6754_data *tas6754 = snd_soc_component_get_drvdata(component);
    int ret;
    u8 chan_states;
    int no_auto_diags = 0;
    unsigned int reg_val;

    dev_dbg(component->dev, "%s: Powering on TAS6754\n", __func__);

    /* 1. Keep PD and STBY pins low initially */
    if (tas6754->pd_gpio)
        gpiod_set_value_cansleep(tas6754->pd_gpio, 0);
    
    if (tas6754->stby_gpio)
        gpiod_set_value_cansleep(tas6754->stby_gpio, 0);

    /* 2. Check for DC load diagnostics bypass setting */
    if (!regmap_read(tas6754->regmap, TAS6754_DC_LDG_CTRL, &reg_val))
        no_auto_diags = reg_val & TAS6754_DC_LDG_BYPASS_MASK;

    /* 3. Enable power supplies */
    ret = regulator_bulk_enable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
    if (ret < 0) {
        dev_err(component->dev, "failed to enable supplies: %d\n", ret);
        return ret;
    }

    /* 4. Wait for supplies to stabilize */
    msleep(1);

    /* 5. Release PD pin to power up digital circuitry */
    if (tas6754->pd_gpio)
        gpiod_set_value_cansleep(tas6754->pd_gpio, 1);

    /* 6. Wait minimum 4ms before releasing STBY pin */
    msleep(4);

    /* 7. Enable register access */
    regcache_cache_only(tas6754->regmap, false);
    ret = regcache_sync(tas6754->regmap);
    if (ret < 0) {
        dev_err(component->dev, "failed to sync regcache: %d\n", ret);
        goto err_disable_supplies;
    }

    /* 8. Check and clear any power faults */
    ret = regmap_read(tas6754->regmap, TAS6754_POWER_FAULT_LATCHED, &reg_val);
    if (ret < 0) {
        dev_err(component->dev, "Failed to read POWER_FAULT_LATCHED: %d\n", ret);
        goto err_disable_supplies;
    }

    if (reg_val & BIT(7)) {
        dev_dbg(component->dev, "POR fault detected, clearing\n");
        /* Fault is cleared by reading the register (already done above) */
    }

    /* 9. Release STBY pin to power up analog circuitry */
    if (tas6754->stby_gpio)
        gpiod_set_value_cansleep(tas6754->stby_gpio, 1);

    /* 10. Set initial channel states to either PLAY with normal volume or PLAY with mute depending on mute GPIO availability.*/
    if (tas6754->mute_gpio) {
        /* If using external mute circuit, set channels to PLAY state */
        /* but keep them muted via the GPIO */
        gpiod_set_value_cansleep(tas6754->mute_gpio, 1); /* Muted */
        
        /* Set channels to PLAY state with normal volume in registers */
        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_PLAY__NORMAL_VOLUME);
        if (ret < 0) {
            dev_err(component->dev, "Failed to set CH1/CH2 to PLAY state with NORMAL VOLUME: %d\n", ret);
            goto err_disable_supplies;
        }

        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_PLAY__NORMAL_VOLUME);
        if (ret < 0) {
            dev_err(component->dev, "Failed to set CH3/CH4 to PLAY state with NORMAL VOLUME: %d\n", ret);
            goto err_disable_supplies;
        }
    } else {
        /* If no external mute circuit, set channels to PLAY state but muted */
        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH1_CH2, TAS6754_STATE_CTRL_CH1_CH2_PLAY__MUTE);
        if (ret < 0) {
            dev_err(component->dev, "Failed to set CH1/CH2 to PLAY state but MUTE: %d\n", ret);
            goto err_disable_supplies;
        }
        ret = snd_soc_component_write(component, TAS6754_STATE_CTRL_CH3_CH4, TAS6754_STATE_CTRL_CH3_CH4_PLAY__MUTE);
        if (ret < 0) {
            dev_err(component->dev, "Failed to set CH3/CH4 to PLAY state but MUTE: %d\n", ret);
            goto err_disable_supplies;
        }
    }

    /* 11. Wait for device to fully power up */
    msleep(6);

    /* 12. Wait for auto-diagnostics if enabled */
    if (!no_auto_diags)
        msleep(230);

    /** 
	* @TODO:
	* 13. Unmute if using external mute circuit and ready to play audio */
    /* Note: You might want to move this to a separate unmute function */
    /*
    if (tas6754->mute_gpio)
        gpiod_set_value_cansleep(tas6754->mute_gpio, 0);
    */

    dev_dbg(component->dev, "%s: TAS6754 powered on\n", __func__);
    return 0;

err_disable_supplies:
    regulator_bulk_disable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
    return ret;
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
	.dapm_widgets		= tas6754_dapm_widgets,//ok
	.num_dapm_widgets	= ARRAY_SIZE(tas6754_dapm_widgets),
	.dapm_routes		= tas6754_audio_map,//ok
	.num_dapm_routes	= ARRAY_SIZE(tas6754_audio_map),
	.use_pmdown_time	= 1,
	.endianness		= 1,
};

static const struct snd_soc_dai_ops tas6754_speaker_dai_ops = {
	.hw_params	= tas6754_hw_params,//ok
	.set_fmt	= tas6754_set_dai_fmt,//ok
	.set_tdm_slot	= tas6754_set_dai_tdm_slot,//ok
	.mute_stream	= tas6754_mute,//ok
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

/**
 * tas6754_i2c_probe - Probe and initialize the TAS6754 audio amplifier
 *
 * @brief: Initializes the TAS6754 Class-D audio amplifier when the device
 * is detected on the I2C bus. It performs the following operations:
 *
 * 1. Allocates and initializes the driver's private data structure
 * 2. Sets up the regmap for register access
 * 3. Initializes audio interface settings with sensible defaults
 * 4. Configures GPIO pins for PD (Power Down), STBY (Standby), and optional mute control
 * 5. Sets up power supplies according to the recommended sequence:
 *    - Keeps PD and STBY pins low initially
 *    - Enables power supplies (PVDD, VBAT, DVDD)
 *    - Releases PD pin with appropriate timing
 *    - Releases STBY pin with appropriate timing
 * 6. Checks for and clears any Power-On-Reset (POR) faults
 * 7. Performs a device reset to establish a well-defined startup state
 * 8. Initializes DC Load Diagnostics settings
 * 9. Sets default volume levels for all channels
 * 10. Registers the codec with the ALSA SoC framework
 * 11. Sets up periodic fault checking
 *
 * The function follows the power-up sequence specified in the TAS6754 datasheet
 * to ensure proper device initialization. It includes appropriate error handling
 * and cleanup in case any step fails.
 * @client: I2C client for the device
 * 
 * @return: Return 0 on success, negative error code on failure
 */
static int tas6754_i2c_probe(struct i2c_client *client)
{
    struct device *dev = &client->dev;
    struct tas6754_data *tas6754;
    unsigned int reg_val;
    int ret;
    int i;

    /* Allocate driver data */
    tas6754 = devm_kzalloc(dev, sizeof(*tas6754), GFP_KERNEL);
    if (!tas6754)
        return -ENOMEM;
    
    dev_set_drvdata(dev, tas6754);
    tas6754->dev = dev;

    /* Initialize mutex for thread safety */
    mutex_init(&tas6754->mutex);

    /* Initialize state tracking variables */
    tas6754->powered = false;
    tas6754->cache_sync = false;
    tas6754->playback_active = false;
    tas6754->muted = true;  /* Start muted for safety */
    
    /* Initialize volume settings to default values */
    for (i = 0; i < 4; i++)
        tas6754->volume[i] = 0x30;  /* 0dB - default from datasheet */
    
    tas6754->dc_load_diag_config = TAS6754_DC_LDG_CTRL_DEFAULT;

    /* Initialize regmap */
    tas6754->regmap = devm_regmap_init_i2c(client, &tas6754_regmap_config);
    if (IS_ERR(tas6754->regmap)) {
        ret = PTR_ERR(tas6754->regmap);
        dev_err(dev, "unable to allocate register map: %d\n", ret);
        return ret;
    }

    /* Initialize audio interface settings */
    tas6754->tdm_mode = false;
    tas6754->dsp_a_mode = false;
    tas6754->short_fsync = false;
    tas6754->use_sdin2_for_ch34 = false;
    tas6754->ll_enabled = false;
    tas6754->support_rate_change = false;
    tas6754->bit_depth = 24;  				/* Default bit depth */
    tas6754->sample_rate = 0;
    tas6754->channels = 0;
    tas6754->dai_fmt = 0;
    tas6754->channel_offset = 0;
    tas6754->ll_offset = 96;  				/* Default LL offset is 96 SCLKs */
    tas6754->sdin2_gpio_num = 1; 			/* Default to GPIO1 for SDIN2 */
    tas6754->tdm_slots = 4;   				/* Default to 4 TDM slots */
    tas6754->tdm_slot_width = 24; 			/* Default to 24-bit slots */
    tas6754->audio_swap = 0;  				/* Default audio channel mapping */
    tas6754->ll_swap = 0;    				/* Default LL channel mapping */
    tas6754->detected_sclk_ratio = 0;  		/* Will be detected during operation */

    /* Initialize fault tracking variables */
    tas6754->last_oc_dc_fault = 0;
    tas6754->last_power_fault = 0;
    tas6754->last_ot_fault = 0;
    tas6754->last_cbc_fault_warn = 0;
    tas6754->last_rtldg_ol_sl_fault = 0;

    /* Get control of the PD pin and set it LOW initially (active) */
    tas6754->pd_gpio = devm_gpiod_get_optional(dev, "pd", GPIOD_OUT_LOW);
    if (IS_ERR(tas6754->pd_gpio)) {
        if (PTR_ERR(tas6754->pd_gpio) == -EPROBE_DEFER)
            return -EPROBE_DEFER;
        dev_info(dev, "failed to get PD GPIO: %ld\n", PTR_ERR(tas6754->pd_gpio));
        tas6754->pd_gpio = NULL;
    }

    /* Get control of the STBY pin and set it LOW initially (active) */
    tas6754->stby_gpio = devm_gpiod_get_optional(dev, "standby", GPIOD_OUT_LOW);
    if (IS_ERR(tas6754->stby_gpio)) {
        if (PTR_ERR(tas6754->stby_gpio) == -EPROBE_DEFER)
            return -EPROBE_DEFER;
        dev_info(dev, "failed to get STBY GPIO: %ld\n", PTR_ERR(tas6754->stby_gpio));
        tas6754->stby_gpio = NULL;
    }

    /* Get optional mute GPIO */
    tas6754->mute_gpio = devm_gpiod_get_optional(dev, "mute", GPIOD_OUT_HIGH);
    if (IS_ERR(tas6754->mute_gpio)) {
        if (PTR_ERR(tas6754->mute_gpio) == -EPROBE_DEFER)
            return -EPROBE_DEFER;
        dev_info(dev, "No external mute GPIO, using register control for muting\n");
        tas6754->mute_gpio = NULL;
    } else if (tas6754->mute_gpio) {
        dev_info(dev, "Using external mute GPIO\n");
        /* Start with mute active (HIGH) */
        gpiod_set_value_cansleep(tas6754->mute_gpio, 1);
    }

    /* Initialize regulators */
    for (i = 0; i < ARRAY_SIZE(tas6754->supplies); i++)
        tas6754->supplies[i].supply = tas6754_supply_names[i];
    
    ret = devm_regulator_bulk_get(dev, ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
    if (ret) {
        dev_err(dev, "unable to request supplies: %d\n", ret);
        return ret;
    }

    /* Power up the device following the proper sequence */
    
    /* 1. Keep PD and STBY pins low initially */
    if (tas6754->pd_gpio)
        gpiod_set_value_cansleep(tas6754->pd_gpio, 0);
    if (tas6754->stby_gpio)
        gpiod_set_value_cansleep(tas6754->stby_gpio, 0);

    /* 2. Enable regulators */
    ret = regulator_bulk_enable(ARRAY_SIZE(tas6754->supplies), tas6754->supplies);
    if (ret) {
        dev_err(dev, "unable to enable supplies: %d\n", ret);
        return ret;
    }

    /* 3. Wait for supplies to stabilize */
    msleep(1);

    /* 4. Release PD pin to power up digital circuitry */
    if (tas6754->pd_gpio)
        gpiod_set_value_cansleep(tas6754->pd_gpio, 1);

    /* 5. Wait minimum 4ms before releasing STBY pin */
    msleep(4);

    /* 6. Release STBY pin to power up analog circuitry */
    if (tas6754->stby_gpio)
        gpiod_set_value_cansleep(tas6754->stby_gpio, 1);

    /* 7. Wait for device to fully power up */
    msleep(6);
    
    /* Update power state */
    tas6754->powered = true;

    /* Check for POR condition and clear it */
    ret = regmap_read(tas6754->regmap, TAS6754_POWER_FAULT_LATCHED, &reg_val);
    if (ret) {
        dev_err(dev, "failed to read power fault register: %d\n", ret);
        goto disable_regs;
    }

    if (reg_val & BIT(7)) {
        dev_dbg(dev, "Device went through POR cycle\n");
        /* Fault is cleared by reading the register (already done above) */
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

    /* Wait for reset to complete */
    msleep(1);

    /* Initialize DC Load Diagnostics */
    ret = regmap_write(tas6754->regmap, TAS6754_DC_LDG_CTRL, tas6754->dc_load_diag_config);
    if (ret) {
        dev_err(dev, "failed to initialize DC load diagnostics control: %d\n", ret);
        goto disable_regs;
    }
    
    ret = regmap_write(tas6754->regmap, TAS6754_DC_LDG_LO_CTRL, TAS6754_DC_LDG_LO_CTRL_DEFAULT);
    if (ret) {
        dev_err(dev, "failed to initialize DC load diagnostics low control: %d\n", ret);
        goto disable_regs;
    }

    /* Initialize volumes to default moderate levels */
    for (i = 0; i < 4; i++) {
        ret = regmap_write(tas6754->regmap, TAS6754_DIG_VOL_CH1 + i, tas6754->volume[i]);
        if (ret) {
            dev_err(dev, "failed to set CH%d volume: %d\n", i+1, ret);
            goto disable_regs;
        }
    }

    /* Initialize fault check work */
    INIT_DELAYED_WORK(&tas6754->fault_check_work, tas6754_fault_check_work);

    /* Register codec with ALSA */
    ret = devm_snd_soc_register_component(dev, &soc_codec_dev_tas6754, 
                                         tas6754_dai, ARRAY_SIZE(tas6754_dai));
    if (ret < 0) {
        dev_err(dev, "unable to register codec: %d\n", ret);
        goto disable_regs;
    }

    /* Schedule initial fault check */
    schedule_delayed_work(&tas6754->fault_check_work, 
                         msecs_to_jiffies(TAS6754_FAULT_CHECK_INTERVAL_MS));

    dev_info(dev, "TAS6754 initialized successfully\n");
    return 0;

disable_regs:
    /* Power down the device */
    tas6754->powered = false;
    
    if (tas6754->stby_gpio)
        gpiod_set_value_cansleep(tas6754->stby_gpio, 0);
    
    msleep(10);
    
    if (tas6754->pd_gpio)
        gpiod_set_value_cansleep(tas6754->pd_gpio, 0);
    
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