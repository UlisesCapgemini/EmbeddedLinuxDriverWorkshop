/* SPDX-License-Identifier: GPL-2.0 */
/*
 * ALSA SoC Texas Instruments TAS6754 Quad-Channel Audio Amplifier
 *
 * Copyright (C) 2023 Your Name <your.email@example.com>
 * Based on TAS6424 driver by Andreas Dannenberg <dannenberg@ti.com>
 * and Andrew F. Davis <afd@ti.com>
 */

#ifndef __TAS6754_H__
#define __TAS6754_H__

#define TAS6754_RATES (SNDRV_PCM_RATE_44100 | \
					   SNDRV_PCM_RATE_48000 | \
					   SNDRV_PCM_RATE_96000 | \
					   SNDRV_PCM_RATE_192000)

#define TAS6754_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \
			 			 SNDRV_PCM_FMTBIT_S24_LE)


/* Register Address Map */
#define TAS6754_RESET             	    	(0x01)//Reset Control
#define TAS6754_OUTPUT_CTRL              	(0x02)//Output Configuration Control
#define TAS6754_STATE_CTRL_CH1_CH2       	(0x03)//State Control Channel 1 & Channel 2
#define TAS6754_STATE_CTRL_CH3_CH4       	(0x04)//State Control Channel 3 & Channel 4
#define TAS6754_ISENSE_CTRL              	(0x05)//Current Sense Control
#define TAS6754_DC_DETECT_CTRL           	(0x06)//DC Detection Control
#define TAS6754_SCLK_INV_CTRL            	(0x20)//SCLK Polarity Control
#define TAS6754_AUDIO_INTERFACE_CTRL     	(0x21)//Audio Interface Control
#define TAS6754_SDIN_CTRL                	(0x23)//SDIN Control
#define TAS6754_SDOUT_CTRL               	(0x25)//SDOUT Control
#define TAS6754_SDIN_OFFSET_MSB          	(0x27)//SDIN Offset MSB
#define TAS6754_SDIN_AUDIO_OFFSET        	(0x28)//SDIN Audio Path Offset
#define TAS6754_SDIN_LL_OFFSET           	(0x29)//SDIN Low Latency Path Offset
#define TAS6754_SDIN_CH_SWAP             	(0x2A)//SDIN Channel Swap Control
#define TAS6754_SDOUT_OFFSET_MSB         	(0x2C)//SDOUT Offset MSB

#define TAS6754_VPREDICT_OFFSET          	(0x2D)//Vpredict SDOUT Offset			ToDo	
#define TAS6754_ISENSE_OFFSET            	(0x2E)//Current Sense SDOUT Offset	ToDo
#define TAS6754_SDOUT_EN                 	(0x31)//SDOUT Enable
#define TAS6754_LL_EN                    	(0x32)//Low Latency Path Enable
#define TAS6754_RTLDG_EN                 	(0x37)//Real-time Load Diagnostic Open Load/Shorted Load Enable
#define TAS6754_DC_BLOCK_BYP             	(0x39)//DC Blocking Bypass
#define TAS6754_DSP_CTRL                 	(0x3A)//DSP Control
#define TAS6754_PAGE_AUTO_INC            	(0x3B)//Page Auto Increment
#define TAS6754_DIG_VOL_CH1              	(0x40)//Digital Volume Channel 1
#define TAS6754_DIG_VOL_CH2              	(0x41)//Digital Volume Channel 2
#define TAS6754_DIG_VOL_CH3              	(0x42)//Digital Volume Channel 3
#define TAS6754_DIG_VOL_CH4              	(0x43)//Digital Volume Channel 4
#define TAS6754_DIG_VOL_RAMP_CTRL        	(0x44)//Digital Volume Ramp Control
#define TAS6754_DIG_VOL_COMBINE_CTRL     	(0x46)//Digital Volume Combination Control
#define TAS6754_AUTO_MUTE_EN             	(0x47)//Auto Mute Enable
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2 	(0x48)//Auto Mute Time Channel 1 & Channel 2
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4 	(0x49)//Auto Mute Time Channel 3 & Channel 4
#define TAS6754_ANALOG_GAIN_CH1_CH2      	(0x4A)//Analog Gain Channel 1 & Channel 2
#define TAS6754_ANALOG_GAIN_CH3_CH4      	(0x4B)//Analog Gain Channel 3 & Channel 4
#define TAS6754_ANALOG_GAIN_RAMP_CTRL    	(0x4E)//Analog Gain Ramp Control
#define TAS6754_PULSE_INJECTION_EN       	(0x52)//Pulse Injection Enable
#define TAS6754_CBC_CTRL                 	(0x54)//CBC Control
#define TAS6754_CURRENT_LIMIT_CTRL       	(0x55)//Current Limit Control
#define TAS6754_ISENSE_CAL               	(0x5B)//Current Sense Calibration

#define TAS6754_PWM_PHASE_CTRL           	(0x60)//PWM Phase Control
#define TAS6754_SS_CTRL                  	(0x61)//Spread Spectrum Control
#define TAS6754_SS_RANGE_CTRL            	(0x62)//Spread Spectrum Range Control
#define TAS6754_SS_DWELL_CTRL            	(0x66)//Spread Spectrum DWELL Control
#define TAS6754_RAMP_PHASE_CTRL_GPO      	(0x68)//Switching Clock Phase Control for GPO
#define TAS6754_PWM_PHASE_M_CTRL_CH1     	(0x69)//PWM Phase Manual Control Channel 1
#define TAS6754_PWM_PHASE_M_CTRL_CH2     	(0x6A)//PWM Phase Manual Control Channel 2
#define TAS6754_PWM_PHASE_M_CTRL_CH3     	(0x6B)//PWM Phase Manual Control Channel 3
#define TAS6754_PWM_PHASE_M_CTRL_CH4     	(0x6C)//PWM Phase Manual Control Channel 4
#define TAS6754_AUTO_MUTE_STATUS         	(0x71)//Auto Mute Status
#define TAS6754_STATE_REPORT_CH1_CH2     	(0x72)//Status Channel 1 & Channel 2
#define TAS6754_STATE_REPORT_CH3_CH4     	(0x73)//Status Channel 3 & Channel 4
#define TAS6754_PVDD_SENSE               	(0x74)//PVDD Voltage Sense
#define TAS6754_TEMP_GLOBAL              	(0x75)//Global Temperature Readout
#define TAS6754_FS_MON                   	(0x76)//FS Monitor
#define TAS6754_SCLK_MON                 	(0x77)//SCLK Monitor
#define TAS6754_REPORT_ROUTING_1         	(0x7C)//Enable Faults and Warnings to GPIO
#define TAS6754_POWER_FAULT_STATUS_1     	(0x7D)//Power Fault Status 1
#define TAS6754_POWER_FAULT_STATUS_2     	(0x80)//Power Fault Status 2
#define TAS6754_OT_FAULT                 	(0x81)//Temperature (OTSD) and Fault Status
#define TAS6754_OTW_STATUS               	(0x82)//Temperature (OTW) Warning Status
#define TAS6754_CLIP_WARN_STATUS         	(0x83)//Channel Clip Detect Status
#define TAS6754_CBC_WARNING_STATUS       	(0x85)//CBC Warning Report
#define TAS6754_POWER_FAULT_LATCHED      	(0x86)//Power Fault Latched
#define TAS6754_OTSD_LATCHED             	(0x87)//Temperature (OTSD) Fault Latched
#define TAS6754_OTW_LATCHED              	(0x88)//Temperature (OTW) Warning Latched
#define TAS6754_CLIP_WARN_LATCHED        	(0x89)//Channel Clip Detect Warning Latched
#define TAS6754_CLK_FAULT_LATCHED        	(0x8A)//Clock Error Latched
#define TAS6754_RTLDG_OL_SL_FAULT_LATCHED	(0x8B)//Real-Time Load Diagnostic OL/SL Latched
#define TAS6754_CBC_FAULT_WARN_LATCHED   	(0x8D)//Channel Load Current Fault Latched
#define TAS6754_OC_DC_FAULT_LATCHED      	(0x8E)//Channel Over Current and DC Detection Fault Latched
#define TAS6754_OTSD_RECOVERY_EN         	(0x8F)//Overtemperature Shutdown Auto-recovery Enable
#define TAS6754_REPORT_ROUTING_2         	(0x90)//Enable Faults to GPIO
#define TAS6754_REPORT_ROUTING_3         	(0x91)//Enable Warnings to GPIO
#define TAS6754_REPORT_ROUTING_4         	(0x92)//Enable Faults and Warnings to GPIO
#define TAS6754_CLIP_DETECT_CTRL         	(0x93)//Clip Detect Control
#define TAS6754_REPORT_ROUTING_5         	(0x94)//Enable Faults and Warnings Reported to GPIO
#define TAS6754_GPIO1_OUTPUT_SELECT      	(0x95)//Select Signals to GPIOs
#define TAS6754_GPIO2_OUTPUT_SELECT      	(0x96)//Select Signals to GPIOs
#define TAS6754_GPIO_INPUT_SLEEP_HIZ     	(0x9B)//Select Signals from GPIOs
#define TAS6754_GPIO_INPUT_PLAY_SLEEP    	(0x9C)//Select Signals from GPIOs
#define TAS6754_GPIO_INPUT_MUTE          	(0x9D)//Select Signals from GPIOs
#define TAS6754_GPIO_INPUT_SYNC          	(0x9E)//Select Signals from GPIOs
#define TAS6754_GPIO_INPUT_SDIN2         	(0x9F)//Select Signals from GPIOs
#define TAS6754_GPIO_CTRL                	(0xA0)//General GPIO Control
#define TAS6754_GPIO_INVERT              	(0xA1)//Invert GPIO Signals
#define TAS6754_DC_LDG_CTRL              	(0xB0)//DC Load Diagnostics Control
#define TAS6754_DC_LDG_LO_CTRL           	(0xB1)//DC Load Diagnostic Line-out Control
#define TAS6754_DC_LDG_TIME_CTRL         	(0xB2)//DC Load Diagnostic Timing Control
#define TAS6754_DC_LDG_SL_CH1_CH2_CTRL   	(0xB3)//DC Load Diagnostic Shorted-load Threshold Channel 1 & Channel 2
#define TAS6754_DC_LDG_SL_CH3_CH4_CTRL   	(0xB4)//DC Load Diagnostic Shorted-load Threshold Channel 3 & Channel 4
#define TAS6754_AC_LDG_CTRL              	(0xB5)//AC Load Diagnostic Control
#define TAS6754_TWEETER_DETECT_CTRL      	(0xB6)//Tweeter Detection Control
#define TAS6754_TWEETER_DETECT_THRESH    	(0xB7)//Tweeter Detection Threshold
#define TAS6754_AC_LDG_FREQ_CTRL         	(0xB8)//AC Load Diagnostic Frequency Control
#define TAS6754_TEMP_CH1_CH2             	(0xBB)//Temperature Range Channel 1 & Channel 2
#define TAS6754_TEMP_CH3_CH4             	(0xBC)//Temperature Range Channel 3 & Channel 4
#define TAS6754_WARN_OT_MAX_FLAG         	(0xBD)//OT Warning MAX Level Report
#define TAS6754_DC_LDG_REPORT_CH1_CH2    	(0xC0)//DC Load Diagnostic Report Channel 1 & Channel 2
#define TAS6754_DC_LDG_REPORT_CH3_CH4    	(0xC1)//DC Load Diagnostic Report Channel 3 & Channel 4
#define TAS6754_DC_LDG_RESULT            	(0xC2)//DC Load Diagnostic Result Report
#define TAS6754_AC_LDG_REPORT_CH1_R      	(0xC3)//AC Load Diagnostic Report Real Channel 1
#define TAS6754_AC_LDG_REPORT_CH1_I      	(0xC4)//AC Load Diagnostic Report Imaginary Channel 1
#define TAS6754_AC_LDG_REPORT_CH2_R      	(0xC5)//AC Load Diagnostic Report Real Channel 2
#define TAS6754_AC_LDG_REPORT_CH2_I      	(0xC6)//AC Load Diagnostic Report Imaginary Channel 2
#define TAS6754_AC_LDG_REPORT_CH3_R      	(0xC7)//AC Load Diagnostic Report Real Channel 3
#define TAS6754_AC_LDG_REPORT_CH3_I      	(0xC8)//AC Load Diagnostic Report Imaginary Channel 3
#define TAS6754_AC_LDG_REPORT_CH4_R      	(0xC9)//AC Load Diagnostic Report Real Channel 4
#define TAS6754_AC_LDG_REPORT_CH4_I      	(0xCA)//AC Load Diagnostic Report Imaginary Channel 4
#define TAS6754_TWEETER_REPORT           	(0xCB)//Tweeter Detection Report
#define TAS6754_CH1_RTLDG_IMP_MSB        	(0xD1)//Real-time Load Diagnostic Channel 1 Impedance MSB
#define TAS6754_CH1_RTLDG_IMP_LSB        	(0xD2)//Real-time Load Diagnostic Channel 1 Impedance LSB
#define TAS6754_CH2_RTLDG_IMP_MSB        	(0xD3)//Real-time Load Diagnostic Channel 2 Impedance MSB
#define TAS6754_CH2_RTLDG_IMP_LSB        	(0xD4)//Real-time Load Diagnostic Channel 2 Impedance LSB
#define TAS6754_CH3_RTLDG_IMP_MSB        	(0xD5)//Real-time Load Diagnostic Channel 3 Impedance MSB
#define TAS6754_CH3_RTLDG_IMP_LSB        	(0xD6)//Real-time Load Diagnostic Channel 3 Impedance LSB
#define TAS6754_CH4_RTLDG_IMP_MSB        	(0xD7)//Real-time Load Diagnostic Channel 4 Impedance MSB
#define TAS6754_CH4_RTLDG_IMP_LSB        	(0xD8)//Real-time Load Diagnostic Channel 4 Impedance LSB
#define TAS6754_DC_LDG_DCR_MSB           	(0xD9)//DC Diagnostic DC Resistance Measurement MSB
#define TAS6754_CH1_DC_LDG_DCR_LSB       	(0xDA)//DC Diagnostic Channel 1 DC Resistance Measurement LSB
#define TAS6754_CH2_DC_LDG_DCR_LSB       	(0xDB)//DC Diagnostic Channel 2 DC Resistance Measurement LSB
#define TAS6754_CH3_DC_LDG_DCR_LSB       	(0xDC)//DC Diagnostic Channel 3 DC Resistance Measurement LSB
#define TAS6754_CH4_DC_LDG_DCR_LSB       	(0xDD)//DC Diagnostic Channel 4 DC Resistance Measurement LSB
#define TAS6754_OTW_CTRL_CH1_CH2         	(0xE2)//Overtemperature Warning (OTW) Level Report Control Channel 1 & Channel 2
#define TAS6754_OTW_CTRL_CH3_CH4         	(0xE3)//Overtemperature Warning (OTW) Level Report Control Channel 3 & Channel 4
#define TAS6754_MAX							(TAS6754_OTW_CTRL_CH3_CH4)

/************************************************************************
 *					TAS6754_RESET (0x01)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_RESET_RESERVED_7_5_MASK             GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_RESET_DEVICE_RESET_MASK             BIT(4)          /* Bit 4: DEVICE RESET */
#define TAS6754_RESET_CLEAR_FAULT_MASK              BIT(3)          /* Bit 3: CLEAR FAULT */
#define TAS6754_RESET_RESERVED_2_MASK               BIT(2)          /* Bit 2: Reserved */
#define TAS6754_RESET_RESERVED_1_MASK               BIT(1)          /* Bit 1: Reserved */
#define TAS6754_RESET_REGISTER_RESET_MASK           BIT(0)          /* Bit 0: REGISTER RESET */

/* DEVICE RESET values (bit 4) */
#define TAS6754_RESET_DEVICE_NORMAL                 (0x00 << 4)  /* 0: Normal operation */
#define TAS6754_RESET_DEVICE_RESET                  (0x01 << 4)  /* 1: Device will be reset */

/* CLEAR FAULT values (bit 3) */
#define TAS6754_RESET_CLEAR_FAULT_NORMAL            (0x00 << 3)  /* 0: Normal operation */
#define TAS6754_RESET_CLEAR_FAULT_CLEAR             (0x01 << 3)  /* 1: Clear analog fault */

/* REGISTER RESET values (bit 0) */
#define TAS6754_RESET_REGISTER_NORMAL               (0x00 << 0)  /* 0: Normal operation */
#define TAS6754_RESET_REGISTER_RESET                (0x01 << 0)  /* 1: Reset registers */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_RESET_RESERVED_MASK                 (TAS6754_RESET_RESERVED_7_5_MASK | \
                                                    TAS6754_RESET_RESERVED_2_MASK | \
                                                    TAS6754_RESET_RESERVED_1_MASK)

/* Reset value */
#define TAS6754_RESET_REG_RESET                     0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_OUTPUT_CTRL (0x02)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_OUTPUT_CTRL_CH1_LO_MODE_MASK        BIT(7)          /* Bit 7: CH1 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH2_LO_MODE_MASK        BIT(6)          /* Bit 6: CH2 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH3_LO_MODE_MASK        BIT(5)          /* Bit 5: CH3 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH4_LO_MODE_MASK        BIT(4)          /* Bit 4: CH4 LO MODE */
#define TAS6754_OUTPUT_CTRL_RESERVED_3_MASK         BIT(3)          /* Bit 3: Reserved */
#define TAS6754_OUTPUT_CTRL_RESERVED_2_MASK         BIT(2)          /* Bit 2: Reserved */
#define TAS6754_OUTPUT_CTRL_RESERVED_1_0_MASK       GENMASK(1, 0)   /* Bits 1-0: Reserved */

/* CH1 LO MODE values (bit 7) */
#define TAS6754_OUTPUT_CTRL_CH1_SPEAKER_MODE        (0x00 << 7)  /* 0: Channel 1 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH1_LINE_OUT_MODE       (0x01 << 7)  /* 1: Channel 1 is in line output mode */

/* CH2 LO MODE values (bit 6) */
#define TAS6754_OUTPUT_CTRL_CH2_SPEAKER_MODE        (0x00 << 6)  /* 0: Channel 2 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH2_LINE_OUT_MODE       (0x01 << 6)  /* 1: Channel 2 is in line output mode */

/* CH3 LO MODE values (bit 5) */
#define TAS6754_OUTPUT_CTRL_CH3_SPEAKER_MODE        (0x00 << 5)  /* 0: Channel 3 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH3_LINE_OUT_MODE       (0x01 << 5)  /* 1: Channel 3 is in line output mode */

/* CH4 LO MODE values (bit 4) */
#define TAS6754_OUTPUT_CTRL_CH4_SPEAKER_MODE        (0x00 << 4)  /* 0: Channel 4 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH4_LINE_OUT_MODE       (0x01 << 4)  /* 1: Channel 4 is in line output mode */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_OUTPUT_CTRL_RESERVED_MASK           (TAS6754_OUTPUT_CTRL_RESERVED_3_MASK | \
                                                    TAS6754_OUTPUT_CTRL_RESERVED_2_MASK | \
                                                    TAS6754_OUTPUT_CTRL_RESERVED_1_0_MASK)

/* Combined channel masks (for convenience) */
#define TAS6754_OUTPUT_CTRL_ALL_CHANNELS_MASK       (TAS6754_OUTPUT_CTRL_CH1_LO_MODE_MASK | \
                                                    TAS6754_OUTPUT_CTRL_CH2_LO_MODE_MASK | \
                                                    TAS6754_OUTPUT_CTRL_CH3_LO_MODE_MASK | \
                                                    TAS6754_OUTPUT_CTRL_CH4_LO_MODE_MASK)

/* Reset value */
#define TAS6754_OUTPUT_CTRL_RESET                   0x00            /* Reset value for the register */
/************************************************************************
 *				TAS6754_STATE_CTRL_CH1_CH2 (0x03)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_STATE_CTRL_CH1_MUTE_MASK            BIT(7)          /* Bit 7: CH1 MUTE */
#define TAS6754_STATE_CTRL_CH1_STATE_MASK           GENMASK(6, 4)   /* Bits 6-4: CH1 STATE CTRL */
#define TAS6754_STATE_CTRL_CH2_MUTE_MASK            BIT(3)          /* Bit 3: CH2 MUTE */
#define TAS6754_STATE_CTRL_CH2_STATE_MASK           GENMASK(2, 0)   /* Bits 2-0: CH2 STATE CTRL */

/* CH1 MUTE values (bit 7) */
#define TAS6754_STATE_CTRL_CH1_NORMAL_VOLUME        (0x00 << 7)  /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH1_MUTE                 (0x01 << 7)  /* 1: Mute */

/* CH1 STATE CTRL values (bits 6-4) */
#define TAS6754_STATE_CTRL_CH1_DEEP_SLEEP           (0x00 << 4)  /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH1_LOAD_DIAG            (0x01 << 4)  /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH1_SLEEP                (0x02 << 4)  /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH1_HIZ                  (0x03 << 4)  /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH1_PLAY                 (0x04 << 4)  /* 100: PLAY */

/* CH2 MUTE values (bit 3) */
#define TAS6754_STATE_CTRL_CH2_NORMAL_VOLUME        (0x00 << 3)  /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH2_MUTE                 (0x01 << 3)  /* 1: Mute */

/* CH2 STATE CTRL values (bits 2-0) */
#define TAS6754_STATE_CTRL_CH2_DEEP_SLEEP           (0x00 << 0)  /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH2_LOAD_DIAG            (0x01 << 0)  /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH2_SLEEP                (0x02 << 0)  /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH2_HIZ                  (0x03 << 0)  /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH2_PLAY                 (0x04 << 0)  /* 100: PLAY */

/* Reset value */
#define TAS6754_STATE_CTRL_CH1_CH2_RESET            0x22            /* Reset value for the register */
/************************************************************************
 *					TAS6754_STATE_CTRL_CH3_CH4 (0x04)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_STATE_CTRL_CH3_MUTE_MASK            BIT(7)          /* Bit 7: CH3 MUTE */
#define TAS6754_STATE_CTRL_CH3_STATE_MASK           GENMASK(6, 4)   /* Bits 6-4: CH3 STATE CTRL */
#define TAS6754_STATE_CTRL_CH4_MUTE_MASK            BIT(3)          /* Bit 3: CH4 MUTE */
#define TAS6754_STATE_CTRL_CH4_STATE_MASK           GENMASK(2, 0)   /* Bits 2-0: CH4 STATE CTRL */

/* CH3 MUTE values (bit 7) */
#define TAS6754_STATE_CTRL_CH3_NORMAL_VOLUME        (0x00 << 7)  /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH3_MUTE                 (0x01 << 7)  /* 1: Mute */

/* CH3 STATE CTRL values (bits 6-4) */
#define TAS6754_STATE_CTRL_CH3_DEEP_SLEEP           (0x00 << 4)  /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH3_LOAD_DIAG            (0x01 << 4)  /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH3_SLEEP                (0x02 << 4)  /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH3_HIZ                  (0x03 << 4)  /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH3_PLAY                 (0x04 << 4)  /* 100: PLAY */

/* CH4 MUTE values (bit 3) */
#define TAS6754_STATE_CTRL_CH4_NORMAL_VOLUME        (0x00 << 3)  /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH4_MUTE                 (0x01 << 3)  /* 1: Mute */

/* CH4 STATE CTRL values (bits 2-0) */
#define TAS6754_STATE_CTRL_CH4_DEEP_SLEEP           (0x00 << 0)  /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH4_LOAD_DIAG            (0x01 << 0)  /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH4_SLEEP                (0x02 << 0)  /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH4_HIZ                  (0x03 << 0)  /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH4_PLAY                 (0x04 << 0)  /* 100: PLAY */

/* Reset value */
#define TAS6754_STATE_CTRL_CH3_CH4_RESET            0x22            /* Reset value for the register */
/************************************************************************
 *					TAS6754_ISENSE_CTRL (0x05)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_ISENSE_CTRL_RESERVED_MASK           GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_ISENSE_CTRL_CH1_DISABLE_MASK        BIT(3)          /* Bit 3: CH1 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH2_DISABLE_MASK        BIT(2)          /* Bit 2: CH2 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH3_DISABLE_MASK        BIT(1)          /* Bit 1: CH3 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH4_DISABLE_MASK        BIT(0)          /* Bit 0: CH4 ISENSE DISABLE */

/* CH1 ISENSE DISABLE values (bit 3) - Note: register description has inverted logic */
#define TAS6754_ISENSE_CTRL_CH1_DISABLE             (0x00 << 3)  /* 0: Disable Current Sense Channel 1 */
#define TAS6754_ISENSE_CTRL_CH1_ENABLE              (0x01 << 3)  /* 1: Enable Current Sense Channel 1 */

/* CH2 ISENSE DISABLE values (bit 2) - Note: register description has inverted logic */
#define TAS6754_ISENSE_CTRL_CH2_DISABLE             (0x00 << 2)  /* 0: Disable Current Sense Channel 2 */
#define TAS6754_ISENSE_CTRL_CH2_ENABLE              (0x01 << 2)  /* 1: Enable Current Sense Channel 2 */

/* CH3 ISENSE DISABLE values (bit 1) - Note: register description has inverted logic */
#define TAS6754_ISENSE_CTRL_CH3_DISABLE             (0x00 << 1)  /* 0: Disable Current Sense Channel 3 */
#define TAS6754_ISENSE_CTRL_CH3_ENABLE              (0x01 << 1)  /* 1: Enable Current Sense Channel 3 */

/* CH4 ISENSE DISABLE values (bit 0) - Note: register description has inverted logic */
#define TAS6754_ISENSE_CTRL_CH4_DISABLE             (0x00 << 0)  /* 0: Disable Current Sense Channel 4 */
#define TAS6754_ISENSE_CTRL_CH4_ENABLE              (0x01 << 0)  /* 1: Enable Current Sense Channel 4 */

/* Combined channel mask (for convenience) */
#define TAS6754_ISENSE_CTRL_ALL_CH_MASK             (TAS6754_ISENSE_CTRL_CH1_DISABLE_MASK | \
                                                    TAS6754_ISENSE_CTRL_CH2_DISABLE_MASK | \
                                                    TAS6754_ISENSE_CTRL_CH3_DISABLE_MASK | \
                                                    TAS6754_ISENSE_CTRL_CH4_DISABLE_MASK)

/* Reset value */
#define TAS6754_ISENSE_CTRL_RESET                   0x0F            /* Reset value for the register */
/************************************************************************
 *					TAS6754_DC_DETECT_CTRL (0x06)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_DC_DETECT_CTRL_RESERVED_MASK        GENMASK(7, 1)   /* Bits 7-1: Reserved */
#define TAS6754_DC_DETECT_CTRL_DC_DETECT_MASK       BIT(0)          /* Bit 0: DC DETECT */

/* DC DETECT values (bit 0) - Note: register description has inverted logic */
#define TAS6754_DC_DETECT_CTRL_ENABLE               (0x00 << 0)  /* 0: Enable DC Detection */
#define TAS6754_DC_DETECT_CTRL_DISABLE              (0x01 << 0)  /* 1: Disable DC Detection */

/* Reset value */
#define TAS6754_DC_DETECT_CTRL_RESET                0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SCLK_INV_CTRL (0x20)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SCLK_INV_CTRL_RESERVED_7_MASK       BIT(7)          /* Bit 7: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_6_MASK       BIT(6)          /* Bit 6: Reserved */
#define TAS6754_SCLK_INV_CTRL_SCLK_INV_TX_MASK      BIT(5)          /* Bit 5: SCLK INV TX */
#define TAS6754_SCLK_INV_CTRL_SCLK_INV_MASK         BIT(4)          /* Bit 4: SCLK INV */
#define TAS6754_SCLK_INV_CTRL_RESERVED_3_2_MASK     GENMASK(3, 2)   /* Bits 3-2: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_1_MASK       BIT(1)          /* Bit 1: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_0_MASK       BIT(0)          /* Bit 0: Reserved */

/* SCLK INV TX values (bit 5) */
#define TAS6754_SCLK_INV_CTRL_TX_NORMAL             (0x00 << 5)  /* 0: Normal SCLK mode */
#define TAS6754_SCLK_INV_CTRL_TX_INVERTED           (0x01 << 5)  /* 1: Inverted SCLK mode */

/* SCLK INV values (bit 4) */
#define TAS6754_SCLK_INV_CTRL_NORMAL                (0x00 << 4)  /* 0: Normal SCLK mode */
#define TAS6754_SCLK_INV_CTRL_INVERTED              (0x01 << 4)  /* 1: Inverted SCLK mode */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_SCLK_INV_CTRL_RESERVED_MASK         (TAS6754_SCLK_INV_CTRL_RESERVED_7_MASK | \
                                                    TAS6754_SCLK_INV_CTRL_RESERVED_6_MASK | \
                                                    TAS6754_SCLK_INV_CTRL_RESERVED_3_2_MASK | \
                                                    TAS6754_SCLK_INV_CTRL_RESERVED_1_MASK | \
                                                    TAS6754_SCLK_INV_CTRL_RESERVED_0_MASK)

/* Reset value */
#define TAS6754_SCLK_INV_CTRL_RESET                 0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_AUDIO_INTERFACE_CTRL (0x21)
 ***********************************************************************/
/* Bit field masks */
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_MASK  BIT(7)          /* Bit 7: LAST SAMPLE HOLD */
#define TAS6754_AUDIO_INTERFACE_RESERVED_6_MASK     BIT(6)          /* Bit 6: Reserved */
#define TAS6754_AUDIO_INTERFACE_RESERVED_5_MASK     BIT(5)          /* Bit 5: Reserved */
#define TAS6754_AUDIO_INTERFACE_TDM_MASK            BIT(4)          /* Bit 4: TDM */
#define TAS6754_AUDIO_INTERFACE_ASI_FORMAT_MASK     GENMASK(3, 2)   /* Bits 3-2: ASI FORMAT */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_MASK GENMASK(1, 0)   /* Bits 1-0: FS PULSE WIDTH */

/* LAST SAMPLE HOLD values (bit 7) - Note: register description has inverted logic */
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_ENABLE  (0x00 << 7)  /* 0: Enable last sample hold */
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_DISABLE (0x01 << 7)  /* 1: Disable last sample hold */

/* TDM values (bit 4) */
#define TAS6754_AUDIO_INTERFACE_DSP_MODE            (0x00 << 4)  /* 0: DSP mode */
#define TAS6754_AUDIO_INTERFACE_TDM_MODE            (0x01 << 4)  /* 1: TDM mode */

/* ASI FORMAT values (bits 3-2) */
#define TAS6754_AUDIO_INTERFACE_FORMAT_I2S          (0x00 << 2)  /* 00: I2S */
#define TAS6754_AUDIO_INTERFACE_FORMAT_TDM_DSP      (0x01 << 2)  /* 01: TDM/DSP */
#define TAS6754_AUDIO_INTERFACE_FORMAT_RTJ          (0x02 << 2)  /* 10: RTJ */
#define TAS6754_AUDIO_INTERFACE_FORMAT_LTJ          (0x03 << 2)  /* 11: LTJ */

/* FS PULSE WIDTH values (bits 1-0) */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_GTE8       (0x00 << 0)  /* 00: High width of FSYNC in TDM/DSP mode is equal to or greater than 8 cycles of SCLK */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_LT8        (0x01 << 0)  /* 01: High width of FSYNC in TDM/DSP mode is less than 8 cycles of SCLK */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_RESERVED_2 (0x02 << 0)  /* 10: Reserved */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_RESERVED_3 (0x03 << 0)  /* 11: Reserved */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_AUDIO_INTERFACE_RESERVED_MASK       (TAS6754_AUDIO_INTERFACE_RESERVED_6_MASK | \
                                                    TAS6754_AUDIO_INTERFACE_RESERVED_5_MASK)

/* Reset value */
#define TAS6754_AUDIO_INTERFACE_CTRL_RESET          0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SDIN_CTRL (0x23)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN_MASK       BIT(7)          /* Bit 7: TDM AUDIO SDIN */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN_MASK          BIT(6)          /* Bit 6: TDM LL SDIN */
#define TAS6754_SDIN_CTRL_RESERVED_5_MASK           BIT(5)          /* Bit 5: Reserved */
#define TAS6754_SDIN_CTRL_RESERVED_4_MASK           BIT(4)          /* Bit 4: Reserved */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_MASK          GENMASK(3, 2)   /* Bits 3-2: SDIN WL SELECT 1 */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_MASK          GENMASK(1, 0)   /* Bits 1-0: SDIN WL SELECT 2 */

/* TDM AUDIO SDIN values (bit 7) */
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN1           (0x00 << 7)  /* 0: Select data from SDIN1 */
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN2           (0x01 << 7)  /* 1: Select data from SDIN2 */

/* TDM LL SDIN values (bit 6) */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN1              (0x00 << 6)  /* 0: Select data from SDIN1 */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN2              (0x01 << 6)  /* 1: Select data from SDIN2 */

/* SDIN WL SELECT 1 values (bits 3-2) */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_16BIT         (0x00 << 2)  /* 00: 16 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_20BIT         (0x01 << 2)  /* 01: 20 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_24BIT         (0x02 << 2)  /* 10: 24 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_32BIT         (0x03 << 2)  /* 11: 32 bits */

/* SDIN WL SELECT 2 values (bits 1-0) */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_16BIT         (0x00 << 0)  /* 00: 16 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_20BIT         (0x01 << 0)  /* 01: 20 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_24BIT         (0x02 << 0)  /* 10: 24 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_32BIT         (0x03 << 0)  /* 11: 32 bits */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_SDIN_CTRL_RESERVED_MASK             (TAS6754_SDIN_CTRL_RESERVED_5_MASK | \
                                                    TAS6754_SDIN_CTRL_RESERVED_4_MASK)

/* Reset value */
#define TAS6754_SDIN_CTRL_RESET                     0x0A            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SDOUT_CTRL (0x25)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDOUT_CTRL_SDOUT_SELECT_MASK        GENMASK(7, 4)   /* Bits 7-4: SDOUT SELECT */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_MASK         GENMASK(3, 2)   /* Bits 3-2: SDOUT WL SELECT 1 */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_MASK         GENMASK(1, 0)   /* Bits 1-0: SDOUT WL SELECT 2 */

/* SDOUT SELECT values (bits 7-4) */
#define TAS6754_SDOUT_CTRL_TDM_SDOUT1               (0x00 << 4)  /* 0000: In TDM mode SDOUT output place on SDOUT1 */
#define TAS6754_SDOUT_CTRL_NONTDM_12_34             (0x01 << 4)  /* 0001: For non-TDM mode, output Channel 1/2 in SDOUT1 line and Channel 3/4 in SDOUT2 line */
#define TAS6754_SDOUT_CTRL_NONTDM_34_12             (0x02 << 4)  /* 0010: For non-TDM mode, output Channel 3/4 in SDOUT1 line and Channel 1/2 in SDOUT2 line */
#define TAS6754_SDOUT_CTRL_TDM_SDOUT2               (0x0F << 4)  /* 1111: In TDM mode SDOUT output place on SDOUT2 */

/* SDOUT WL SELECT 1 values (bits 3-2) */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_16BIT        (0x00 << 2)  /* 00: 16 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_20BIT        (0x01 << 2)  /* 01: 20 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_24BIT        (0x02 << 2)  /* 10: 24 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_32BIT        (0x03 << 2)  /* 11: 32 bits */

/* SDOUT WL SELECT 2 values (bits 1-0) */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_16BIT        (0x00 << 0)  /* 00: 16 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_20BIT        (0x01 << 0)  /* 01: 20 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_24BIT        (0x02 << 0)  /* 10: 24 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_32BIT        (0x03 << 0)  /* 11: 32 bits */

/* Reset value */
#define TAS6754_SDOUT_CTRL_RESET                    0x1A            /* Reset value for the register */
/************************************************************************
 *					 TAS6754_SDIN_OFFSET_MSB (0x27)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_MASK     GENMASK(7, 6)   /* Bits 7-6: AUDIO PATH OFFSET MSB */
#define TAS6754_SDIN_OFFSET_LL_PATH_MSB_MASK        GENMASK(5, 4)   /* Bits 5-4: LL PATH OFFSET MSB */
#define TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_MASK     GENMASK(3, 2)   /* Bits 3-2: LL CH SWAP MSB */
#define TAS6754_SDIN_OFFSET_RESERVED_MASK           GENMASK(1, 0)   /* Bits 1-0: Reserved */

/* Bit positions for shifting */
#define TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT    6
#define TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT       4
#define TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT    2

/* Helper macros for extracting MSB values */
#define TAS6754_SDIN_OFFSET_GET_AUDIO_PATH_MSB(val) (((val) & TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_MASK) >> TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_GET_LL_PATH_MSB(val)    (((val) & TAS6754_SDIN_OFFSET_LL_PATH_MSB_MASK) >> TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_GET_LL_CH_SWAP_MSB(val) (((val) & TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_MASK) >> TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT)

/* Helper macros for setting MSB values */
#define TAS6754_SDIN_OFFSET_SET_AUDIO_PATH_MSB(val) (((val) & 0x03) << TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_SET_LL_PATH_MSB(val)    (((val) & 0x03) << TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_SET_LL_CH_SWAP_MSB(val) (((val) & 0x03) << TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT)

/* Reset value */
#define TAS6754_SDIN_OFFSET_MSB_RESET               0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SDIN_AUDIO_OFFSET (0x28)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDIN_AUDIO_OFFSET_LSB_MASK          0xFF            /* Bits 7-0: AUDIO PATH OFFSET LSB */

/* Helper macros for offset calculation */
#define TAS6754_SDIN_AUDIO_OFFSET_GET_LSB(val)      ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit offset value
 * msb_val should be the 2-bit value extracted from SDIN_OFFSET_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_SDIN_AUDIO_OFFSET_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Helper macro to set a specific offset value (0-511) */
#define TAS6754_SDIN_AUDIO_OFFSET_SET(offset)       ((offset) & 0x1FF)  /* Limit to 9 valid bits */

/* Common offset values */
#define TAS6754_SDIN_AUDIO_OFFSET_0                 0x000           /* No offset */
#define TAS6754_SDIN_AUDIO_OFFSET_1                 0x001           /* 1 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_2                 0x002           /* 2 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_8                 0x008           /* 8 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_16                0x010           /* 16 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_32                0x020           /* 32 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_64                0x040           /* 64 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_128               0x080           /* 128 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_256               0x100           /* 256 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_511               0x1FF           /* Maximum valid offset (511 SCLK) */

/* Reset value */
#define TAS6754_SDIN_AUDIO_OFFSET_RESET             0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SDIN_LL_OFFSET (0x29)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDIN_LL_OFFSET_LSB_MASK             0xFF            /* Bits 7-0: LOW LATENCY PATH OFFSET LSB */

/* Helper macros for offset calculation */
#define TAS6754_SDIN_LL_OFFSET_GET_LSB(val)         ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit offset value
 * msb_val should be the 2-bit value extracted from SDIN_OFFSET_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_SDIN_LL_OFFSET_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Helper macro to set a specific offset value (0-511) */
#define TAS6754_SDIN_LL_OFFSET_SET(offset)          ((offset) & 0x1FF)  /* Limit to 9 valid bits */

/* Common offset values */
#define TAS6754_SDIN_LL_OFFSET_0                    0x000           /* 0 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_1                    0x001           /* 1 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_2                    0x002           /* 2 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_8                    0x008           /* 8 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_16                   0x010           /* 16 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_32                   0x020           /* 32 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_64                   0x040           /* 64 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_96                   0x060           /* 96 SCLK offset (default) */
#define TAS6754_SDIN_LL_OFFSET_128                  0x080           /* 128 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_256                  0x100           /* 256 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_511                  0x1FF           /* Maximum valid offset (511 SCLK) */

/* Reset value */
#define TAS6754_SDIN_LL_OFFSET_RESET                0x60            /* Reset value for the register (96 SCLK offset) */
/************************************************************************
 *					TAS6754_SDIN_CH_SWAP (0x2A)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDIN_CH_SWAP_LL_LSB_MASK            GENMASK(7, 5)   /* Bits 7-5: LOW LATENCY CHANNEL SWAP LSB */
#define TAS6754_SDIN_CH_SWAP_AUDIO_MASK             GENMASK(4, 0)   /* Bits 4-0: AUDIO CHANNEL SWAP */

/* Bit positions for shifting */
#define TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT           5

/* Helper macros for extracting values */
#define TAS6754_SDIN_CH_SWAP_GET_LL_LSB(val)        (((val) & TAS6754_SDIN_CH_SWAP_LL_LSB_MASK) >> TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT)
#define TAS6754_SDIN_CH_SWAP_GET_AUDIO(val)         ((val) & TAS6754_SDIN_CH_SWAP_AUDIO_MASK)

/* 
 * Combine MSB (2 bits) and LSB (3 bits) to get 5-bit LL channel swap value
 * msb_val should be the 2-bit value extracted from SDIN_OFFSET_MSB register
 * lsb_val should be the 3-bit value extracted from this register
 */
#define TAS6754_SDIN_CH_SWAP_LL_COMBINE(msb_val, lsb_val) \
                                                    (((uint8_t)(msb_val) << 3) | (lsb_val))

/* Helper macros for setting values */
#define TAS6754_SDIN_CH_SWAP_SET_LL_LSB(val)        (((val) & 0x07) << TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT)
#define TAS6754_SDIN_CH_SWAP_SET_AUDIO(val)         ((val) & 0x1F)

/* Common audio channel swap configurations (examples - actual mapping would depend on device documentation) */
#define TAS6754_SDIN_CH_SWAP_AUDIO_DEFAULT          0x00            /* Default mapping */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_1_2         0x01            /* Swap channels 1 and 2 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_3_4         0x02            /* Swap channels 3 and 4 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_1_3         0x04            /* Swap channels 1 and 3 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_2_4         0x08            /* Swap channels 2 and 4 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_REVERSE          0x0F            /* Reverse channel order */

/* Reset value */
#define TAS6754_SDIN_CH_SWAP_RESET                  0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_SDOUT_OFFSET_MSB (0x2C)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDOUT_OFFSET_VPREDICT_MSB_MASK      GENMASK(7, 6)   /* Bits 7-6: VPREDICT OFFSET MSB */
#define TAS6754_SDOUT_OFFSET_ISENSE_MSB_MASK        GENMASK(5, 4)   /* Bits 5-4: ISENSE OFFSET MSB */
#define TAS6754_SDOUT_OFFSET_RESERVED_3_2_MASK      GENMASK(3, 2)   /* Bits 3-2: Reserved */
#define TAS6754_SDOUT_OFFSET_RESERVED_1_0_MASK      GENMASK(1, 0)   /* Bits 1-0: Reserved */

/* Bit positions for shifting */
#define TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT     6
#define TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT       4
#define TAS6754_SDOUT_OFFSET_RESERVED_3_2_SHIFT     2
#define TAS6754_SDOUT_OFFSET_RESERVED_1_0_SHIFT     0

/* Helper macros for extracting MSB values */
#define TAS6754_SDOUT_OFFSET_GET_VPREDICT_MSB(val)  (((val) & TAS6754_SDOUT_OFFSET_VPREDICT_MSB_MASK) >> TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_GET_ISENSE_MSB(val)    (((val) & TAS6754_SDOUT_OFFSET_ISENSE_MSB_MASK) >> TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT)

/* Helper macros for setting MSB values */
#define TAS6754_SDOUT_OFFSET_SET_VPREDICT_MSB(val)  (((val) & 0x03) << TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_SET_ISENSE_MSB(val)    (((val) & 0x03) << TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_SDOUT_OFFSET_RESERVED_MASK          (TAS6754_SDOUT_OFFSET_RESERVED_3_2_MASK | \
                                                    TAS6754_SDOUT_OFFSET_RESERVED_1_0_MASK)

/* Reset value */
#define TAS6754_SDOUT_OFFSET_MSB_RESET              0xCF            /* Reset value for the register */
/************************************************************************
 *					TAS6754_VPREDICT_OFFSET (0x2D)
 ************************************************************************/
/************************************************************************

  			TAS6754_VPREDICT_OFFSET_REG (0x2D)
************************************************************************/ #define TAS6754_VPREDICT_OFFSET_REG 0x2D
/* Bit field masks */
#define TAS6754_VPREDICT_OFFSET_LSB_MASK            GENMASK(7, 0)   /* Bits 7-0: VPREDICT OFFSET LSB */

/* Bit positions for shifting */
#define TAS6754_VPREDICT_OFFSET_LSB_SHIFT           0

/* Helper macros for extracting values */
#define TAS6754_VPREDICT_OFFSET_GET_LSB(val)        ((val) & TAS6754_VPREDICT_OFFSET_LSB_MASK)

/* Helper macros for setting values */
#define TAS6754_VPREDICT_OFFSET_SET_LSB(val)        ((val) & 0xFF)

/* Reset value */
#define TAS6754_VPREDICT_OFFSET_RESET               0xFF            /* Reset value for the register */

/* Combined offset access macros (for convenience) */
#define TAS6754_VPREDICT_OFFSET_GET_FULL(msb_reg, lsb_reg) \
    ((TAS6754_SDOUT_OFFSET_GET_VPREDICT_MSB(msb_reg) << 8) | TAS6754_VPREDICT_OFFSET_GET_LSB(lsb_reg))

/* Special offset values */
#define TAS6754_VPREDICT_OFFSET_MAX_VALID           511             /* Maximum valid offset within TDM frame */
#define TAS6754_VPREDICT_OFFSET_OUTSIDE_TDM         1023            /* Offset value outside TDM frame (all bits set)
/************************************************************************
 *					TAS6754_ISENSE_OFFSET (0x2E)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_ISENSE_OFFSET_LSB_MASK              GENMASK(7, 0)   /* Bits 7-0: ISENSE OFFSET LSB */

/* Bit positions for shifting */
#define TAS6754_ISENSE_OFFSET_LSB_SHIFT             0

/* Helper macros for extracting values */
#define TAS6754_ISENSE_OFFSET_GET_LSB(val)          ((val) & TAS6754_ISENSE_OFFSET_LSB_MASK)

/* Helper macros for setting values */
#define TAS6754_ISENSE_OFFSET_SET_LSB(val)          ((val) & 0xFF)

/* Reset value */
#define TAS6754_ISENSE_OFFSET_RESET                 0x00            /* Reset value for the register */

/* Combined offset access macros (for convenience) */
#define TAS6754_ISENSE_OFFSET_GET_FULL(msb_reg, lsb_reg) \
    ((TAS6754_SDOUT_OFFSET_GET_ISENSE_MSB(msb_reg) << 8) | TAS6754_ISENSE_OFFSET_GET_LSB(lsb_reg))

#define TAS6754_ISENSE_OFFSET_MAX                   511             /* Maximum valid offset value */
/************************************************************************
 *					TAS6754_SDOUT_EN (0x31)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_SDOUT_EN_RESERVED_MASK              GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_SDOUT_EN_NON_TDM_SELECT_MASK        GENMASK(5, 4)   /* Bits 5-4: NON-TDM SDOUT SELECT */
#define TAS6754_SDOUT_EN_ENABLE_MASK                GENMASK(3, 0)   /* Bits 3-0: SDOUT ENABLE */

/* Bit positions for shifting */
#define TAS6754_SDOUT_EN_RESERVED_SHIFT             6
#define TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT       4
#define TAS6754_SDOUT_EN_ENABLE_SHIFT               0

/* Helper macros for extracting values */
#define TAS6754_SDOUT_EN_GET_NON_TDM_SELECT(val)    (((val) & TAS6754_SDOUT_EN_NON_TDM_SELECT_MASK) >> TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT)
#define TAS6754_SDOUT_EN_GET_ENABLE(val)            (((val) & TAS6754_SDOUT_EN_ENABLE_MASK) >> TAS6754_SDOUT_EN_ENABLE_SHIFT)

/* Helper macros for setting values */
#define TAS6754_SDOUT_EN_SET_NON_TDM_SELECT(val)    (((val) & 0x03) << TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT)
#define TAS6754_SDOUT_EN_SET_ENABLE(val)            (((val) & 0x0F) << TAS6754_SDOUT_EN_ENABLE_SHIFT)

/* NON-TDM SDOUT SELECT values */
#define TAS6754_SDOUT_EN_NON_TDM_VPREDICT           0x00            /* Vpredict Channel 1/2/3/4 */
#define TAS6754_SDOUT_EN_NON_TDM_ISENSE             0x01            /* Isense Channel 1/2/3/4 */

/* SDOUT ENABLE bit definitions for TDM mode */
#define TAS6754_SDOUT_EN_TDM_DISABLE_ALL            0x00            /* Disable all output data channels */
#define TAS6754_SDOUT_EN_TDM_VPREDICT               0x01            /* Enable Vpredict Channel 1/2/3/4 output */
#define TAS6754_SDOUT_EN_TDM_ISENSE                 0x02            /* Enable Isense Channel 1/2/3/4 output */
#define TAS6754_SDOUT_EN_TDM_ALL                    0x03            /* Enable both Vpredict and Isense */

/* SDOUT ENABLE bit definitions for NON-TDM mode */
#define TAS6754_SDOUT_EN_NON_TDM_DISABLE_ALL        0x00            /* Disable all output data channels */
#define TAS6754_SDOUT_EN_NON_TDM_ENABLE_ALL         0x03            /* Enable all output data channels */

/* Reset value */
#define TAS6754_SDOUT_EN_RESET                      0x00            /* Reset value for the register */
/************************************************************************
 *					TAS6754_LL_EN (0x32)
 ************************************************************************/
/* Bit field masks */
#define TAS6754_LL_EN_RESERVED_7_MASK               BIT(7)          /* Bit 7: Reserved */
#define TAS6754_LL_EN_RESERVED_6_3_MASK             GENMASK(6, 3)   /* Bits 6-3: Reserved */
#define TAS6754_LL_EN_RESERVED_2_MASK               BIT(2)          /* Bit 2: Reserved */
#define TAS6754_LL_EN_FFLP_ENABLE_MASK              BIT(1)          /* Bit 1: FFLP ENABLE */
#define TAS6754_LL_EN_LLP_ENABLE_MASK               BIT(0)          /* Bit 0: LLP ENABLE */

/* Bit positions for shifting */
#define TAS6754_LL_EN_RESERVED_7_SHIFT              7
#define TAS6754_LL_EN_RESERVED_6_3_SHIFT            3
#define TAS6754_LL_EN_RESERVED_2_SHIFT              2
#define TAS6754_LL_EN_FFLP_ENABLE_SHIFT             1
#define TAS6754_LL_EN_LLP_ENABLE_SHIFT              0

/* Helper macros for extracting values */
#define TAS6754_LL_EN_GET_FFLP_ENABLE(val)          (((val) & TAS6754_LL_EN_FFLP_ENABLE_MASK) >> TAS6754_LL_EN_FFLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_GET_LLP_ENABLE(val)           (((val) & TAS6754_LL_EN_LLP_ENABLE_MASK) >> TAS6754_LL_EN_LLP_ENABLE_SHIFT)

/* Helper macros for setting values */
#define TAS6754_LL_EN_SET_FFLP_ENABLE(val)          (((val) & 0x01) << TAS6754_LL_EN_FFLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_SET_LLP_ENABLE(val)           (((val) & 0x01) << TAS6754_LL_EN_LLP_ENABLE_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_LL_EN_RESERVED_MASK                 (TAS6754_LL_EN_RESERVED_7_MASK | \
                                                    TAS6754_LL_EN_RESERVED_6_3_MASK | \
                                                    TAS6754_LL_EN_RESERVED_2_MASK)

/* Feature enable/disable values */
#define TAS6754_LL_EN_FFLP_DISABLED                 0x00            /* Full Feature Low Latency path disabled */
#define TAS6754_LL_EN_FFLP_ENABLED                  0x01            /* Full Feature Low Latency path enabled */
#define TAS6754_LL_EN_LLP_DISABLED                  0x00            /* Low Latency path disabled */
#define TAS6754_LL_EN_LLP_ENABLED                   0x01            /* Low Latency path enabled */

/* Common configurations */
#define TAS6754_LL_EN_ALL_DISABLED                  0x00            /* Both paths disabled */
#define TAS6754_LL_EN_ONLY_LLP                      0x01            /* Only Low Latency path enabled */
#define TAS6754_LL_EN_ONLY_FFLP                     0x02            /* Only Full Feature Low Latency path enabled */
#define TAS6754_LL_EN_ALL_ENABLED                   0x03            /* Both paths enabled */

/* Reset value */
#define TAS6754_LL_EN_RESET                         0x00            /* Reset value for the register */
/************************************************************************
*					TAS6754_RTLDG_EN (0x37)
************************************************************************/
/* Bit field masks */
#define TAS6754_RTLDG_EN_RESERVED_MASK              GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_RTLDG_EN_CLIP_MASK_MASK             BIT(4)          /* Bit 4: RTLDG CLIP MASK */
#define TAS6754_RTLDG_EN_CH1_OLSL_MASK              BIT(3)          /* Bit 3: CH1 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH2_OLSL_MASK              BIT(2)          /* Bit 2: CH2 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH3_OLSL_MASK              BIT(1)          /* Bit 1: CH3 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH4_OLSL_MASK              BIT(0)          /* Bit 0: CH4 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_MASK           GENMASK(3, 0)   /* Bits 3-0: All channel OLSL enables */

/* Bit positions for shifting */
#define TAS6754_RTLDG_EN_RESERVED_SHIFT             5
#define TAS6754_RTLDG_EN_CLIP_MASK_SHIFT            4
#define TAS6754_RTLDG_EN_CH1_OLSL_SHIFT             3
#define TAS6754_RTLDG_EN_CH2_OLSL_SHIFT             2
#define TAS6754_RTLDG_EN_CH3_OLSL_SHIFT             1
#define TAS6754_RTLDG_EN_CH4_OLSL_SHIFT             0

/* Helper macros for extracting values */
#define TAS6754_RTLDG_EN_GET_CLIP_MASK(val)         (((val) & TAS6754_RTLDG_EN_CLIP_MASK_MASK) >> TAS6754_RTLDG_EN_CLIP_MASK_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH1_OLSL(val)          (((val) & TAS6754_RTLDG_EN_CH1_OLSL_MASK) >> TAS6754_RTLDG_EN_CH1_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH2_OLSL(val)          (((val) & TAS6754_RTLDG_EN_CH2_OLSL_MASK) >> TAS6754_RTLDG_EN_CH2_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH3_OLSL(val)          (((val) & TAS6754_RTLDG_EN_CH3_OLSL_MASK) >> TAS6754_RTLDG_EN_CH3_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH4_OLSL(val)          (((val) & TAS6754_RTLDG_EN_CH4_OLSL_MASK) >> TAS6754_RTLDG_EN_CH4_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_ALL_CH_OLSL(val)       (((val) & TAS6754_RTLDG_EN_ALL_CH_OLSL_MASK) >> 0)

/* Helper macros for setting values */
#define TAS6754_RTLDG_EN_SET_CLIP_MASK(val)         (((val) & 0x01) << TAS6754_RTLDG_EN_CLIP_MASK_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH1_OLSL(val)          (((val) & 0x01) << TAS6754_RTLDG_EN_CH1_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH2_OLSL(val)          (((val) & 0x01) << TAS6754_RTLDG_EN_CH2_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH3_OLSL(val)          (((val) & 0x01) << TAS6754_RTLDG_EN_CH3_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH4_OLSL(val)          (((val) & 0x01) << TAS6754_RTLDG_EN_CH4_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_ALL_CH_OLSL(val)       (((val) & 0x0F) << 0)

/* RTLDG CLIP MASK values */
#define TAS6754_RTLDG_EN_CLIP_MASK_DISABLED         0x00            /* Real-time load diagnostic report is not masked when clip */
#define TAS6754_RTLDG_EN_CLIP_MASK_ENABLED          0x01            /* Real-time load diagnostic report is masked when clip fault occurs */

/* Channel OLSL enable/disable values */
#define TAS6754_RTLDG_EN_CH_OLSL_DISABLED           0x00            /* Disable Real-time load diagnostic open load/shorted load */
#define TAS6754_RTLDG_EN_CH_OLSL_ENABLED            0x01            /* Enable Real-time load diagnostic open load/shorted load */

/* Common configurations */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_DISABLED       0x00            /* All channels OLSL disabled */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_ENABLED        0x0F            /* All channels OLSL enabled */

/* Reset value */
#define TAS6754_RTLDG_EN_RESET                      0x10            /* Reset value for the register */
/************************************************************************
*					TAS6754_DC_BLOCK_BYP (0x39)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_BLOCK_BYP_RESERVED_MASK          GENMASK(7, 1)   /* Bits 7-1: Reserved */
#define TAS6754_DC_BLOCK_BYP_BYPASS_MASK            BIT(0)          /* Bit 0: DC BLOCK BYPASS */

/* Bit positions for shifting */
#define TAS6754_DC_BLOCK_BYP_RESERVED_SHIFT         1
#define TAS6754_DC_BLOCK_BYP_BYPASS_SHIFT           0

/* Helper macros for extracting values */
#define TAS6754_DC_BLOCK_BYP_GET_BYPASS(val)        ((val) & TAS6754_DC_BLOCK_BYP_BYPASS_MASK)

/* Helper macros for setting values */
#define TAS6754_DC_BLOCK_BYP_SET_BYPASS(val)        ((val) & 0x01)

/* DC BLOCK BYPASS values */
#define TAS6754_DC_BLOCK_BYP_ENABLED                0x00            /* Enable DC Blocking */
#define TAS6754_DC_BLOCK_BYP_BYPASSED               0x01            /* Bypass DC Blocking */

/* Reset value */
#define TAS6754_DC_BLOCK_BYP_RESET                  0x00            /* Reset value for the register */
/************************************************************************
*					TAS6754_DSP_CTRL (0x3A)
************************************************************************/
/* Bit field masks */
#define TAS6754_DSP_CTRL_RESERVED_7_6_MASK          GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_MASK       BIT(5)          /* Bit 5: AUDIO SDOUT SELECT */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_MASK      BIT(4)          /* Bit 4: PVDD FOLDBACK ENABLE */
#define TAS6754_DSP_CTRL_RESERVED_3_1_MASK          GENMASK(3, 1)   /* Bits 3-1: Reserved */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_MASK   BIT(0)          /* Bit 0: THERMAL FOLDBACK ENABLE */

/* Bit positions for shifting */
#define TAS6754_DSP_CTRL_RESERVED_7_6_SHIFT         6
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT      5
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT     4
#define TAS6754_DSP_CTRL_RESERVED_3_1_SHIFT         1
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT  0

/* Helper macros for extracting values */
#define TAS6754_DSP_CTRL_GET_AUDIO_SDOUT_SEL(val)   (((val) & TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_MASK) >> TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT)
#define TAS6754_DSP_CTRL_GET_PVDD_FOLDBACK_EN(val)  (((val) & TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_MASK) >> TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_GET_THERMAL_FOLDBACK_EN(val) (((val) & TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_MASK) >> TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT)

/* Helper macros for setting values */
#define TAS6754_DSP_CTRL_SET_AUDIO_SDOUT_SEL(val)   (((val) & 0x01) << TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT)
#define TAS6754_DSP_CTRL_SET_PVDD_FOLDBACK_EN(val)  (((val) & 0x01) << TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_SET_THERMAL_FOLDBACK_EN(val) (((val) & 0x01) << TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_DSP_CTRL_RESERVED_MASK              (TAS6754_DSP_CTRL_RESERVED_7_6_MASK | \
                                                    TAS6754_DSP_CTRL_RESERVED_3_1_MASK)

/* AUDIO SDOUT SELECT values */
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_DISABLED       0x00            /* Post-processed audio signal not routed to SDOUT */
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_ENABLED        0x01            /* Post-processed audio signal routed to SDOUT. Replaces Vpredict data */

/* PVDD FOLDBACK ENABLE values */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_DISABLED     0x00            /* Disable PVDD Foldback */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_ENABLED      0x01            /* Enable PVDD Foldback */

/* THERMAL FOLDBACK ENABLE values */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_DISABLED  0x00            /* Disable Thermal Foldback */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_ENABLED   0x01            /* Enable Thermal Foldback */

/* Reset value */
#define TAS6754_DSP_CTRL_RESET                      0x00            /* Reset value for the register */
/************************************************************************
*					TAS6754_PAGE_AUTO_INC (0x3B)
************************************************************************/
/* Bit field masks */
#define TAS6754_PAGE_AUTO_INC_RESERVED_7_4_MASK     GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_PAGE_AUTO_INC_DISABLE_MASK          BIT(3)          /* Bit 3: PAGE AUTO INCREMENT DISABLE */
#define TAS6754_PAGE_AUTO_INC_RESERVED_2_MASK       BIT(2)          /* Bit 2: Reserved */
#define TAS6754_PAGE_AUTO_INC_RESERVED_1_0_MASK     GENMASK(1, 0)   /* Bits 1-0: Reserved */

/* Bit positions for shifting */
#define TAS6754_PAGE_AUTO_INC_RESERVED_7_4_SHIFT    4
#define TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT         3
#define TAS6754_PAGE_AUTO_INC_RESERVED_2_SHIFT      2
#define TAS6754_PAGE_AUTO_INC_RESERVED_1_0_SHIFT    0

/* Helper macros for extracting values */
#define TAS6754_PAGE_AUTO_INC_GET_DISABLE(val)      (((val) & TAS6754_PAGE_AUTO_INC_DISABLE_MASK) >> TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT)

/* Helper macros for setting values */
#define TAS6754_PAGE_AUTO_INC_SET_DISABLE(val)      (((val) & 0x01) << TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_PAGE_AUTO_INC_RESERVED_MASK         (TAS6754_PAGE_AUTO_INC_RESERVED_7_4_MASK | \
                                                    TAS6754_PAGE_AUTO_INC_RESERVED_2_MASK | \
                                                    TAS6754_PAGE_AUTO_INC_RESERVED_1_0_MASK)

/* PAGE AUTO INCREMENT DISABLE values */
#define TAS6754_PAGE_AUTO_INC_ENABLED               0x00            /* Enable Page auto increment */
#define TAS6754_PAGE_AUTO_INC_DISABLED              0x01            /* Disable Page auto increment */

/* Reset value */
#define TAS6754_PAGE_AUTO_INC_RESET                 0x00            /* Reset value for the register */
/************************************************************************
*					TAS6754_DIG_VOL_CH1 (0x40)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_CH1_MASK                    GENMASK(7, 0)   /* Bits 7-0: CH1 DIGITAL VOLUME */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_CH1_SHIFT                   0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_CH1_GET_VOL(val)            ((val) & TAS6754_DIG_VOL_CH1_MASK)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_CH1_SET_VOL(val)            ((val) & 0xFF)

/* Special volume values */
#define TAS6754_DIG_VOL_CH1_0DB                     0x30            /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH1_MINUS_0_5DB             0x31            /* -0.5 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_1DB               0x32            /* -1.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_6DB               0x3C            /* -6.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_12DB              0x48            /* -12.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_20DB              0x58            /* -20.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_40DB              0x80            /* -40.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_60DB              0xA8            /* -60.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_80DB              0xD0            /* -80.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_100DB             0xF8            /* -100.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_103DB             0xFE            /* -103.0 dB */
#define TAS6754_DIG_VOL_CH1_MUTE                    0xFF            /* Mute */

/* Helper macro to convert dB to register value */
#define TAS6754_DIG_VOL_CH1_DB_TO_REG(db)           ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)

/* Helper macro to convert register value to dB */
#define TAS6754_DIG_VOL_CH1_REG_TO_DB(reg)          ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))

/* Reset value */
#define TAS6754_DIG_VOL_CH1_RESET                   0x30            /* Reset value for the register (0.0 dB) */
/************************************************************************
*					TAS6754_DIG_VOL_CH2 (0x41)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_CH2_MASK                    GENMASK(7, 0)   /* Bits 7-0: CH2 DIGITAL VOLUME */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_CH2_SHIFT                   0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_CH2_GET_VOL(val)            ((val) & TAS6754_DIG_VOL_CH2_MASK)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_CH2_SET_VOL(val)            ((val) & 0xFF)

/* Special volume values */
#define TAS6754_DIG_VOL_CH2_0DB                     0x30            /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH2_MINUS_0_5DB             0x31            /* -0.5 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_1DB               0x32            /* -1.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_6DB               0x3C            /* -6.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_12DB              0x48            /* -12.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_20DB              0x58            /* -20.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_40DB              0x80            /* -40.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_60DB              0xA8            /* -60.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_80DB              0xD0            /* -80.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_100DB             0xF8            /* -100.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_103DB             0xFE            /* -103.0 dB */
#define TAS6754_DIG_VOL_CH2_MUTE                    0xFF            /* Mute */

/* Helper macro to convert dB to register value */
#define TAS6754_DIG_VOL_CH2_DB_TO_REG(db)           ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)

/* Helper macro to convert register value to dB */
#define TAS6754_DIG_VOL_CH2_REG_TO_DB(reg)          ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))

/* Reset value */
#define TAS6754_DIG_VOL_CH2_RESET                   0x30            /* Reset value for the register (0.0 dB) */
/************************************************************************
*					TAS6754_DIG_VOL_CH3 (0x42)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_CH3_MASK                    GENMASK(7, 0)   /* Bits 7-0: CH3 DIGITAL VOLUME */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_CH3_SHIFT                   0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_CH3_GET_VOL(val)            ((val) & TAS6754_DIG_VOL_CH3_MASK)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_CH3_SET_VOL(val)            ((val) & 0xFF)

/* Special volume values */
#define TAS6754_DIG_VOL_CH3_0DB                     0x30            /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH3_MINUS_0_5DB             0x31            /* -0.5 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_1DB               0x32            /* -1.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_6DB               0x3C            /* -6.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_12DB              0x48            /* -12.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_20DB              0x58            /* -20.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_40DB              0x80            /* -40.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_60DB              0xA8            /* -60.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_80DB              0xD0            /* -80.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_100DB             0xF8            /* -100.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_103DB             0xFE            /* -103.0 dB */
#define TAS6754_DIG_VOL_CH3_MUTE                    0xFF            /* Mute */

/* Helper macro to convert dB to register value */
#define TAS6754_DIG_VOL_CH3_DB_TO_REG(db)           ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)

/* Helper macro to convert register value to dB */
#define TAS6754_DIG_VOL_CH3_REG_TO_DB(reg)          ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))

/* Reset value */
#define TAS6754_DIG_VOL_CH3_RESET                   0x30            /* Reset value for the register (0.0 dB) */
/************************************************************************
*					TAS6754_DIG_VOL_CH4 (0x43)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_CH4_MASK                    GENMASK(7, 0)   /* Bits 7-0: CH4 DIGITAL VOLUME */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_CH4_SHIFT                   0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_CH4_GET_VOL(val)            ((val) & TAS6754_DIG_VOL_CH4_MASK)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_CH4_SET_VOL(val)            ((val) & 0xFF)

/* Special volume values */
#define TAS6754_DIG_VOL_CH4_0DB                     0x30            /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH4_MINUS_0_5DB             0x31            /* -0.5 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_1DB               0x32            /* -1.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_6DB               0x3C            /* -6.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_12DB              0x48            /* -12.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_20DB              0x58            /* -20.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_40DB              0x80            /* -40.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_60DB              0xA8            /* -60.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_80DB              0xD0            /* -80.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_100DB             0xF8            /* -100.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_103DB             0xFE            /* -103.0 dB */
#define TAS6754_DIG_VOL_CH4_MUTE                    0xFF            /* Mute */

/* Helper macro to convert dB to register value */
#define TAS6754_DIG_VOL_CH4_DB_TO_REG(db)           ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)

/* Helper macro to convert register value to dB */
#define TAS6754_DIG_VOL_CH4_REG_TO_DB(reg)          ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))

/* Reset value */
#define TAS6754_DIG_VOL_CH4_RESET                   0x30            /* Reset value for the register (0.0 dB) */
/************************************************************************
*                   Common Definitions for Digital Volume Registers
*					TAS6754_DIG_VOL_CH1 (0x40)
*					TAS6754_DIG_VOL_CH2 (0x41)
*					TAS6754_DIG_VOL_CH3 (0x42)
*					TAS6754_DIG_VOL_CH4 (0x43)
************************************************************************/
#define TAS6754_DIG_VOL_CHx_MAX				 			(0x30)  /* Maximum volume 0.0 dB */
#define TAS6754_DIG_VOL_CHx_MIN				 			(0xFE)  /* Minimum volume -103.0 dB */
#define TAS6754_DIG_VOL_CHx_MUTE                   		(0xFF)  /* Mute: no sound */
/* Helper macro for calculating volume values */
/**
 * Convert dB value to register value
 * Valid range: 0 to -103 dB in 0.5 dB steps
 * Rationale:
 * 		-db negates the dB value (since dB values are negative but we need a positive number for calculation)
 * 		(-db) * 2 multiplies by 2 because each step is 0.5 dB (so -1 dB would be 2 steps, -1.5 dB would be 3 steps, etc.)
 * 		(uint8_t) casts the result to an 8-bit unsigned integer
 * 		0x30 + adds the base value (0x30 corresponds to 0 dB) to get the final register value
 * @param dB: Volume in dB (should be 0 or negative)
 * @return: Register value
 **/
#define TAS6754_DIG_VOL_CHx_DB_TO_VOLUME(dB)			((dB <= -103.0) ? TAS6754_DIG_VOL_MIN : \
                                                		((dB == -0.0) ? TAS6754_DIG_VOL_MAX : \
                                                		(TAS6754_DIG_VOL_MAX + (uint8_t)((-dB) * 2))))
/************************************************************************
* 					TAS6754_DIG_VOL_RAMP_CTRL (0x44)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_MASK        GENMASK(7, 6)   /* Bits 7-6: DIGITAL VOLUME RAMP DOWN FREQUENCY */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_MASK        GENMASK(5, 4)   /* Bits 5-4: DIGITAL VOLUME RAMP DOWN STEP */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_MASK          GENMASK(3, 2)   /* Bits 3-2: DIGITAL VOLUME RAMP UP FREQUENCY */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_MASK          GENMASK(1, 0)   /* Bits 1-0: DIGITAL VOLUME RAMP UP STEP */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT       6
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT       4
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT         2
#define TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT         0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_RAMP_GET_DOWN_FREQ(val)    (((val) & TAS6754_DIG_VOL_RAMP_DOWN_FREQ_MASK) >> TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_DOWN_STEP(val)    (((val) & TAS6754_DIG_VOL_RAMP_DOWN_STEP_MASK) >> TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_UP_FREQ(val)      (((val) & TAS6754_DIG_VOL_RAMP_UP_FREQ_MASK) >> TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_UP_STEP(val)      (((val) & TAS6754_DIG_VOL_RAMP_UP_STEP_MASK) >> TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_RAMP_SET_DOWN_FREQ(val)    (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_DOWN_STEP(val)    (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_UP_FREQ(val)      (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_UP_STEP(val)      (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT)

/* DIGITAL VOLUME RAMP DOWN FREQUENCY values */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_4FS         0x00            /* Update every 4 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_1FS         0x01            /* Update every 1 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_32FS        0x02            /* Update every 32 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_INSTANT     0x03            /* Directly set the volume to zero (Instant mute) */

/* DIGITAL VOLUME RAMP DOWN STEP values */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_4DB         0x00            /* Decrement by 4 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_2DB         0x01            /* Decrement by 2 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_1DB         0x02            /* Decrement by 1 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_0_5DB       0x03            /* Decrement by 0.5 dB for each update */

/* DIGITAL VOLUME RAMP UP FREQUENCY values */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_4FS           0x00            /* Update every 4 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_16FS          0x01            /* Update every 16 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_32FS          0x02            /* Update every 32 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_INSTANT       0x03            /* Directly restore the volume (Instant unmute) */

/* DIGITAL VOLUME RAMP UP STEP values */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_4DB           0x00            /* Increment by 4 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_2DB           0x01            /* Increment by 2 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_1DB           0x02            /* Increment by 1 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_0_5DB         0x03            /* Increment by 0.5 dB for each update */

/* Common configurations */
#define TAS6754_DIG_VOL_RAMP_SLOW                  0x2A            /* Slow ramping: 32FS/1dB down, 16FS/1dB up */
#define TAS6754_DIG_VOL_RAMP_MEDIUM                0x55            /* Medium ramping: 1FS/2dB down, 16FS/2dB up */
#define TAS6754_DIG_VOL_RAMP_FAST                  0x00            /* Fast ramping: 4FS/4dB down, 4FS/4dB up */
#define TAS6754_DIG_VOL_RAMP_INSTANT               0xCF            /* Instant volume changes */

/* Reset value */
#define TAS6754_DIG_VOL_RAMP_CTRL_RESET            0x77            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DIG_VOL_COMBINE_CTRL (0x46)
************************************************************************/
/* Bit field masks */
#define TAS6754_DIG_VOL_COMBINE_RESERVED_MASK      GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_DIG_VOL_COMBINE_CH3_4_MASK         GENMASK(3, 2)   /* Bits 3-2: DIGITAL VOLUME COMBINE CH3/4 */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_MASK         GENMASK(1, 0)   /* Bits 1-0: DIGITAL VOLUME COMBINE CH1/2 */

/* Bit positions for shifting */
#define TAS6754_DIG_VOL_COMBINE_RESERVED_SHIFT     4
#define TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT        2
#define TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT        0

/* Helper macros for extracting values */
#define TAS6754_DIG_VOL_COMBINE_GET_CH3_4(val)     (((val) & TAS6754_DIG_VOL_COMBINE_CH3_4_MASK) >> TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_GET_CH1_2(val)     (((val) & TAS6754_DIG_VOL_COMBINE_CH1_2_MASK) >> TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT)

/* Helper macros for setting values */
#define TAS6754_DIG_VOL_COMBINE_SET_CH3_4(val)     (((val) & 0x03) << TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_SET_CH1_2(val)     (((val) & 0x03) << TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT)

/* DIGITAL VOLUME COMBINE CH3/4 values */
#define TAS6754_DIG_VOL_COMBINE_CH3_4_INDEPENDENT  0x00            /* The volume for Channel 3 and Channel 4 are independent */
#define TAS6754_DIG_VOL_COMBINE_CH4_FOLLOWS_CH3    0x01            /* Channel 4 volume follows Channel 3 setting */
#define TAS6754_DIG_VOL_COMBINE_CH3_FOLLOWS_CH4    0x02            /* Channel 3 volume follows Channel 4 setting */
#define TAS6754_DIG_VOL_COMBINE_CH3_4_RESERVED     0x03            /* Reserved */

/* DIGITAL VOLUME COMBINE CH1/2 values */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_INDEPENDENT  0x00            /* The volume for Channel 1 and Channel 2 are independent */
#define TAS6754_DIG_VOL_COMBINE_CH2_FOLLOWS_CH1    0x01            /* Channel 2 volume follows Channel 1 setting */
#define TAS6754_DIG_VOL_COMBINE_CH1_FOLLOWS_CH2    0x02            /* Channel 1 volume follows Channel 2 setting */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_RESERVED     0x03            /* Reserved */

/* Common configurations */
#define TAS6754_DIG_VOL_COMBINE_ALL_INDEPENDENT    0x00            /* All channels have independent volume control */
#define TAS6754_DIG_VOL_COMBINE_STEREO_PAIRS       0x05            /* CH2 follows CH1, CH4 follows CH3 (stereo pairs) */
#define TAS6754_DIG_VOL_COMBINE_LEFT_RIGHT         0x0A            /* CH1 follows CH2, CH3 follows CH4 (left/right pairs) */

/* Reset value */
#define TAS6754_DIG_VOL_COMBINE_CTRL_RESET         0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AUTO_MUTE_EN (0x47)
************************************************************************/
/* Bit field masks */
#define TAS6754_AUTO_MUTE_EN_RESERVED_MASK         GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_AUTO_MUTE_EN_COMBINE_MASK          BIT(4)          /* Bit 4: AUTO MUTE COMBINE */
#define TAS6754_AUTO_MUTE_EN_CH4_MASK              BIT(3)          /* Bit 3: CH4 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH3_MASK              BIT(2)          /* Bit 2: CH3 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH2_MASK              BIT(1)          /* Bit 1: CH2 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH1_MASK              BIT(0)          /* Bit 0: CH1 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_ALL_CH_MASK           GENMASK(3, 0)   /* Bits 3-0: All channel auto mute enables */

/* Bit positions for shifting */
#define TAS6754_AUTO_MUTE_EN_RESERVED_SHIFT        5
#define TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT         4
#define TAS6754_AUTO_MUTE_EN_CH4_SHIFT             3
#define TAS6754_AUTO_MUTE_EN_CH3_SHIFT             2
#define TAS6754_AUTO_MUTE_EN_CH2_SHIFT             1
#define TAS6754_AUTO_MUTE_EN_CH1_SHIFT             0

/* Helper macros for extracting values */
#define TAS6754_AUTO_MUTE_EN_GET_COMBINE(val)      (((val) & TAS6754_AUTO_MUTE_EN_COMBINE_MASK) >> TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH4(val)          (((val) & TAS6754_AUTO_MUTE_EN_CH4_MASK) >> TAS6754_AUTO_MUTE_EN_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH3(val)          (((val) & TAS6754_AUTO_MUTE_EN_CH3_MASK) >> TAS6754_AUTO_MUTE_EN_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH2(val)          (((val) & TAS6754_AUTO_MUTE_EN_CH2_MASK) >> TAS6754_AUTO_MUTE_EN_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH1(val)          (((val) & TAS6754_AUTO_MUTE_EN_CH1_MASK) >> TAS6754_AUTO_MUTE_EN_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_ALL_CH(val)       ((val) & TAS6754_AUTO_MUTE_EN_ALL_CH_MASK)

/* Helper macros for setting values */
#define TAS6754_AUTO_MUTE_EN_SET_COMBINE(val)      (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH4(val)          (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH3(val)          (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH2(val)          (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH1(val)          (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_ALL_CH(val)       ((val) & 0x0F)

/* AUTO MUTE COMBINE values */
#define TAS6754_AUTO_MUTE_EN_COMBINE_INDEPENDENT   0x00            /* Auto mute channel independently */
#define TAS6754_AUTO_MUTE_EN_COMBINE_ALL_CHANNELS  0x01            /* Auto mute channels only when all four channels are about to be auto muted */

/* Channel AUTO MUTE ENABLE values */
#define TAS6754_AUTO_MUTE_EN_CH_DISABLED           0x00            /* Disable Channel auto mute */
#define TAS6754_AUTO_MUTE_EN_CH_ENABLED            0x01            /* Enable Channel auto mute */

/* Common configurations */
#define TAS6754_AUTO_MUTE_EN_ALL_DISABLED          0x00            /* All channels auto mute disabled */
#define TAS6754_AUTO_MUTE_EN_ALL_ENABLED           0x0F            /* All channels auto mute enabled */
#define TAS6754_AUTO_MUTE_EN_ALL_ENABLED_COMBINED  0x1F            /* All channels auto mute enabled and combined */
#define TAS6754_AUTO_MUTE_EN_STEREO_PAIRS          0x05            /* Only CH1 and CH3 auto mute enabled (for stereo pairs) */

/* Reset value */
#define TAS6754_AUTO_MUTE_EN_RESET                 0x00            /* Reset value for the register */
/************************************************************************
*					TAS6754_AUTO_MUTE_TIMING_CH1_CH2 (0x48)
************************************************************************/
/* Bit field masks */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK   BIT(7)          /* Bit 7: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH1_MASK          GENMASK(6, 4)   /* Bits 6-4: CH1 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK   BIT(3)          /* Bit 3: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH2_MASK          GENMASK(2, 0)   /* Bits 2-0: CH2 AUTO MUTE TIMING */

/* Bit positions for shifting */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_SHIFT  7
#define TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT         4
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_SHIFT  3
#define TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT         0

/* Helper macros for extracting values */
#define TAS6754_AUTO_MUTE_TIMING_GET_CH1(val)      (((val) & TAS6754_AUTO_MUTE_TIMING_CH1_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH2(val)      (((val) & TAS6754_AUTO_MUTE_TIMING_CH2_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT)

/* Helper macros for setting values */
#define TAS6754_AUTO_MUTE_TIMING_SET_CH1(val)      (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH2(val)      (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_MASK     (TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK | \
                                                   TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK)

/* AUTO MUTE TIMING values (for 96 kHz sampling rate) */
#define TAS6754_AUTO_MUTE_TIMING_11_5MS            0x00            /* 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_53MS              0x01            /* 53 ms */
#define TAS6754_AUTO_MUTE_TIMING_106_5MS           0x02            /* 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_266_5MS           0x03            /* 266.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_535MS             0x04            /* 0.535 sec */
#define TAS6754_AUTO_MUTE_TIMING_1_065S            0x05            /* 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_2_665S            0x06            /* 2.665 sec */
#define TAS6754_AUTO_MUTE_TIMING_5_33S             0x07            /* 5.33 sec */

/* Common configurations */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_SHORT     0x00            /* Both channels: 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_MEDIUM    0x22            /* Both channels: 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_LONG      0x55            /* Both channels: 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_MAX       0x77            /* Both channels: 5.33 sec */

/* Reset value */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_RESET     0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AUTO_MUTE_TIMING_CH3_CH4 (0x49)
************************************************************************/
/* Bit field masks */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK   BIT(7)          /* Bit 7: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH3_MASK          GENMASK(6, 4)   /* Bits 6-4: CH3 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK   BIT(3)          /* Bit 3: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH4_MASK          GENMASK(2, 0)   /* Bits 2-0: CH4 AUTO MUTE TIMING */

/* Bit positions for shifting */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_SHIFT  7
#define TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT         4
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_SHIFT  3
#define TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT         0

/* Helper macros for extracting values */
#define TAS6754_AUTO_MUTE_TIMING_GET_CH3(val)      (((val) & TAS6754_AUTO_MUTE_TIMING_CH3_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH4(val)      (((val) & TAS6754_AUTO_MUTE_TIMING_CH4_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT)

/* Helper macros for setting values */
#define TAS6754_AUTO_MUTE_TIMING_SET_CH3(val)      (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH4(val)      (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_RESERVED_MASK (TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK | \
                                                       TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK)

/* AUTO MUTE TIMING values (for 96 kHz sampling rate) - reusing from CH1/CH2 */
/* Common configurations */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_SHORT     0x00            /* Both channels: 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_MEDIUM    0x22            /* Both channels: 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_LONG      0x55            /* Both channels: 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_MAX       0x77            /* Both channels: 5.33 sec */

/* Reset value */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_RESET     0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_ANALOG_GAIN_CH1_CH2 (0x4A)
************************************************************************/
/* Bit field masks */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED_MASK  GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_MASK      GENMASK(5, 0)   /* Bits 5-0: CH 1/2 ANALOG GAIN */

/* Bit positions for shifting */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED_SHIFT 6
#define TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT     0

/* Helper macros for extracting values */
#define TAS6754_ANALOG_GAIN_CH1_CH2_GET_GAIN(val)  (((val) & TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_MASK) >> TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT)

/* Helper macros for setting values */
#define TAS6754_ANALOG_GAIN_CH1_CH2_SET_GAIN(val)  (((val) & 0x3F) << TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT)

/* Special gain values */
#define TAS6754_ANALOG_GAIN_CH1_CH2_0DB            0x00            /* 0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_0_5DB    0x01            /* -0.5 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_1DB      0x02            /* -1.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_3DB      0x06            /* -3.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_6DB      0x0C            /* -6.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_9DB      0x12            /* -9.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_12DB     0x18            /* -12.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_15DB     0x1E            /* -15.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_15_5DB   0x1F            /* -15.5 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED       0x3F            /* Reserved value */

/* Helper macro to convert dB to register value (dB must be negative or zero) */
#define TAS6754_ANALOG_GAIN_CH1_CH2_DB_TO_REG(db)  ((db <= 0 && db > -15.5) ? (uint8_t)(-db * 2) : \
                                                   (db == -15.5) ? 0x1F : 0x00)

/* Helper macro to convert register value to dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_REG_TO_DB(reg) ((reg <= 0x1F) ? (-((float)(reg)) / 2.0) : 0.0)

/* Reset value */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESET          0x00            /* Reset value for the register (0.0 dB) */
/************************************************************************
* 					TAS6754_ANALOG_GAIN_CH3_CH4 (0x4B)
************************************************************************/
/* Bit field masks */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED_MASK  GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_MASK      GENMASK(5, 0)   /* Bits 5-0: CH 3/4 ANALOG GAIN */

/* Bit positions for shifting */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED_SHIFT 6
#define TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT     0

/* Helper macros for extracting values */
#define TAS6754_ANALOG_GAIN_CH3_CH4_GET_GAIN(val)  (((val) & TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_MASK) >> TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT)

/* Helper macros for setting values */
#define TAS6754_ANALOG_GAIN_CH3_CH4_SET_GAIN(val)  (((val) & 0x3F) << TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT)

/* Special gain values */
#define TAS6754_ANALOG_GAIN_CH3_CH4_0DB            0x00            /* 0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_0_5DB    0x01            /* -0.5 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_1DB      0x02            /* -1.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_3DB      0x06            /* -3.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_6DB      0x0C            /* -6.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_9DB      0x12            /* -9.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_12DB     0x18            /* -12.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_15DB     0x1E            /* -15.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_15_5DB   0x1F            /* -15.5 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED       0x3F            /* Reserved value */

/* Helper macro to convert dB to register value (dB must be negative or zero) */
#define TAS6754_ANALOG_GAIN_CH3_CH4_DB_TO_REG(db)  ((db <= 0 && db > -15.5) ? (uint8_t)(-db * 2) : \
                                                   (db == -15.5) ? 0x1F : 0x00)

/* Helper macro to convert register value to dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_REG_TO_DB(reg) ((reg <= 0x1F) ? (-((float)(reg)) / 2.0) : 0.0)

/* Reset value */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESET          0x00            /* Reset value for the register (0.0 dB) */
/************************************************************************
* 					TAS6754_ANALOG_GAIN_RAMP_CTRL (0x4E)
************************************************************************/
/* Bit field masks */
#define TAS6754_ANALOG_GAIN_RAMP_RESERVED_MASK     GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_MASK         GENMASK(3, 2)   /* Bits 3-2: ANALOG GAIN RAMP STEP */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_MASK     BIT(1)          /* Bit 1: ANALOG GAIN RAMP DOWN DISABLE */
#define TAS6754_ANALOG_GAIN_RAMP_UP_DIS_MASK       BIT(0)          /* Bit 0: ANALOG GAIN RAMP UP DISABLE */

/* Bit positions for shifting */
#define TAS6754_ANALOG_GAIN_RAMP_RESERVED_SHIFT    4
#define TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT        2
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT    1
#define TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT      0

/* Helper macros for extracting values */
#define TAS6754_ANALOG_GAIN_RAMP_GET_STEP(val)     (((val) & TAS6754_ANALOG_GAIN_RAMP_STEP_MASK) >> TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_GET_DOWN_DIS(val) (((val) & TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_MASK) >> TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_GET_UP_DIS(val)   (((val) & TAS6754_ANALOG_GAIN_RAMP_UP_DIS_MASK) >> TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT)

/* Helper macros for setting values */
#define TAS6754_ANALOG_GAIN_RAMP_SET_STEP(val)     (((val) & 0x03) << TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_SET_DOWN_DIS(val) (((val) & 0x01) << TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_SET_UP_DIS(val)   (((val) & 0x01) << TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT)

/* ANALOG GAIN RAMP STEP values */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_15US         0x00            /* 15us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_60US         0x01            /* 60us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_200US        0x02            /* 200us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_400US        0x03            /* 400us/step */

/* ANALOG GAIN RAMP DOWN DISABLE values */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_ENABLED      0x00            /* Enable Analog Gain ramp down */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DISABLED     0x01            /* Disable Analog Gain ramp down */

/* ANALOG GAIN RAMP UP DISABLE values */
#define TAS6754_ANALOG_GAIN_RAMP_UP_ENABLED        0x00            /* Enable Analog Gain ramp up */
#define TAS6754_ANALOG_GAIN_RAMP_UP_DISABLED       0x01            /* Disable Analog Gain ramp up */

/* Common configurations */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_ENABLED_FAST  0x00            /* Both ramps enabled, 15us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_ENABLED_SLOW  0x0C            /* Both ramps enabled, 400us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_DISABLED      0x03            /* Both ramps disabled */
#define TAS6754_ANALOG_GAIN_RAMP_ONLY_UP_MEDIUM    0x05            /* Only ramp up enabled, 60us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ONLY_DOWN_MEDIUM  0x09            /* Only ramp down enabled, 60us/step */

/* Reset value */
#define TAS6754_ANALOG_GAIN_RAMP_CTRL_RESET        0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PULSE_INJECTION_EN (0x52)
************************************************************************/
/* Bit field masks */
#define TAS6754_PULSE_INJECTION_EN_ENABLE_MASK     BIT(7)          /* Bit 7: PULSE INJECTION ENABLE */
#define TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_MASK GENMASK(6, 3) /* Bits 6-3: Reserved */
#define TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_MASK GENMASK(2, 0) /* Bits 2-0: Reserved */

/* Bit positions for shifting */
#define TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT    7
#define TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_SHIFT 3
#define TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_SHIFT 0

/* Helper macros for extracting values */
#define TAS6754_PULSE_INJECTION_EN_GET_ENABLE(val) (((val) & TAS6754_PULSE_INJECTION_EN_ENABLE_MASK) >> TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT)

/* Helper macros for setting values */
#define TAS6754_PULSE_INJECTION_EN_SET_ENABLE(val) (((val) & 0x01) << TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_PULSE_INJECTION_EN_RESERVED_MASK   (TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_MASK | \
                                                   TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_MASK)

/* PULSE INJECTION ENABLE values */
#define TAS6754_PULSE_INJECTION_DISABLED           0x00            /* Disable pulse injection */
#define TAS6754_PULSE_INJECTION_ENABLED            0x01            /* Enable pulse injection */

/* Reset value */
#define TAS6754_PULSE_INJECTION_EN_RESET           0x03            /* Reset value for the register */

/* Special note: When writing to this register, preserve the reserved bits 2-0 as 0x3 */
#define TAS6754_PULSE_INJECTION_EN_WRITE_MASK      0x87            /* Mask for writing (preserve reserved bits) */
#define TAS6754_PULSE_INJECTION_EN_WRITE_DISABLED  0x03            /* Value to write to disable pulse injection */
#define TAS6754_PULSE_INJECTION_EN_WRITE_ENABLED   0x83            /* Value to write to enable pulse injection */
/************************************************************************
* 					TAS6754_CBC_CTRL (0x54)
************************************************************************/
/* Bit field masks */
#define TAS6754_CBC_CTRL_RESERVED_7_4_MASK         GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_CBC_CTRL_RESERVED_3_MASK           BIT(3)          /* Bit 3: Reserved */
#define TAS6754_CBC_CTRL_RESERVED_2_MASK           BIT(2)          /* Bit 2: Reserved */
#define TAS6754_CBC_CTRL_FAULT_DISABLE_MASK        BIT(1)          /* Bit 1: CBC FAULT DISABLE */
#define TAS6754_CBC_CTRL_WARN_DISABLE_MASK         BIT(0)          /* Bit 0: CBC WARN DISABLE */

/* Bit positions for shifting */
#define TAS6754_CBC_CTRL_RESERVED_7_4_SHIFT        4
#define TAS6754_CBC_CTRL_RESERVED_3_SHIFT          3
#define TAS6754_CBC_CTRL_RESERVED_2_SHIFT          2
#define TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT       1
#define TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT        0

/* Helper macros for extracting values */
#define TAS6754_CBC_CTRL_GET_FAULT_DISABLE(val)    (((val) & TAS6754_CBC_CTRL_FAULT_DISABLE_MASK) >> TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_GET_WARN_DISABLE(val)     (((val) & TAS6754_CBC_CTRL_WARN_DISABLE_MASK) >> TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT)

/* Helper macros for setting values */
#define TAS6754_CBC_CTRL_SET_FAULT_DISABLE(val)    (((val) & 0x01) << TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_SET_WARN_DISABLE(val)     (((val) & 0x01) << TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_CBC_CTRL_RESERVED_MASK             (TAS6754_CBC_CTRL_RESERVED_7_4_MASK | \
                                                   TAS6754_CBC_CTRL_RESERVED_3_MASK | \
                                                   TAS6754_CBC_CTRL_RESERVED_2_MASK)

/* CBC FAULT DISABLE values (note: register naming is inverted from functionality) */
#define TAS6754_CBC_CTRL_FAULT_DETECTION_DISABLED  0x00            /* Disable CBC fault detection */
#define TAS6754_CBC_CTRL_FAULT_DETECTION_ENABLED   0x01            /* Enable CBC fault detection */

/* CBC WARN DISABLE values (note: register naming is inverted from functionality) */
#define TAS6754_CBC_CTRL_WARN_DETECTION_DISABLED   0x00            /* Disable CBC warning detection */
#define TAS6754_CBC_CTRL_WARN_DETECTION_ENABLED    0x01            /* Enable CBC warning detection */

/* Common configurations */
#define TAS6754_CBC_CTRL_ALL_DETECTION_DISABLED    0x00            /* Both fault and warning detection disabled */
#define TAS6754_CBC_CTRL_ALL_DETECTION_ENABLED     0x03            /* Both fault and warning detection enabled */
#define TAS6754_CBC_CTRL_ONLY_FAULT_ENABLED        0x02            /* Only fault detection enabled */
#define TAS6754_CBC_CTRL_ONLY_WARN_ENABLED         0x01            /* Only warning detection enabled */

/* Reset value */
#define TAS6754_CBC_CTRL_RESET                     0x07            /* Reset value for the register */

/* Special note: When writing to this register, preserve the reserved bit 2 as 1 */
#define TAS6754_CBC_CTRL_WRITE_MASK                0x07            /* Mask for writing (preserve reserved bits) */
/************************************************************************
* 					TAS6754_CURRENT_LIMIT_CTRL (0x55)
************************************************************************/
/* Bit field masks */
#define TAS6754_CURRENT_LIMIT_RESERVED_7_MASK      BIT(7)          /* Bit 7: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_6_5_MASK    GENMASK(6, 5)   /* Bits 6-5: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_4_3_MASK    GENMASK(4, 3)   /* Bits 4-3: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_2_MASK      BIT(2)          /* Bit 2: Reserved */
#define TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_MASK    GENMASK(1, 0)   /* Bits 1-0: CBC/OC LEVEL */

/* Bit positions for shifting */
#define TAS6754_CURRENT_LIMIT_RESERVED_7_SHIFT     7
#define TAS6754_CURRENT_LIMIT_RESERVED_6_5_SHIFT   5
#define TAS6754_CURRENT_LIMIT_RESERVED_4_3_SHIFT   3
#define TAS6754_CURRENT_LIMIT_RESERVED_2_SHIFT     2
#define TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT   0

/* Helper macros for extracting values */
#define TAS6754_CURRENT_LIMIT_GET_CBC_OC_LEVEL(val) (((val) & TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_MASK) >> TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT)

/* Helper macros for setting values */
#define TAS6754_CURRENT_LIMIT_SET_CBC_OC_LEVEL(val) (((val) & 0x03) << TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_CURRENT_LIMIT_RESERVED_MASK        (TAS6754_CURRENT_LIMIT_RESERVED_7_MASK | \
                                                   TAS6754_CURRENT_LIMIT_RESERVED_6_5_MASK | \
                                                   TAS6754_CURRENT_LIMIT_RESERVED_4_3_MASK | \
                                                   TAS6754_CURRENT_LIMIT_RESERVED_2_MASK)

/* CBC/OC LEVEL values */
#define TAS6754_CURRENT_LIMIT_LEVEL_4              0x00            /* Level 4 (lowest current limit) */
#define TAS6754_CURRENT_LIMIT_LEVEL_3              0x01            /* Level 3 */
#define TAS6754_CURRENT_LIMIT_LEVEL_2              0x02            /* Level 2 */
#define TAS6754_CURRENT_LIMIT_LEVEL_1              0x03            /* Level 1 (highest current limit) */

/* Reset value */
#define TAS6754_CURRENT_LIMIT_CTRL_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_ISENSE_CAL (0x5B)
************************************************************************/
/* Bit field masks */
#define TAS6754_ISENSE_CAL_RESERVED_7_4_MASK       GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_ISENSE_CAL_CALIBRATION_MASK        BIT(3)          /* Bit 3: ISENSE CALIBRATION */
#define TAS6754_ISENSE_CAL_RESERVED_2_0_MASK       GENMASK(2, 0)   /* Bits 2-0: Reserved */

/* Bit positions for shifting */
#define TAS6754_ISENSE_CAL_RESERVED_7_4_SHIFT      4
#define TAS6754_ISENSE_CAL_CALIBRATION_SHIFT       3
#define TAS6754_ISENSE_CAL_RESERVED_2_0_SHIFT      0

/* Helper macros for extracting values */
#define TAS6754_ISENSE_CAL_GET_CALIBRATION(val)    (((val) & TAS6754_ISENSE_CAL_CALIBRATION_MASK) >> TAS6754_ISENSE_CAL_CALIBRATION_SHIFT)

/* Helper macros for setting values */
#define TAS6754_ISENSE_CAL_SET_CALIBRATION(val)    (((val) & 0x01) << TAS6754_ISENSE_CAL_CALIBRATION_SHIFT)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_ISENSE_CAL_RESERVED_MASK           (TAS6754_ISENSE_CAL_RESERVED_7_4_MASK | \
                                                   TAS6754_ISENSE_CAL_RESERVED_2_0_MASK)

/* ISENSE CALIBRATION values */
#define TAS6754_ISENSE_CAL_DISABLED                0x00            /* Disable Isense Offset calibration logic */
#define TAS6754_ISENSE_CAL_ENABLED                 0x01            /* Enable Isense Offset calibration logic */

/* Reset value */
#define TAS6754_ISENSE_CAL_RESET                   0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PWM_PHASE_CTRL (0x60)
************************************************************************/
/* Bit field masks */
#define TAS6754_PWM_PHASE_MANUAL_MODE_ENABLE_MASK   BIT(7)          /* Bit 7: PWM PHASE MANUAL MODE ENABLE */
#define TAS6754_PWM_PHASE_SELECT_MASK               GENMASK(6, 4)   /* Bits 6-4: PWM PHASE SELECT */
#define TAS6754_PWM_PHASE_RESERVED_MASK             GENMASK(3, 2)   /* Bits 3-2: Reserved */
#define TAS6754_PWM_PHASE_SYNC_SELECT_MASK          BIT(1)          /* Bit 1: PWM PHASE SYNC SELECT */
#define TAS6754_PWM_PHASE_SYNC_ENABLE_MASK          BIT(0)          /* Bit 0: PWM PHASE SYNC ENABLE */

/* PWM PHASE MANUAL MODE ENABLE values (bit 7) */
#define TAS6754_PWM_PHASE_MANUAL_MODE_DISABLE       (0x00 << 7)  /* 0: Disable manual phase mode */
#define TAS6754_PWM_PHASE_MANUAL_MODE_ENABLE        (0x01 << 7)  /* 1: Enable manual phase mode */

/* PWM PHASE SELECT values (bits 6-4) */
#define TAS6754_PWM_PHASE_SELECT_MODE_0             (0x00 << 4)  /* 000: Ch1:0°, Ch2:180°, Ch3:90°, Ch4:270° */
#define TAS6754_PWM_PHASE_SELECT_MODE_1             (0x01 << 4)  /* 001: Ch1:0°, Ch2:45°, Ch3:90°, Ch4:135° */
/* Additional phase select modes can be defined here */

/* PWM PHASE SYNC SELECT values (bit 1) */
#define TAS6754_PWM_PHASE_SYNC_SELECT_GPIO          (0x00 << 1)  /* 0: GPIO sync */
#define TAS6754_PWM_PHASE_SYNC_SELECT_SCLK          (0x01 << 1)  /* 1: SCLK sync */

/* PWM PHASE SYNC ENABLE values (bit 0) */
#define TAS6754_PWM_PHASE_SYNC_DISABLE              (0x00 << 0)  /* 0: Disable ramp phase sync */
#define TAS6754_PWM_PHASE_SYNC_ENABLE               (0x01 << 0)  /* 1: Enable ramp phase sync */

/* Reset value */
#define TAS6754_PWM_PHASE_CTRL_RESET                0x00  /* Reset value for the register */
/************************************************************************
* 					TAS6754_SS_CTRL (0x61)
************************************************************************/
/* Bit field masks */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV_MASK       GENMASK(7, 6)   /* Bits 7-6: GPO RAMP CLK DIV */
#define TAS6754_SS_CTRL_RESERVED_5_MASK             BIT(5)          /* Bit 5: Reserved */
#define TAS6754_SS_CTRL_RESERVED_4_MASK             BIT(4)          /* Bit 4: Reserved */
#define TAS6754_SS_CTRL_RESERVED_3_MASK             BIT(3)          /* Bit 3: Reserved */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_SS_MASK BIT(2)          /* Bit 2: RDM PERIOD TRIANGLE SS ENABLE */
#define TAS6754_SS_CTRL_RANDOM_SS_MASK              BIT(1)          /* Bit 1: RANDOM SS ENABLE */
#define TAS6754_SS_CTRL_TRIANGLE_SS_MASK            BIT(0)          /* Bit 0: TRIANGLE SS ENABLE */

/* GPO RAMP CLK DIV values (bits 7-6) */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV1           (0x00 << 6)  /* 0: div1 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV2           (0x01 << 6)  /* 1: div2 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV4           (0x02 << 6)  /* 2: div4 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV8           (0x03 << 6)  /* 3: div8 */

/* RDM PERIOD TRIANGLE SS ENABLE values (bit 2) */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_NORMAL  (0x00 << 2)  /* 0: Normal triangle spread spectrum */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_RANDOM  (0x01 << 2)  /* 1: Triangle spread spectrum with random period */

/* RANDOM SS ENABLE values (bit 1) */
#define TAS6754_SS_CTRL_RANDOM_SS_DISABLE           (0x00 << 1)  /* 0: Disable random spread spectrum */
#define TAS6754_SS_CTRL_RANDOM_SS_ENABLE            (0x01 << 1)  /* 1: Enable random spread spectrum */

/* TRIANGLE SS ENABLE values (bit 0) */
#define TAS6754_SS_CTRL_TRIANGLE_SS_DISABLE         (0x00 << 0)  /* 0: Disable triangle spread spectrum */
#define TAS6754_SS_CTRL_TRIANGLE_SS_ENABLE          (0x01 << 0)  /* 1: Enable triangle spread spectrum */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_SS_CTRL_RESERVED_MASK               (TAS6754_SS_CTRL_RESERVED_5_MASK | \
                                                    TAS6754_SS_CTRL_RESERVED_4_MASK | \
                                                    TAS6754_SS_CTRL_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_SS_CTRL_RESET                       0x00  /* Reset value for the register */
/************************************************************************
* 					TAS6754_SS_RANGE_CTRL (0x62)
************************************************************************/
/* Bit field masks */
#define TAS6754_SS_RANGE_RESERVED_7_MASK            BIT(7)          /* Bit 7: Reserved */
#define TAS6754_SS_RANGE_RANDOM_SS_RANGE_MASK       GENMASK(6, 4)   /* Bits 6-4: RANDOM SS RANGE */
#define TAS6754_SS_RANGE_RDM_DWELL_CTRL_MASK        GENMASK(3, 2)   /* Bits 3-2: SS RDM DWELL CTRL */
#define TAS6754_SS_RANGE_TRIANGLE_SS_RANGE_MASK     GENMASK(1, 0)   /* Bits 1-0: TRIANGLE SS RANGE */

/* RANDOM SS RANGE values (bits 6-4) */
#define TAS6754_SS_RANGE_RANDOM_0_83_PERCENT        (0x00 << 4)  /* 000: ±0.83% */
#define TAS6754_SS_RANGE_RANDOM_2_50_PERCENT        (0x01 << 4)  /* 001: ±2.50% */
#define TAS6754_SS_RANGE_RANDOM_5_83_PERCENT        (0x02 << 4)  /* 010: ±5.83% */
#define TAS6754_SS_RANGE_RANDOM_12_50_PERCENT       (0x03 << 4)  /* 011: ±12.50% */
#define TAS6754_SS_RANGE_RANDOM_25_83_PERCENT       (0x04 << 4)  /* 100: ±25.83% */

/* SS RDM DWELL CTRL values (bits 3-2) */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_2           (0x00 << 2)  /* 00: Random triangle SS period from 1/FSS to 2/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_4           (0x01 << 2)  /* 01: Random triangle SS period from 1/FSS to 4/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_8           (0x02 << 2)  /* 10: Random triangle SS period from 1/FSS to 8/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_15          (0x03 << 2)  /* 11: Random triangle SS period from 1/FSS to 15/FSS */

/* TRIANGLE SS RANGE values (bits 1-0) */
#define TAS6754_SS_RANGE_TRIANGLE_6_5_PERCENT       (0x00 << 0)  /* 00: ±6.5% */
#define TAS6754_SS_RANGE_TRIANGLE_13_5_PERCENT      (0x01 << 0)  /* 01: ±13.5% */
#define TAS6754_SS_RANGE_TRIANGLE_5_PERCENT         (0x02 << 0)  /* 10: ±5% */
#define TAS6754_SS_RANGE_TRIANGLE_10_PERCENT        (0x03 << 0)  /* 11: ±10% */

/* Reset value */
#define TAS6754_SS_RANGE_CTRL_RESET                 0x00  /* Reset value for the register */
/************************************************************************
* 					TAS6754_SS_DWELL_CTRL (0x66)
************************************************************************/
/* Bit field masks */
#define TAS6754_SS_DWELL_MIN_MASK                   GENMASK(7, 4)   /* Bits 7-4: DWELL min control */
#define TAS6754_SS_DWELL_MAX_MASK                   GENMASK(3, 0)   /* Bits 3-0: DWELL max control */

/* Helper macros for setting DWELL values */
#define TAS6754_SS_DWELL_MIN_VAL(val)               ((val & 0x0F) << 4)  /* Set DWELL min value (1-15, 0 unavailable) */
#define TAS6754_SS_DWELL_MAX_VAL(val)               (val & 0x0F)         /* Set DWELL max value (1-15, 0 unavailable) */

/* Combined DWELL control value (for convenience) */
#define TAS6754_SS_DWELL_CTRL_VAL(min, max)         ((min << 4) | max)   /* Set both min and max values */

/* Reset value */
#define TAS6754_SS_DWELL_CTRL_RESET                 0x00  /* Reset value for the register */
/************************************************************************
* 					TAS6754_RAMP_PHASE_CTRL_GPO (0x68)
************************************************************************/
/* Bit field masks */
#define TAS6754_RAMP_PHASE_GPO_SYNC_MASK            0xFF            /* Bits 7-0: RAMP PHASE GPO SYNC */

/* Common phase values (for convenience) */
#define TAS6754_RAMP_PHASE_GPO_0_DEG                0x00            /* 0x00: 0 degrees */
#define TAS6754_RAMP_PHASE_GPO_45_DEG               0x20            /* 0x20: 45 degrees */
#define TAS6754_RAMP_PHASE_GPO_90_DEG               0x40            /* 0x40: 90 degrees */
#define TAS6754_RAMP_PHASE_GPO_135_DEG              0x60            /* 0x60: 135 degrees */
#define TAS6754_RAMP_PHASE_GPO_180_DEG              0x80            /* 0x80: 180 degrees */
#define TAS6754_RAMP_PHASE_GPO_225_DEG              0xA0            /* 0xA0: 225 degrees */
#define TAS6754_RAMP_PHASE_GPO_270_DEG              0xC0            /* 0xC0: 270 degrees */
#define TAS6754_RAMP_PHASE_GPO_315_DEG              0xE0            /* 0xE0: 315 degrees */

/* Helper macro for calculating phase values */
#define TAS6754_RAMP_PHASE_GPO_DEG(deg)             ((uint8_t)((deg * 256) / 360))

/* Reset value */
#define TAS6754_RAMP_PHASE_CTRL_GPO_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH1 (0x69)
************************************************************************/
/* Bit field masks */
#define TAS6754_PWM_PHASE_M_CTRL_CH1_MASK           0xFF            /* Bits 7-0: CH1 PWM PHASE MANUAL CTRL */

/* Common phase values (for convenience) */
#define TAS6754_PWM_PHASE_CH1_0_DEG                 0x00            /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH1_45_DEG                0x20            /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH1_90_DEG                0x40            /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH1_135_DEG               0x60            /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH1_180_DEG               0x80            /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH1_225_DEG               0xA0            /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH1_270_DEG               0xC0            /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH1_315_DEG               0xE0            /* 0xE0: 315 degrees */

/* Helper macro for calculating phase values */
#define TAS6754_PWM_PHASE_CH1_DEG(deg)              ((uint8_t)((deg * 256) / 360))

/* Reset value */
#define TAS6754_PWM_PHASE_M_CTRL_CH1_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH2 (0x6A)
************************************************************************/
/* Bit field masks */
#define TAS6754_PWM_PHASE_M_CTRL_CH2_MASK           0xFF            /* Bits 7-0: CH2 PWM PHASE MANUAL CTRL */

/* Common phase values (for convenience) */
#define TAS6754_PWM_PHASE_CH2_0_DEG                 0x00            /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH2_45_DEG                0x20            /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH2_90_DEG                0x40            /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH2_135_DEG               0x60            /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH2_180_DEG               0x80            /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH2_225_DEG               0xA0            /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH2_270_DEG               0xC0            /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH2_315_DEG               0xE0            /* 0xE0: 315 degrees */

/* Helper macro for calculating phase values */
#define TAS6754_PWM_PHASE_CH2_DEG(deg)              ((uint8_t)((deg * 256) / 360))

/* Reset value */
#define TAS6754_PWM_PHASE_M_CTRL_CH2_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH3 (0x6B)
************************************************************************/
/* Bit field masks */
#define TAS6754_PWM_PHASE_M_CTRL_CH3_MASK           0xFF            /* Bits 7-0: CH3 PWM PHASE MANUAL CTRL */

/* Common phase values (for convenience) */
#define TAS6754_PWM_PHASE_CH3_0_DEG                 0x00            /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH3_45_DEG                0x20            /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH3_90_DEG                0x40            /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH3_135_DEG               0x60            /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH3_180_DEG               0x80            /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH3_225_DEG               0xA0            /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH3_270_DEG               0xC0            /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH3_315_DEG               0xE0            /* 0xE0: 315 degrees */

/* Helper macro for calculating phase values */
#define TAS6754_PWM_PHASE_CH3_DEG(deg)              ((uint8_t)((deg * 256) / 360))

/* Reset value */
#define TAS6754_PWM_PHASE_M_CTRL_CH3_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH4 (0x6C)
************************************************************************/
/* Bit field masks */
#define TAS6754_PWM_PHASE_M_CTRL_CH4_MASK           0xFF            /* Bits 7-0: CH4 PWM PHASE MANUAL CTRL */

/* Common phase values (for convenience) */
#define TAS6754_PWM_PHASE_CH4_0_DEG                 0x00            /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH4_45_DEG                0x20            /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH4_90_DEG                0x40            /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH4_135_DEG               0x60            /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH4_180_DEG               0x80            /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH4_225_DEG               0xA0            /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH4_270_DEG               0xC0            /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH4_315_DEG               0xE0            /* 0xE0: 315 degrees */

/* Helper macro for calculating phase values */
#define TAS6754_PWM_PHASE_CH4_DEG(deg)              ((uint8_t)((deg * 256) / 360))

/* Reset value */
#define TAS6754_PWM_PHASE_M_CTRL_CH4_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AUTO_MUTE_STATUS (0x71)
************************************************************************/
/* Bit field masks */
#define TAS6754_AUTO_MUTE_STATUS_RESERVED_MASK      GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_AUTO_MUTE_STATUS_CH1_MASK           BIT(3)          /* Bit 3: CH1 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH2_MASK           BIT(2)          /* Bit 2: CH2 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH3_MASK           BIT(1)          /* Bit 1: CH3 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH4_MASK           BIT(0)          /* Bit 0: CH4 AM STATUS */

/* Status values for each channel */
#define TAS6754_AUTO_MUTE_STATUS_CH1_NOT_MUTED      (0x00 << 3)  /* 0: Channel 1 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH1_MUTED          (0x01 << 3)  /* 1: Channel 1 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH2_NOT_MUTED      (0x00 << 2)  /* 0: Channel 2 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH2_MUTED          (0x01 << 2)  /* 1: Channel 2 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH3_NOT_MUTED      (0x00 << 1)  /* 0: Channel 3 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH3_MUTED          (0x01 << 1)  /* 1: Channel 3 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH4_NOT_MUTED      (0x00 << 0)  /* 0: Channel 4 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH4_MUTED          (0x01 << 0)  /* 1: Channel 4 auto muted */

/* Combined channel masks (for convenience) */
#define TAS6754_AUTO_MUTE_STATUS_ALL_CHANNELS_MASK  GENMASK(3, 0)   /* Mask for all channel status bits */

/* Reset value */
#define TAS6754_AUTO_MUTE_STATUS_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_STATE_REPORT_CH1_CH2 (0x72)
************************************************************************/
/* Bit field masks */
#define TAS6754_STATE_REPORT_CH1_MASK               GENMASK(7, 4)   /* Bits 7-4: CH1 STATUS */
#define TAS6754_STATE_REPORT_CH2_MASK               GENMASK(3, 0)   /* Bits 3-0: CH2 STATUS */

/* Channel status values - CH1 (bits 7-4) */
#define TAS6754_STATE_CH1_DEEPSLEEP                 (0x00 << 4)  /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH1_LOAD_DIAG                 (0x01 << 4)  /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH1_SLEEP                     (0x02 << 4)  /* 0010: SLEEP */
#define TAS6754_STATE_CH1_HIZ                       (0x03 << 4)  /* 0011: HI-Z */
#define TAS6754_STATE_CH1_PLAY                      (0x04 << 4)  /* 0100: PLAY */
#define TAS6754_STATE_CH1_FAULT                     (0x05 << 4)  /* 0101: FAULT */
#define TAS6754_STATE_CH1_AUTOREC                   (0x06 << 4)  /* 0110: AUTOREC */

/* Channel status values - CH2 (bits 3-0) */
#define TAS6754_STATE_CH2_DEEPSLEEP                 (0x00 << 0)  /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH2_LOAD_DIAG                 (0x01 << 0)  /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH2_SLEEP                     (0x02 << 0)  /* 0010: SLEEP */
#define TAS6754_STATE_CH2_HIZ                       (0x03 << 0)  /* 0011: HI-Z */
#define TAS6754_STATE_CH2_PLAY                      (0x04 << 0)  /* 0100: PLAY */
#define TAS6754_STATE_CH2_FAULT                     (0x05 << 0)  /* 0101: FAULT */
#define TAS6754_STATE_CH2_AUTOREC                   (0x06 << 0)  /* 0110: AUTOREC */

/* Helper macros to extract channel status */
#define TAS6754_GET_CH1_STATE(reg_val)              ((reg_val & TAS6754_STATE_REPORT_CH1_MASK) >> 4)
#define TAS6754_GET_CH2_STATE(reg_val)              (reg_val & TAS6754_STATE_REPORT_CH2_MASK)

/* Reset value */
#define TAS6754_STATE_REPORT_CH1_CH2_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_STATE_REPORT_CH3_CH4 (0x73)
************************************************************************/
/* Bit field masks */
#define TAS6754_STATE_REPORT_CH3_MASK               GENMASK(7, 4)   /* Bits 7-4: CH3 STATUS */
#define TAS6754_STATE_REPORT_CH4_MASK               GENMASK(3, 0)   /* Bits 3-0: CH4 STATUS */

/* Channel status values - CH3 (bits 7-4) */
#define TAS6754_STATE_CH3_DEEPSLEEP                 (0x00 << 4)  /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH3_LOAD_DIAG                 (0x01 << 4)  /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH3_SLEEP                     (0x02 << 4)  /* 0010: SLEEP */
#define TAS6754_STATE_CH3_HIZ                       (0x03 << 4)  /* 0011: HI-Z */
#define TAS6754_STATE_CH3_PLAY                      (0x04 << 4)  /* 0100: PLAY */
#define TAS6754_STATE_CH3_FAULT                     (0x05 << 4)  /* 0101: FAULT */
#define TAS6754_STATE_CH3_AUTOREC                   (0x06 << 4)  /* 0110: AUTOREC */

/* Channel status values - CH4 (bits 3-0) */
#define TAS6754_STATE_CH4_DEEPSLEEP                 (0x00 << 0)  /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH4_LOAD_DIAG                 (0x01 << 0)  /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH4_SLEEP                     (0x02 << 0)  /* 0010: SLEEP */
#define TAS6754_STATE_CH4_HIZ                       (0x03 << 0)  /* 0011: HI-Z */
#define TAS6754_STATE_CH4_PLAY                      (0x04 << 0)  /* 0100: PLAY */
#define TAS6754_STATE_CH4_FAULT                     (0x05 << 0)  /* 0101: FAULT */
#define TAS6754_STATE_CH4_AUTOREC                   (0x06 << 0)  /* 0110: AUTOREC */

/* Helper macros to extract channel status */
#define TAS6754_GET_CH3_STATE(reg_val)              ((reg_val & TAS6754_STATE_REPORT_CH3_MASK) >> 4)
#define TAS6754_GET_CH4_STATE(reg_val)              (reg_val & TAS6754_STATE_REPORT_CH4_MASK)

/* Reset value */
#define TAS6754_STATE_REPORT_CH3_CH4_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_PVDD_SENSE (0x74)
************************************************************************/
/* Bit field masks */
#define TAS6754_PVDD_SENSE_MASK                     0xFF            /* Bits 7-0: PVDD SENSE */

/* Helper macros for PVDD voltage calculation */
#define TAS6754_PVDD_VOLTAGE_SCALE                  0.19            /* Voltage scale factor: 0.19V per bit */
#define TAS6754_PVDD_VOLTAGE_TO_REG(volt)           ((uint8_t)((volt) / TAS6754_PVDD_VOLTAGE_SCALE))
#define TAS6754_REG_TO_PVDD_VOLTAGE(reg_val)        ((float)(reg_val) * TAS6754_PVDD_VOLTAGE_SCALE)

/* Common PVDD voltage values (for convenience) */
#define TAS6754_PVDD_SENSE_0V                       0x00            /* 0000 0000: 0V */
#define TAS6754_PVDD_SENSE_5V                       0x1A            /* 0001 1010: ~5V (actually 4.94V) */
#define TAS6754_PVDD_SENSE_12V                      0x40            /* 0100 0000: ~12V (actually 12.16V) */
#define TAS6754_PVDD_SENSE_14_44V                   0x4C            /* 0100 1100: 14.44V */
#define TAS6754_PVDD_SENSE_24V                      0x7F            /* 0111 1111: ~24V (actually 24.13V) */
#define TAS6754_PVDD_SENSE_48V                      0xFF            /* 1111 1111: ~48V (actually 48.45V) */

/* Reset value */
#define TAS6754_PVDD_SENSE_RESET                    0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_TEMP_GLOBAL (0x75)
************************************************************************/
/* Bit field masks */
#define TAS6754_TEMP_GLOBAL_MASK                    0xFF            /* Bits 7-0: GLOBAL TEMP SENSOR */

/* Helper macros for temperature calculation */
#define TAS6754_TEMP_KELVIN_SCALE                   2.19            /* Temperature scale factor: 2.19K per bit */
#define TAS6754_KELVIN_TO_CELSIUS_OFFSET            273.15          /* Offset to convert Kelvin to Celsius */

/* Temperature conversion macros */
#define TAS6754_REG_TO_TEMP_KELVIN(reg_val)         ((float)(reg_val) * TAS6754_TEMP_KELVIN_SCALE)
#define TAS6754_REG_TO_TEMP_CELSIUS(reg_val)        (TAS6754_REG_TO_TEMP_KELVIN(reg_val) - TAS6754_KELVIN_TO_CELSIUS_OFFSET)
#define TAS6754_TEMP_CELSIUS_TO_REG(celsius)        ((uint8_t)(((celsius) + TAS6754_KELVIN_TO_CELSIUS_OFFSET) / TAS6754_TEMP_KELVIN_SCALE))

/* Common temperature values (for convenience) */
#define TAS6754_TEMP_0C                             0x7D            /* ~0°C (actually 0.02°C) */
#define TAS6754_TEMP_25C                            0x8A            /* ~25°C (actually 25.06°C) */
#define TAS6754_TEMP_50C                            0x96            /* ~50°C (actually 50.09°C) */
#define TAS6754_TEMP_75C                            0xA3            /* ~75°C (actually 75.12°C) */
#define TAS6754_TEMP_100C                           0xAF            /* ~100°C (actually 100.16°C) */
#define TAS6754_TEMP_125C                           0xBC            /* ~125°C (actually 125.19°C) */

/* Reset value */
#define TAS6754_TEMP_GLOBAL_RESET                   0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_FS_MON (0x76)
************************************************************************/
/* Bit field masks */
#define TAS6754_FS_MON_RESERVED_MASK                GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_FS_MON_SCLK_RATIO_MSB_MASK          GENMASK(5, 4)   /* Bits 5-4: SCLK RATIO MSB */
#define TAS6754_FS_MON_DETECTED_SAMPLE_RATE_MASK    GENMASK(3, 0)   /* Bits 3-0: DETECTED SAMPLE RATE */

/* SCLK RATIO MSB values (bits 5-4) - to be used with SCLK Monitor register (0x77) */
#define TAS6754_FS_MON_SCLK_RATIO_MSB_SHIFT         4               /* Shift value for MSB bits */

/* DETECTED SAMPLE RATE values (bits 3-0) */
#define TAS6754_FS_MON_SAMPLE_RATE_ERROR            (0x00 << 0)  /* 0000: FS Error */
#define TAS6754_FS_MON_SAMPLE_RATE_48KHZ            (0x09 << 0)  /* 1001: 48KHz */
#define TAS6754_FS_MON_SAMPLE_RATE_96KHZ            (0x0B << 0)  /* 1011: 96KHz */
#define TAS6754_FS_MON_SAMPLE_RATE_192KHZ           (0x0D << 0)  /* 1101: 192KHz */

/* Helper macro to extract detected sample rate */
#define TAS6754_GET_DETECTED_SAMPLE_RATE(reg_val)   (reg_val & TAS6754_FS_MON_DETECTED_SAMPLE_RATE_MASK)

/* Helper macro to extract SCLK ratio MSB */
#define TAS6754_GET_SCLK_RATIO_MSB(reg_val)         ((reg_val & TAS6754_FS_MON_SCLK_RATIO_MSB_MASK) >> TAS6754_FS_MON_SCLK_RATIO_MSB_SHIFT)

/* Reset value */
#define TAS6754_FS_MON_RESET                        0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_SCLK_MON (0x77)
************************************************************************/
/* Bit field masks */
#define TAS6754_SCLK_MON_RATIO_LSB_MASK             0xFF            /* Bits 7-0: SCLK RATIO LSB */

/* Common SCLK ratio values (LSB part only, for convenience) */
#define TAS6754_SCLK_RATIO_32FS_LSB                 0x20            /* LSB part of 32Fs ratio */
#define TAS6754_SCLK_RATIO_64FS_LSB                 0x40            /* LSB part of 64Fs ratio */
#define TAS6754_SCLK_RATIO_128FS_LSB                0x80            /* LSB part of 128Fs ratio */
#define TAS6754_SCLK_RATIO_256FS_LSB                0x00            /* LSB part of 256Fs ratio (with MSB=01) */
#define TAS6754_SCLK_RATIO_512FS_LSB                0x00            /* LSB part of 512Fs ratio (with MSB=10) */

/* Helper macros for SCLK ratio calculation */
#define TAS6754_GET_SCLK_RATIO(msb_reg_val, lsb_reg_val) \
    ((TAS6754_GET_SCLK_RATIO_MSB(msb_reg_val) << 8) | (lsb_reg_val))

/* Common complete SCLK ratio values (requires both MSB and LSB) */
#define TAS6754_SCLK_RATIO_32FS                     0x020           /* 00 0010 0000: 32Fs */
#define TAS6754_SCLK_RATIO_64FS                     0x040           /* 00 0100 0000: 64Fs */
#define TAS6754_SCLK_RATIO_128FS                    0x080           /* 00 1000 0000: 128Fs */
#define TAS6754_SCLK_RATIO_256FS                    0x100           /* 01 0000 0000: 256Fs */
#define TAS6754_SCLK_RATIO_512FS                    0x200           /* 10 0000 0000: 512Fs */

/* Reset value */
#define TAS6754_SCLK_MON_RESET                      0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_1 (0x7C)
************************************************************************/
/* Bit field masks */
#define TAS6754_REPORT_ROUTING_1_RESERVED_7_MASK    BIT(7)          /* Bit 7: Reserved */
#define TAS6754_REPORT_ROUTING_1_OUTM_STORED_MASK   BIT(6)          /* Bit 6: OUTM STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_CP_UV_STORED_MASK  BIT(5)          /* Bit 5: CP UV STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_CP_STORED_MASK     BIT(4)          /* Bit 4: CP STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_RESERVED_3_MASK    BIT(3)          /* Bit 3: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_2_MASK    BIT(2)          /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_1_MASK    BIT(1)          /* Bit 1: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_0_MASK    BIT(0)          /* Bit 0: Reserved */

/* OUTM STORED GPIO values (bit 6) */
#define TAS6754_REPORT_ROUTING_1_OUTM_NOT_REPORTED  (0x00 << 6)  /* 0: Latched OUTM soft short fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_OUTM_REPORTED      (0x01 << 6)  /* 1: Latched OUTM soft short fault is reported to FAULT */

/* CP UV STORED GPIO values (bit 5) */
#define TAS6754_REPORT_ROUTING_1_CP_UV_NOT_REPORTED (0x00 << 5)  /* 0: Latched charge pump UVLO fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_UV_REPORTED     (0x01 << 5)  /* 1: Latched charge pump UVLO fault is reported to FAULT */

/* CP STORED GPIO values (bit 4) */
#define TAS6754_REPORT_ROUTING_1_CP_NOT_REPORTED    (0x00 << 4)  /* 0: Latched charge pump fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_REPORTED        (0x01 << 4)  /* 1: Latched charge pump fault is reported to FAULT */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_REPORT_ROUTING_1_RESERVED_MASK      (TAS6754_REPORT_ROUTING_1_RESERVED_7_MASK | \
                                                    TAS6754_REPORT_ROUTING_1_RESERVED_3_MASK | \
                                                    TAS6754_REPORT_ROUTING_1_RESERVED_2_MASK | \
                                                    TAS6754_REPORT_ROUTING_1_RESERVED_1_MASK | \
                                                    TAS6754_REPORT_ROUTING_1_RESERVED_0_MASK)

/* Reset value */
#define TAS6754_REPORT_ROUTING_1_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_POWER_FAULT_STATUS_1 (0x7D)
************************************************************************/
/* Bit field masks */
#define TAS6754_POWER_FAULT_1_RESERVED_MASK         GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_POWER_FAULT_1_CH4_SOFT_SHORT_MASK   BIT(5)          /* Bit 5: CH4 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH3_SOFT_SHORT_MASK   BIT(4)          /* Bit 4: CH3 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH2_SOFT_SHORT_MASK   BIT(3)          /* Bit 3: CH2 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH1_SOFT_SHORT_MASK   BIT(2)          /* Bit 2: CH1 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CP_UVLO_STORED_MASK   BIT(1)          /* Bit 1: CP UVLO FAULT STORED */
#define TAS6754_POWER_FAULT_1_CP_UVLO_MASK          BIT(0)          /* Bit 0: CP UVLO FAULT */

/* Channel soft short status values */
#define TAS6754_POWER_FAULT_1_CH4_NO_SHORT          (0x00 << 5)  /* 0: No Channel 4 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH4_SHORT_STORED      (0x01 << 5)  /* 1: Channel 4 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH3_NO_SHORT          (0x00 << 4)  /* 0: No Channel 3 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH3_SHORT_STORED      (0x01 << 4)  /* 1: Channel 3 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH2_NO_SHORT          (0x00 << 3)  /* 0: No Channel 2 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH2_SHORT_STORED      (0x01 << 3)  /* 1: Channel 2 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH1_NO_SHORT          (0x00 << 2)  /* 0: No Channel 1 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH1_SHORT_STORED      (0x01 << 2)  /* 1: Channel 1 OUTM soft short fault event stored */

/* CP UVLO fault status values */
#define TAS6754_POWER_FAULT_1_CP_UVLO_NO_STORED     (0x00 << 1)  /* 0: No charge pump UVLO fault event stored */
#define TAS6754_POWER_FAULT_1_CP_UVLO_STORED        (0x01 << 1)  /* 1: Charge pump UVLO fault event stored */
#define TAS6754_POWER_FAULT_1_CP_UVLO_NO_FAULT      (0x00 << 0)  /* 0: No charge pump UVLO fault */
#define TAS6754_POWER_FAULT_1_CP_UVLO_FAULT         (0x01 << 0)  /* 1: Charge pump UVLO fault happens */

/* Combined channel soft short mask (for convenience) */
#define TAS6754_POWER_FAULT_1_ALL_CH_SHORT_MASK     (TAS6754_POWER_FAULT_1_CH4_SOFT_SHORT_MASK | \
                                                    TAS6754_POWER_FAULT_1_CH3_SOFT_SHORT_MASK | \
                                                    TAS6754_POWER_FAULT_1_CH2_SOFT_SHORT_MASK | \
                                                    TAS6754_POWER_FAULT_1_CH1_SOFT_SHORT_MASK)

/* Reset value */
#define TAS6754_POWER_FAULT_STATUS_1_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_POWER_FAULT_STATUS_2 (0x80)
************************************************************************/
/* Bit field masks */
#define TAS6754_POWER_FAULT_2_GLOBAL_WARNING_MASK   BIT(7)          /* Bit 7: GLOBAL WARNING STATUS */
#define TAS6754_POWER_FAULT_2_GLOBAL_FAULT_MASK     BIT(6)          /* Bit 6: GLOBAL FAULT STATUS */
#define TAS6754_POWER_FAULT_2_RESERVED_MASK         BIT(5)          /* Bit 5: Reserved */
#define TAS6754_POWER_FAULT_2_DVDD_UV_MASK          BIT(4)          /* Bit 4: DVDD UV STATUS */
#define TAS6754_POWER_FAULT_2_PVDD_OV_MASK          BIT(3)          /* Bit 3: PVDD OV STATUS */
#define TAS6754_POWER_FAULT_2_VBAT_OV_MASK          BIT(2)          /* Bit 2: VBAT OV STATUS */
#define TAS6754_POWER_FAULT_2_PVDD_UV_MASK          BIT(1)          /* Bit 1: PVDD UV STATUS */
#define TAS6754_POWER_FAULT_2_VBAT_UV_MASK          BIT(0)          /* Bit 0: VBAT UV STATUS */

/* Global status values */
#define TAS6754_POWER_FAULT_2_NO_WARNING            (0x00 << 7)  /* 0: No warning */
#define TAS6754_POWER_FAULT_2_WARNING_ACTIVE        (0x01 << 7)  /* 1: Warning active in device */
#define TAS6754_POWER_FAULT_2_NO_FAULT              (0x00 << 6)  /* 0: No fault */
#define TAS6754_POWER_FAULT_2_FAULT_ACTIVE          (0x01 << 6)  /* 1: Fault active in device */

/* Supply voltage status values */
#define TAS6754_POWER_FAULT_2_DVDD_UV_NORMAL        (0x00 << 4)  /* 0: DVDD supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_DVDD_UV_FAULT         (0x01 << 4)  /* 1: DVDD supply voltage is below UV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_OV_NORMAL        (0x00 << 3)  /* 0: PVDD supply voltage is below OV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_OV_FAULT         (0x01 << 3)  /* 1: PVDD supply voltage is above OV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_OV_NORMAL        (0x00 << 2)  /* 0: VBAT supply voltage is below OV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_OV_FAULT         (0x01 << 2)  /* 1: VBAT supply voltage is above OV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_UV_NORMAL        (0x00 << 1)  /* 0: PVDD supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_UV_FAULT         (0x01 << 1)  /* 1: PVDD supply voltage is below UV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_UV_NORMAL        (0x00 << 0)  /* 0: VBAT supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_UV_FAULT         (0x01 << 0)  /* 1: VBAT supply voltage is below UV threshold */

/* Combined supply voltage fault mask (for convenience) */
#define TAS6754_POWER_FAULT_2_ALL_SUPPLY_MASK       (TAS6754_POWER_FAULT_2_DVDD_UV_MASK | \
                                                    TAS6754_POWER_FAULT_2_PVDD_OV_MASK | \
                                                    TAS6754_POWER_FAULT_2_VBAT_OV_MASK | \
                                                    TAS6754_POWER_FAULT_2_PVDD_UV_MASK | \
                                                    TAS6754_POWER_FAULT_2_VBAT_UV_MASK)

/* Reset value */
#define TAS6754_POWER_FAULT_STATUS_2_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OT_FAULT (0x81)
************************************************************************/
/* Bit field masks */
#define TAS6754_OT_FAULT_GLOBAL_WARNING_MASK        BIT(7)          /* Bit 7: GLOBAL WARNING */
#define TAS6754_OT_FAULT_GLOBAL_FAULT_MASK          BIT(6)          /* Bit 6: GLOBAL FAULT */
#define TAS6754_OT_FAULT_CP_FAULT_MASK              BIT(5)          /* Bit 5: CP FAULT STATUS */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_MASK           BIT(4)          /* Bit 4: GLOBAL OTSD STATUS */
#define TAS6754_OT_FAULT_CH1_OTSD_MASK              BIT(3)          /* Bit 3: CH1 OTSD STATUS */
#define TAS6754_OT_FAULT_CH2_OTSD_MASK              BIT(2)          /* Bit 2: CH2 OTSD STATUS */
#define TAS6754_OT_FAULT_CH3_OTSD_MASK              BIT(1)          /* Bit 1: CH3 OTSD STATUS */
#define TAS6754_OT_FAULT_CH4_OTSD_MASK              BIT(0)          /* Bit 0: CH4 OTSD STATUS */

/* Global status values */
#define TAS6754_OT_FAULT_NO_WARNING                 (0x00 << 7)  /* 0: No warning */
#define TAS6754_OT_FAULT_WARNING_TRIGGERED          (0x01 << 7)  /* 1: Any warning triggered */
#define TAS6754_OT_FAULT_NO_FAULT                   (0x00 << 6)  /* 0: No fault */
#define TAS6754_OT_FAULT_FAULT_TRIGGERED            (0x01 << 6)  /* 1: Any fault triggered */

/* CP fault status values */
#define TAS6754_OT_FAULT_CP_NORMAL                  (0x00 << 5)  /* 0: No charge pump fault detected */
#define TAS6754_OT_FAULT_CP_FAULT                   (0x01 << 5)  /* 1: Charge pump fault detected */

/* OTSD status values */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_NORMAL         (0x00 << 4)  /* 0: Global die temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_FAULT          (0x01 << 4)  /* 1: Global die temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH1_OTSD_NORMAL            (0x00 << 3)  /* 0: Channel 1 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH1_OTSD_FAULT             (0x01 << 3)  /* 1: Channel 1 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH2_OTSD_NORMAL            (0x00 << 2)  /* 0: Channel 2 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH2_OTSD_FAULT             (0x01 << 2)  /* 1: Channel 2 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH3_OTSD_NORMAL            (0x00 << 1)  /* 0: Channel 3 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH3_OTSD_FAULT             (0x01 << 1)  /* 1: Channel 3 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH4_OTSD_NORMAL            (0x00 << 0)  /* 0: Channel 4 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH4_OTSD_FAULT             (0x01 << 0)  /* 1: Channel 4 temperature is above OTSD threshold */

/* Combined channel OTSD mask (for convenience) */
#define TAS6754_OT_FAULT_ALL_CH_OTSD_MASK           (TAS6754_OT_FAULT_CH1_OTSD_MASK | \
                                                    TAS6754_OT_FAULT_CH2_OTSD_MASK | \
                                                    TAS6754_OT_FAULT_CH3_OTSD_MASK | \
                                                    TAS6754_OT_FAULT_CH4_OTSD_MASK)

/* Reset value */
#define TAS6754_OT_FAULT_RESET                      0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OTW_STATUS (0x82)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTW_STATUS_RESERVED_MASK            GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_OTW_STATUS_GLOBAL_OTW_MASK          BIT(4)          /* Bit 4: GLOBAL OTW STATUS */
#define TAS6754_OTW_STATUS_CH1_OTW_MASK             BIT(3)          /* Bit 3: CH1 OTW STATUS */
#define TAS6754_OTW_STATUS_CH2_OTW_MASK             BIT(2)          /* Bit 2: CH2 OTW STATUS */
#define TAS6754_OTW_STATUS_CH3_OTW_MASK             BIT(1)          /* Bit 1: CH3 OTW STATUS */
#define TAS6754_OTW_STATUS_CH4_OTW_MASK             BIT(0)          /* Bit 0: CH4 OTW STATUS */

/* OTW status values */
#define TAS6754_OTW_STATUS_GLOBAL_NORMAL            (0x00 << 4)  /* 0: Global die temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_GLOBAL_WARNING           (0x01 << 4)  /* 1: Global die temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH1_NORMAL               (0x00 << 3)  /* 0: Channel 1 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH1_WARNING              (0x01 << 3)  /* 1: Channel 1 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH2_NORMAL               (0x00 << 2)  /* 0: Channel 2 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH2_WARNING              (0x01 << 2)  /* 1: Channel 2 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH3_NORMAL               (0x00 << 1)  /* 0: Channel 3 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH3_WARNING              (0x01 << 1)  /* 1: Channel 3 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH4_NORMAL               (0x00 << 0)  /* 0: Channel 4 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH4_WARNING              (0x01 << 0)  /* 1: Channel 4 temperature is above OTW threshold */

/* Combined channel OTW mask (for convenience) */
#define TAS6754_OTW_STATUS_ALL_CH_MASK              (TAS6754_OTW_STATUS_CH1_OTW_MASK | \
                                                    TAS6754_OTW_STATUS_CH2_OTW_MASK | \
                                                    TAS6754_OTW_STATUS_CH3_OTW_MASK | \
                                                    TAS6754_OTW_STATUS_CH4_OTW_MASK)

/* Reset value */
#define TAS6754_OTW_STATUS_RESET                    0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CLIP_WARN_STATUS (0x83)
************************************************************************/
/* Bit field masks */
#define TAS6754_CLIP_WARN_STATUS_RESERVED_MASK      GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_CLIP_WARN_STATUS_CH1_MASK           BIT(3)          /* Bit 3: CH1 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH2_MASK           BIT(2)          /* Bit 2: CH2 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH3_MASK           BIT(1)          /* Bit 1: CH3 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH4_MASK           BIT(0)          /* Bit 0: CH4 CLIP STATUS */

/* Clip status values */
#define TAS6754_CLIP_WARN_STATUS_CH1_NORMAL         (0x00 << 3)  /* 0: Channel 1 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH1_CLIPPING       (0x01 << 3)  /* 1: Channel 1 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH2_NORMAL         (0x00 << 2)  /* 0: Channel 2 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH2_CLIPPING       (0x01 << 2)  /* 1: Channel 2 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH3_NORMAL         (0x00 << 1)  /* 0: Channel 3 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH3_CLIPPING       (0x01 << 1)  /* 1: Channel 3 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH4_NORMAL         (0x00 << 0)  /* 0: Channel 4 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH4_CLIPPING       (0x01 << 0)  /* 1: Channel 4 clipping is above clip detect threshold */

/* Combined channel clip mask (for convenience) */
#define TAS6754_CLIP_WARN_STATUS_ALL_CH_MASK        (TAS6754_CLIP_WARN_STATUS_CH1_MASK | \
                                                    TAS6754_CLIP_WARN_STATUS_CH2_MASK | \
                                                    TAS6754_CLIP_WARN_STATUS_CH3_MASK | \
                                                    TAS6754_CLIP_WARN_STATUS_CH4_MASK)

/* Reset value */
#define TAS6754_CLIP_WARN_STATUS_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CBC_WARNING_STATUS (0x85)
************************************************************************/
/* Bit field masks */
#define TAS6754_CBC_WARNING_CH1_MASK                BIT(7)          /* Bit 7: CH1 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH2_MASK                BIT(6)          /* Bit 6: CH2 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH3_MASK                BIT(5)          /* Bit 5: CH3 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH4_MASK                BIT(4)          /* Bit 4: CH4 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_RESERVED_MASK           GENMASK(3, 0)   /* Bits 3-0: Reserved */

/* CBC warning status values */
#define TAS6754_CBC_WARNING_CH1_NORMAL              (0x00 << 7)  /* 0: Channel 1 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH1_PRESENT             (0x01 << 7)  /* 1: Channel 1 CBC warning is present */
#define TAS6754_CBC_WARNING_CH2_NORMAL              (0x00 << 6)  /* 0: Channel 2 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH2_PRESENT             (0x01 << 6)  /* 1: Channel 2 CBC warning is present */
#define TAS6754_CBC_WARNING_CH3_NORMAL              (0x00 << 5)  /* 0: Channel 3 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH3_PRESENT             (0x01 << 5)  /* 1: Channel 3 CBC warning is present */
#define TAS6754_CBC_WARNING_CH4_NORMAL              (0x00 << 4)  /* 0: Channel 4 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH4_PRESENT             (0x01 << 4)  /* 1: Channel 4 CBC warning is present */

/* Combined channel CBC warning mask (for convenience) */
#define TAS6754_CBC_WARNING_ALL_CH_MASK             (TAS6754_CBC_WARNING_CH1_MASK | \
                                                    TAS6754_CBC_WARNING_CH2_MASK | \
                                                    TAS6754_CBC_WARNING_CH3_MASK | \
                                                    TAS6754_CBC_WARNING_CH4_MASK)

/* Reset value */
#define TAS6754_CBC_WARNING_STATUS_RESET            0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_POWER_FAULT_LATCHED (0x86)
************************************************************************/
/* Bit field masks */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK   BIT(7)          /* Bit 7: DVDD POR STORED */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_6_MASK BIT(6)          /* Bit 6: Reserved */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_5_MASK BIT(5)          /* Bit 5: Reserved */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK    BIT(4)          /* Bit 4: DVDD UV STORED */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK    BIT(3)          /* Bit 3: PVDD OV STORED */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK    BIT(2)          /* Bit 2: VBAT OV STORED */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK    BIT(1)          /* Bit 1: PVDD UV STORED */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK    BIT(0)          /* Bit 0: VBAT UV STORED */

/* Latched fault status values */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_NONE   (0x00 << 7)  /* 0: No DVDD power on reset event stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_STORED (0x01 << 7)  /* 1: DVDD power on reset event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_NONE    (0x00 << 4)  /* 0: No DVDD under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_STORED  (0x01 << 4)  /* 1: DVDD under voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_NONE    (0x00 << 3)  /* 0: No PVDD over voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_STORED  (0x01 << 3)  /* 1: PVDD over voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_NONE    (0x00 << 2)  /* 0: No VBAT over voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_STORED  (0x01 << 2)  /* 1: VBAT over voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_NONE    (0x00 << 1)  /* 0: No PVDD under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_STORED  (0x01 << 1)  /* 1: PVDD under voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_NONE    (0x00 << 0)  /* 0: No VBAT under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_STORED  (0x01 << 0)  /* 1: VBAT under voltage event detected and stored */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_MASK   (TAS6754_POWER_FAULT_LATCHED_RESERVED_6_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_RESERVED_5_MASK)

/* Combined voltage fault mask (for convenience) */
#define TAS6754_POWER_FAULT_LATCHED_ALL_MASK        (TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK | \
                                                    TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK)

/* Reset value */
#define TAS6754_POWER_FAULT_LATCHED_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OTSD_LATCHED (0x87)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTSD_LATCHED_RESERVED_7_6_MASK      GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_OTSD_LATCHED_RESERVED_5_MASK        BIT(5)          /* Bit 5: Reserved */
#define TAS6754_OTSD_LATCHED_GLOBAL_MASK            BIT(4)          /* Bit 4: GLOBAL OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH1_MASK               BIT(3)          /* Bit 3: CH1 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH2_MASK               BIT(2)          /* Bit 2: CH2 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH3_MASK               BIT(1)          /* Bit 1: CH3 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH4_MASK               BIT(0)          /* Bit 0: CH4 OTSD STORED */

/* Latched OTSD status values */
#define TAS6754_OTSD_LATCHED_GLOBAL_NONE            (0x00 << 4)  /* 0: No global over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_GLOBAL_STORED          (0x01 << 4)  /* 1: Global over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH1_NONE               (0x00 << 3)  /* 0: No Channel 1 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH1_STORED             (0x01 << 3)  /* 1: Channel 1 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH2_NONE               (0x00 << 2)  /* 0: No Channel 2 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH2_STORED             (0x01 << 2)  /* 1: Channel 2 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH3_NONE               (0x00 << 1)  /* 0: No Channel 3 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH3_STORED             (0x01 << 1)  /* 1: Channel 3 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH4_NONE               (0x00 << 0)  /* 0: No Channel 4 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH4_STORED             (0x01 << 0)  /* 1: Channel 4 over temperature shutdown event detected and stored */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_OTSD_LATCHED_RESERVED_MASK          (TAS6754_OTSD_LATCHED_RESERVED_7_6_MASK | \
                                                    TAS6754_OTSD_LATCHED_RESERVED_5_MASK)

/* Combined channel OTSD mask (for convenience) */
#define TAS6754_OTSD_LATCHED_ALL_CH_MASK            (TAS6754_OTSD_LATCHED_CH1_MASK | \
                                                    TAS6754_OTSD_LATCHED_CH2_MASK | \
                                                    TAS6754_OTSD_LATCHED_CH3_MASK | \
                                                    TAS6754_OTSD_LATCHED_CH4_MASK)

/* Reset value */
#define TAS6754_OTSD_LATCHED_RESET                  0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OTW_LATCHED (0x88)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTW_LATCHED_RESERVED_MASK           GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_OTW_LATCHED_GLOBAL_MASK             BIT(4)          /* Bit 4: GLOBAL OTW STORED */
#define TAS6754_OTW_LATCHED_CH1_MASK                BIT(3)          /* Bit 3: CH1 OTW STORED */
#define TAS6754_OTW_LATCHED_CH2_MASK                BIT(2)          /* Bit 2: CH2 OTW STORED */
#define TAS6754_OTW_LATCHED_CH3_MASK                BIT(1)          /* Bit 1: CH3 OTW STORED */
#define TAS6754_OTW_LATCHED_CH4_MASK                BIT(0)          /* Bit 0: CH4 OTW STORED */

/* Latched OTW status values */
#define TAS6754_OTW_LATCHED_GLOBAL_NONE             (0x00 << 4)  /* 0: No global over temperature warning event stored */
#define TAS6754_OTW_LATCHED_GLOBAL_STORED           (0x01 << 4)  /* 1: Global over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH1_NONE                (0x00 << 3)  /* 0: No Channel 1 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH1_STORED              (0x01 << 3)  /* 1: Channel 1 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH2_NONE                (0x00 << 2)  /* 0: No Channel 2 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH2_STORED              (0x01 << 2)  /* 1: Channel 2 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH3_NONE                (0x00 << 1)  /* 0: No Channel 3 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH3_STORED              (0x01 << 1)  /* 1: Channel 3 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH4_NONE                (0x00 << 0)  /* 0: No Channel 4 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH4_STORED              (0x01 << 0)  /* 1: Channel 4 over temperature warning event detected and stored */

/* Combined channel OTW mask (for convenience) */
#define TAS6754_OTW_LATCHED_ALL_CH_MASK             (TAS6754_OTW_LATCHED_CH1_MASK | \
                                                    TAS6754_OTW_LATCHED_CH2_MASK | \
                                                    TAS6754_OTW_LATCHED_CH3_MASK | \
                                                    TAS6754_OTW_LATCHED_CH4_MASK)

/* Reset value */
#define TAS6754_OTW_LATCHED_RESET                   0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CLIP_WARN_LATCHED (0x89)
************************************************************************/
/* Bit field masks */
#define TAS6754_CLIP_WARN_LATCHED_RESERVED_MASK     GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_CLIP_WARN_LATCHED_CH1_MASK          BIT(3)          /* Bit 3: CH1 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH2_MASK          BIT(2)          /* Bit 2: CH2 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH3_MASK          BIT(1)          /* Bit 1: CH3 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH4_MASK          BIT(0)          /* Bit 0: CH4 CLIP STORED */

/* Latched clip status values */
#define TAS6754_CLIP_WARN_LATCHED_CH1_NONE          (0x00 << 3)  /* 0: No Channel 1 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH1_STORED        (0x01 << 3)  /* 1: Channel 1 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH2_NONE          (0x00 << 2)  /* 0: No Channel 2 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH2_STORED        (0x01 << 2)  /* 1: Channel 2 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH3_NONE          (0x00 << 1)  /* 0: No Channel 3 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH3_STORED        (0x01 << 1)  /* 1: Channel 3 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH4_NONE          (0x00 << 0)  /* 0: No Channel 4 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH4_STORED        (0x01 << 0)  /* 1: Channel 4 clipping event detected and stored */

/* Combined channel clip mask (for convenience) */
#define TAS6754_CLIP_WARN_LATCHED_ALL_CH_MASK       (TAS6754_CLIP_WARN_LATCHED_CH1_MASK | \
                                                    TAS6754_CLIP_WARN_LATCHED_CH2_MASK | \
                                                    TAS6754_CLIP_WARN_LATCHED_CH3_MASK | \
                                                    TAS6754_CLIP_WARN_LATCHED_CH4_MASK)

/* Reset value */
#define TAS6754_CLIP_WARN_LATCHED_RESET             0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CLK_FAULT_LATCHED (0x8A)
************************************************************************/
/* Bit field masks */
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_7_2_MASK GENMASK(7, 2)   /* Bits 7-2: Reserved */
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_1_MASK   BIT(1)          /* Bit 1: Reserved */
#define TAS6754_CLK_FAULT_LATCHED_CLOCK_FAULT_MASK  BIT(0)          /* Bit 0: CLOCK FAULT STORED */

/* Latched clock fault status values */
#define TAS6754_CLK_FAULT_LATCHED_NONE              (0x00 << 0)  /* 0: No Clock Error event stored */
#define TAS6754_CLK_FAULT_LATCHED_STORED            (0x01 << 0)  /* 1: Clock Error event stored */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_MASK     (TAS6754_CLK_FAULT_LATCHED_RESERVED_7_2_MASK | \
                                                    TAS6754_CLK_FAULT_LATCHED_RESERVED_1_MASK)

/* Reset value */
#define TAS6754_CLK_FAULT_LATCHED_RESET             0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_RTLDG_OL_SL_FAULT_LATCHED (0x8B)
************************************************************************/
/* Bit field masks - Shorted Load (SL) */
#define TAS6754_RTLDG_SL_CH1_MASK                   BIT(7)          /* Bit 7: CH1 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH2_MASK                   BIT(6)          /* Bit 6: CH2 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH3_MASK                   BIT(5)          /* Bit 5: CH3 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH4_MASK                   BIT(4)          /* Bit 4: CH4 RTLDG SL STORED */

/* Bit field masks - Open Load (OL) */
#define TAS6754_RTLDG_OL_CH1_MASK                   BIT(3)          /* Bit 3: CH1 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH2_MASK                   BIT(2)          /* Bit 2: CH2 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH3_MASK                   BIT(1)          /* Bit 1: CH3 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH4_MASK                   BIT(0)          /* Bit 0: CH4 RTLDG OL STORED */

/* Latched Shorted Load (SL) status values */
#define TAS6754_RTLDG_SL_CH1_NONE                   (0x00 << 7)  /* 0: No shorted load condition on Channel 1 */
#define TAS6754_RTLDG_SL_CH1_STORED                 (0x01 << 7)  /* 1: Shorted load condition on Channel 1 */
#define TAS6754_RTLDG_SL_CH2_NONE                   (0x00 << 6)  /* 0: No shorted load condition on Channel 2 */
#define TAS6754_RTLDG_SL_CH2_STORED                 (0x01 << 6)  /* 1: Shorted load condition on Channel 2 */
#define TAS6754_RTLDG_SL_CH3_NONE                   (0x00 << 5)  /* 0: No shorted load condition on Channel 3 */
#define TAS6754_RTLDG_SL_CH3_STORED                 (0x01 << 5)  /* 1: Shorted load condition on Channel 3 */
#define TAS6754_RTLDG_SL_CH4_NONE                   (0x00 << 4)  /* 0: No shorted load condition on Channel 4 */
#define TAS6754_RTLDG_SL_CH4_STORED                 (0x01 << 4)  /* 1: Shorted load condition on Channel 4 */

/* Latched Open Load (OL) status values */
#define TAS6754_RTLDG_OL_CH1_NONE                   (0x00 << 3)  /* 0: No open load condition on Channel 1 */
#define TAS6754_RTLDG_OL_CH1_STORED                 (0x01 << 3)  /* 1: Open load condition on Channel 1 */
#define TAS6754_RTLDG_OL_CH2_NONE                   (0x00 << 2)  /* 0: No open load condition on Channel 2 */
#define TAS6754_RTLDG_OL_CH2_STORED                 (0x01 << 2)  /* 1: Open load condition on Channel 2 */
#define TAS6754_RTLDG_OL_CH3_NONE                   (0x00 << 1)  /* 0: No open load condition on Channel 3 */
#define TAS6754_RTLDG_OL_CH3_STORED                 (0x01 << 1)  /* 1: Open load condition on Channel 3 */
#define TAS6754_RTLDG_OL_CH4_NONE                   (0x00 << 0)  /* 0: No open load condition on Channel 4 */
#define TAS6754_RTLDG_OL_CH4_STORED                 (0x01 << 0)  /* 1: Open load condition on Channel 4 */

/* Combined masks (for convenience) */
#define TAS6754_RTLDG_SL_ALL_CH_MASK                (TAS6754_RTLDG_SL_CH1_MASK | \
                                                    TAS6754_RTLDG_SL_CH2_MASK | \
                                                    TAS6754_RTLDG_SL_CH3_MASK | \
                                                    TAS6754_RTLDG_SL_CH4_MASK)

#define TAS6754_RTLDG_OL_ALL_CH_MASK                (TAS6754_RTLDG_OL_CH1_MASK | \
                                                    TAS6754_RTLDG_OL_CH2_MASK | \
                                                    TAS6754_RTLDG_OL_CH3_MASK | \
                                                    TAS6754_RTLDG_OL_CH4_MASK)

/* Reset value */
#define TAS6754_RTLDG_OL_SL_FAULT_LATCHED_RESET     0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CBC_FAULT_WARN_LATCHED (0x8D)
************************************************************************/
/* Bit field masks - CBC Warning */
#define TAS6754_CBC_WARN_CH1_MASK                   BIT(7)          /* Bit 7: CH1 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH2_MASK                   BIT(6)          /* Bit 6: CH2 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH3_MASK                   BIT(5)          /* Bit 5: CH3 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH4_MASK                   BIT(4)          /* Bit 4: CH4 CBC WARN STORED */

/* Bit field masks - CBC Fault */
#define TAS6754_CBC_FAULT_CH1_MASK                  BIT(3)          /* Bit 3: CH1 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH2_MASK                  BIT(2)          /* Bit 2: CH2 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH3_MASK                  BIT(1)          /* Bit 1: CH3 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH4_MASK                  BIT(0)          /* Bit 0: CH4 CBC FAULT STORED */

/* Latched CBC Warning status values */
#define TAS6754_CBC_WARN_CH1_NONE                   (0x00 << 7)  /* 0: No Channel 1 load current warning event stored */
#define TAS6754_CBC_WARN_CH1_STORED                 (0x01 << 7)  /* 1: Channel 1 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH2_NONE                   (0x00 << 6)  /* 0: No Channel 2 load current warning event stored */
#define TAS6754_CBC_WARN_CH2_STORED                 (0x01 << 6)  /* 1: Channel 2 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH3_NONE                   (0x00 << 5)  /* 0: No Channel 3 load current warning event stored */
#define TAS6754_CBC_WARN_CH3_STORED                 (0x01 << 5)  /* 1: Channel 3 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH4_NONE                   (0x00 << 4)  /* 0: No Channel 4 load current warning event stored */
#define TAS6754_CBC_WARN_CH4_STORED                 (0x01 << 4)  /* 1: Channel 4 load current warning event detected and stored */

/* Latched CBC Fault status values */
#define TAS6754_CBC_FAULT_CH1_NONE                  (0x00 << 3)  /* 0: No Channel 1 load current fault event stored */
#define TAS6754_CBC_FAULT_CH1_STORED                (0x01 << 3)  /* 1: Channel 1 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH2_NONE                  (0x00 << 2)  /* 0: No Channel 2 load current fault event stored */
#define TAS6754_CBC_FAULT_CH2_STORED                (0x01 << 2)  /* 1: Channel 2 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH3_NONE                  (0x00 << 1)  /* 0: No Channel 3 load current fault event stored */
#define TAS6754_CBC_FAULT_CH3_STORED                (0x01 << 1)  /* 1: Channel 3 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH4_NONE                  (0x00 << 0)  /* 0: No Channel 4 load current fault event stored */
#define TAS6754_CBC_FAULT_CH4_STORED                (0x01 << 0)  /* 1: Channel 4 load current fault event detected and stored */

/* Combined masks (for convenience) */
#define TAS6754_CBC_WARN_ALL_CH_MASK                (TAS6754_CBC_WARN_CH1_MASK | \
                                                    TAS6754_CBC_WARN_CH2_MASK | \
                                                    TAS6754_CBC_WARN_CH3_MASK | \
                                                    TAS6754_CBC_WARN_CH4_MASK)

#define TAS6754_CBC_FAULT_ALL_CH_MASK               (TAS6754_CBC_FAULT_CH1_MASK | \
                                                    TAS6754_CBC_FAULT_CH2_MASK | \
                                                    TAS6754_CBC_FAULT_CH3_MASK | \
                                                    TAS6754_CBC_FAULT_CH4_MASK)

/* Reset value */
#define TAS6754_CBC_FAULT_WARN_LATCHED_RESET        0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OC_DC_FAULT_LATCHED (0x8E)
************************************************************************/
/* Bit field masks - Over Current (OC) Fault */
#define TAS6754_OC_FAULT_CH1_MASK                   BIT(7)          /* Bit 7: CH1 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH2_MASK                   BIT(6)          /* Bit 6: CH2 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH3_MASK                   BIT(5)          /* Bit 5: CH3 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH4_MASK                   BIT(4)          /* Bit 4: CH4 OC FAULT STORED */

/* Bit field masks - DC Fault */
#define TAS6754_DC_FAULT_CH1_MASK                   BIT(3)          /* Bit 3: CH1 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH2_MASK                   BIT(2)          /* Bit 2: CH2 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH3_MASK                   BIT(1)          /* Bit 1: CH3 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH4_MASK                   BIT(0)          /* Bit 0: CH4 DC FAULT STORED */

/* Latched Over Current (OC) Fault status values */
#define TAS6754_OC_FAULT_CH1_NONE                   (0x00 << 7)  /* 0: No Channel 1 over current fault event stored */
#define TAS6754_OC_FAULT_CH1_STORED                 (0x01 << 7)  /* 1: Channel 1 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH2_NONE                   (0x00 << 6)  /* 0: No Channel 2 over current fault event stored */
#define TAS6754_OC_FAULT_CH2_STORED                 (0x01 << 6)  /* 1: Channel 2 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH3_NONE                   (0x00 << 5)  /* 0: No Channel 3 over current fault event stored */
#define TAS6754_OC_FAULT_CH3_STORED                 (0x01 << 5)  /* 1: Channel 3 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH4_NONE                   (0x00 << 4)  /* 0: No Channel 4 over current fault event stored */
#define TAS6754_OC_FAULT_CH4_STORED                 (0x01 << 4)  /* 1: Channel 4 over current fault event detected and stored */

/* Latched DC Fault status values */
#define TAS6754_DC_FAULT_CH1_NONE                   (0x00 << 3)  /* 0: No Channel 1 DC fault event stored */
#define TAS6754_DC_FAULT_CH1_STORED                 (0x01 << 3)  /* 1: Channel 1 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH2_NONE                   (0x00 << 2)  /* 0: No Channel 2 DC fault event stored */
#define TAS6754_DC_FAULT_CH2_STORED                 (0x01 << 2)  /* 1: Channel 2 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH3_NONE                   (0x00 << 1)  /* 0: No Channel 3 DC fault event stored */
#define TAS6754_DC_FAULT_CH3_STORED                 (0x01 << 1)  /* 1: Channel 3 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH4_NONE                   (0x00 << 0)  /* 0: No Channel 4 DC fault event stored */
#define TAS6754_DC_FAULT_CH4_STORED                 (0x01 << 0)  /* 1: Channel 4 DC fault event detected and stored */

/* Combined masks (for convenience) */
#define TAS6754_OC_FAULT_ALL_CH_MASK                (TAS6754_OC_FAULT_CH1_MASK | \
                                                    TAS6754_OC_FAULT_CH2_MASK | \
                                                    TAS6754_OC_FAULT_CH3_MASK | \
                                                    TAS6754_OC_FAULT_CH4_MASK)

#define TAS6754_DC_FAULT_ALL_CH_MASK                (TAS6754_DC_FAULT_CH1_MASK | \
                                                    TAS6754_DC_FAULT_CH2_MASK | \
                                                    TAS6754_DC_FAULT_CH3_MASK | \
                                                    TAS6754_DC_FAULT_CH4_MASK)

/* Reset value */
#define TAS6754_OC_DC_FAULT_LATCHED_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OTSD_RECOVERY_EN (0x8F)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTSD_RECOVERY_RESERVED_7_3_MASK     GENMASK(7, 3)   /* Bits 7-3: Reserved */
#define TAS6754_OTSD_RECOVERY_RESERVED_2_MASK       BIT(2)          /* Bit 2: Reserved */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_EN_MASK      BIT(1)          /* Bit 1: OTSD AUTO REC ENABLE */
#define TAS6754_OTSD_RECOVERY_RESERVED_0_MASK       BIT(0)          /* Bit 0: Reserved */

/* OTSD Auto Recovery Enable values */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_DISABLE      (0x00 << 1)  /* 0: Disable Overtemperature Shutdown Auto-recovery */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_ENABLE       (0x01 << 1)  /* 1: Enable Overtemperature Shutdown Auto-recovery */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_OTSD_RECOVERY_RESERVED_MASK         (TAS6754_OTSD_RECOVERY_RESERVED_7_3_MASK | \
                                                    TAS6754_OTSD_RECOVERY_RESERVED_2_MASK | \
                                                    TAS6754_OTSD_RECOVERY_RESERVED_0_MASK)

/* Reset value */
#define TAS6754_OTSD_RECOVERY_EN_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_2 (0x90)
************************************************************************/
/* Bit field masks */
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_MASK     BIT(7)          /* Bit 7: CBC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_RESERVED_6_MASK    BIT(6)          /* Bit 6: Reserved */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_MASK    BIT(5)          /* Bit 5: OTSD LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_MASK   BIT(4)          /* Bit 4: POWER LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_MASK        BIT(3)          /* Bit 3: DC LDG FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_RESERVED_2_MASK    BIT(2)          /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_2_OTSD_MASK          BIT(1)          /* Bit 1: OTSD FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_POWER_MASK         BIT(0)          /* Bit 0: POWER FAULT GPIO */

/* CBC LATCH FAULT GPIO values (bit 7) */
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_DISABLE  (0x00 << 7)  /* 0: Latching Overcurrent Limiting events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_ENABLE   (0x01 << 7)  /* 1: Latching Overcurrent Limiting events are routed to FAULT */

/* OTSD LATCH FAULT GPIO values (bit 5) */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_DISABLE (0x00 << 5)  /* 0: Latching Overtemperature Shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_ENABLE  (0x01 << 5)  /* 1: Latching Overtemperature Shutdown events are routed to FAULT */

/* POWER LATCH FAULT GPIO values (bit 4) */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_DISABLE (0x00 << 4) /* 0: Latching Power Fault events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_ENABLE (0x01 << 4)  /* 1: Latching Power Fault events are routed to FAULT */

/* DC LDG FAULT GPIO values (bit 3) */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_DISABLE     (0x00 << 3)  /* 0: Non-Latched DC Load Diagnostic events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_ENABLE      (0x01 << 3)  /* 1: Non-Latched DC Load Diagnostics events are routed to FAULT */

/* OTSD FAULT GPIO values (bit 1) */
#define TAS6754_REPORT_ROUTING_2_OTSD_DISABLE       (0x00 << 1)  /* 0: Non-Latched Overtemperature Shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_ENABLE        (0x01 << 1)  /* 1: Non-Latched Overtemperature Shutdown events are routed to FAULT */

/* POWER FAULT GPIO values (bit 0) */
#define TAS6754_REPORT_ROUTING_2_POWER_DISABLE      (0x00 << 0)  /* 0: Non-Latching Power Fault events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_ENABLE       (0x01 << 0)  /* 1: Non-Latching Power Fault events are routed to FAULT */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_REPORT_ROUTING_2_RESERVED_MASK      (TAS6754_REPORT_ROUTING_2_RESERVED_6_MASK | \
                                                    TAS6754_REPORT_ROUTING_2_RESERVED_2_MASK)

/* Reset value */
#define TAS6754_REPORT_ROUTING_2_RESET              0xA2            /* Reset value for the register */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_3 (0x91)
************************************************************************/
/* Bit field masks */
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_MASK     BIT(7)          /* Bit 7: CBC LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_RESERVED_6_MASK    BIT(6)          /* Bit 6: Reserved */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_MASK    BIT(5)          /* Bit 5: OTSD LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_MASK   BIT(4)          /* Bit 4: POWER LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_MASK        BIT(3)          /* Bit 3: DC LDG WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_RESERVED_2_MASK    BIT(2)          /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_3_OTSD_MASK          BIT(1)          /* Bit 1: OTSD WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_POWER_MASK         BIT(0)          /* Bit 0: POWER WARN GPIO */

/* CBC LATCH WARN GPIO values (bit 7) */
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_DISABLE  (0x00 << 7)  /* 0: Latching Overcurrent Limiting events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_ENABLE   (0x01 << 7)  /* 1: Latching Overcurrent Limiting events are routed to WARN */

/* OTSD LATCH WARN GPIO values (bit 5) */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_DISABLE (0x00 << 5)  /* 0: Latching Overtemperature Shutdown events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_ENABLE  (0x01 << 5)  /* 1: Latching Overtemperature Shutdown events are routed to WARN */

/* POWER LATCH WARN GPIO values (bit 4) */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_DISABLE (0x00 << 4) /* 0: Latching Power Fault events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_ENABLE (0x01 << 4)  /* 1: Latching Power Fault events are routed to WARN */

/* DC LDG WARN GPIO values (bit 3) */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_DISABLE     (0x00 << 3)  /* 0: Non-Latched DC Load Diagnostic events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_ENABLE      (0x01 << 3)  /* 1: Non-Latched DC Load Diagnostics events are routed to WARN */

/* OTSD WARN GPIO values (bit 1) */
#define TAS6754_REPORT_ROUTING_3_OTSD_DISABLE       (0x00 << 1)  /* 0: Non-Latched Overtemperature Shutdown events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_ENABLE        (0x01 << 1)  /* 1: Non-Latched Overtemperature Shutdown events are routed to WARN */

/* POWER WARN GPIO values (bit 0) */
#define TAS6754_REPORT_ROUTING_3_POWER_DISABLE      (0x00 << 0)  /* 0: Non-Latching Power Fault events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_ENABLE       (0x01 << 0)  /* 1: Non-Latching Power Fault events are routed to WARN */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_REPORT_ROUTING_3_RESERVED_MASK      (TAS6754_REPORT_ROUTING_3_RESERVED_6_MASK | \
                                                    TAS6754_REPORT_ROUTING_3_RESERVED_2_MASK)

/* Reset value */
#define TAS6754_REPORT_ROUTING_3_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_4 (0x92)
************************************************************************/
/* Bit field masks */
#define TAS6754_REPORT_ROUTING_4_RESERVED_7_MASK    BIT(7)          /* Bit 7: Reserved */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_MASK    BIT(6)          /* Bit 6: CLIP LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_MASK     BIT(5)          /* Bit 5: OTW LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_OTW_MASK           BIT(4)          /* Bit 4: OTW WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_MASK       BIT(3)          /* Bit 3: PROT SD FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_MASK      BIT(2)          /* Bit 2: OC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_MASK      BIT(1)          /* Bit 1: DC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_MASK    BIT(0)          /* Bit 0: FAULT WARN GPIO */

/* CLIP LATCH WARN GPIO values (bit 6) */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_DISABLE (0x00 << 6)  /* 0: Latching Clip Detect events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_ENABLE  (0x01 << 6)  /* 1: Latching Clip Detect events are routed to WARN */

/* OTW LATCH WARN GPIO values (bit 5) */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_DISABLE  (0x00 << 5)  /* 0: Latching Overtemperature Warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_ENABLE   (0x01 << 5)  /* 1: Latching Overtemperature Warning events are routed to WARN */

/* OTW WARN GPIO values (bit 4) */
#define TAS6754_REPORT_ROUTING_4_OTW_DISABLE        (0x00 << 4)  /* 0: Non-latched Overtemperature Warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_ENABLE         (0x01 << 4)  /* 1: Non-latched Overtemperature Warning events are routed to WARN */

/* PROT SD FAULT GPIO values (bit 3) */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_DISABLE    (0x00 << 3)  /* 0: If any channel enters the FAULT state it is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_ENABLE     (0x01 << 3)  /* 1: If any channel enters the FAULT state it is reported to FAULT */

/* OC LATCH FAULT GPIO values (bit 2) */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_DISABLE   (0x00 << 2)  /* 0: Latching Overcurrent shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_ENABLE    (0x01 << 2)  /* 1: Latching Overcurrent shutdown events are routed to FAULT */

/* DC LATCH FAULT GPIO values (bit 1) */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_DISABLE   (0x00 << 1)  /* 0: Latching DC Detect events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_ENABLE    (0x01 << 1)  /* 1: Latching DC Detect events are routed to FAULT */

/* FAULT WARN GPIO values (bit 0) */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_DISABLE (0x00 << 0)  /* 0: WARN pin signals are not routed to the FAULT pin */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_ENABLE  (0x01 << 0)  /* 1: WARN pin signals are routed to the FAULT pin */

/* Reset value */
#define TAS6754_REPORT_ROUTING_4_RESET              0x06            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CLIP_DETECT_CTRL (0x93)
************************************************************************/
/* Bit field masks */
#define TAS6754_CLIP_DETECT_RESERVED_7_MASK         BIT(7)          /* Bit 7: Reserved */
#define TAS6754_CLIP_DETECT_ENABLE_MASK             BIT(6)          /* Bit 6: CLIP DETECT ENABLE */
#define TAS6754_CLIP_DETECT_RESERVED_5_4_MASK       GENMASK(5, 4)   /* Bits 5-4: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_3_MASK         BIT(3)          /* Bit 3: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_2_MASK         BIT(2)          /* Bit 2: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_1_MASK         BIT(1)          /* Bit 1: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_0_MASK         BIT(0)          /* Bit 0: Reserved */

/* CLIP DETECT ENABLE values (bit 6) */
#define TAS6754_CLIP_DETECT_DISABLE                 (0x00 << 6)  /* 0: Disable Clip detect */
#define TAS6754_CLIP_DETECT_ENABLE                  (0x01 << 6)  /* 1: Enable Clip detect */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_CLIP_DETECT_RESERVED_MASK           (TAS6754_CLIP_DETECT_RESERVED_7_MASK | \
                                                    TAS6754_CLIP_DETECT_RESERVED_5_4_MASK | \
                                                    TAS6754_CLIP_DETECT_RESERVED_3_MASK | \
                                                    TAS6754_CLIP_DETECT_RESERVED_2_MASK | \
                                                    TAS6754_CLIP_DETECT_RESERVED_1_MASK | \
                                                    TAS6754_CLIP_DETECT_RESERVED_0_MASK)

/* Reset value */
#define TAS6754_CLIP_DETECT_CTRL_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_5 (0x94)
************************************************************************/
/* Bit field masks */
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_MASK     BIT(7)          /* Bit 7: CLK FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_MASK     BIT(6)          /* Bit 6: CLK LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_MASK      BIT(5)          /* Bit 5: CBC WARN FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_RTLDG_LATCH_F_MASK BIT(4)          /* Bit 4: RTLDG LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_RESERVED_3_MASK    BIT(3)          /* Bit 3: Reserved */
#define TAS6754_REPORT_ROUTING_5_RESERVED_2_MASK    BIT(2)          /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_MASK     BIT(1)          /* Bit 1: CLIP WARN GPIO */
#define TAS6754_REPORT_ROUTING_5_RTLDG_LATCH_W_MASK BIT(0)          /* Bit 0: RTLDG LATCH WARN GPIO */

/* CLK FAULT GPIO values (bit 7) */
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_DISABLE  (0x00 << 7)  /* 0: Non-Latched Clock error events are not routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_ENABLE   (0x01 << 7)  /* 1: Non-Latched Clock error events are routed to FAULT and WARN */

/* CLK LATCH FAULT GPIO values (bit 6) */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_DISABLE  (0x00 << 6)  /* 0: Latched Clock error events are not routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_ENABLE   (0x01 << 6)  /* 1: Latched Clock error events are routed to FAULT and WARN */

/* CBC WARN FAULT GPIO values (bit 5) */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_DISABLE   (0x00 << 5)  /* 0: Unlatched CBC warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_ENABLE    (0x01 << 5)  /* 1: Unlatched CBC warning events are routed to WARN */

/* RTLDG LATCH FAULT GPIO values (bit 4) */
#define TAS6754_REPORT_ROUTING_5_RTLDG_F_DISABLE    (0x00 << 4)  /* 0: Latched Real-time load diagnostic events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_5_RTLDG_F_ENABLE     (0x01 << 4)  /* 1: Latched Real-time load diagnostic events are routed to FAULT */

/* CLIP WARN GPIO values (bit 1) */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_DISABLE  (0x00 << 1)  /* 0: Non-latched Clip Detect events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_ENABLE   (0x01 << 1)  /* 1: Non-latched Clip Detect events are routed to WARN */

/* RTLDG LATCH WARN GPIO values (bit 0) */
#define TAS6754_REPORT_ROUTING_5_RTLDG_W_DISABLE    (0x00 << 0)  /* 0: Latched Real-time load diagnostic events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_RTLDG_W_ENABLE     (0x01 << 0)  /* 1: Latched Real-time load diagnostic events are routed to WARN */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_REPORT_ROUTING_5_RESERVED_MASK      (TAS6754_REPORT_ROUTING_5_RESERVED_3_MASK | \
                                                    TAS6754_REPORT_ROUTING_5_RESERVED_2_MASK)

/* Reset value */
#define TAS6754_REPORT_ROUTING_5_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO1_OUTPUT_SELECT (0x95)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO1_OUTPUT_RESERVED_7_6_MASK      GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_GPIO1_OUTPUT_RESERVED_5_MASK        BIT(5)          /* Bit 5: Reserved */
#define TAS6754_GPIO1_OUTPUT_SELECT_MASK            GENMASK(4, 0)   /* Bits 4-0: GPIO1 OUTPUT */

/* GPIO1 OUTPUT values (bits 4-0) */
#define TAS6754_GPIO1_OUTPUT_LOW                    0x00            /* 0x00: LOW */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_ALL          0x02            /* 0x02: Auto Mute All Channels */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH4          0x03            /* 0x03: Auto Mute Channel 4 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH3          0x04            /* 0x04: Auto Mute Channel 3 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH2          0x05            /* 0x05: Auto Mute Channel 2 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH1          0x06            /* 0x06: Auto Mute Channel 1 */
#define TAS6754_GPIO1_OUTPUT_SDOUT2                 0x08            /* 0x08: SDOUT2 */
#define TAS6754_GPIO1_OUTPUT_SDOUT1                 0x09            /* 0x09: SDOUT1 */
#define TAS6754_GPIO1_OUTPUT_WARN                   0x0A            /* 0x0A: WARN */
#define TAS6754_GPIO1_OUTPUT_FAULT                  0x0B            /* 0x0B: FAULT */
#define TAS6754_GPIO1_OUTPUT_CLOCK_SYNC             0x0E            /* 0x0E: Clock sync out to secondary devices */
#define TAS6754_GPIO1_OUTPUT_INVALID_CLOCK          0x0F            /* 0x0F: Invalid Clock */
#define TAS6754_GPIO1_OUTPUT_HIGH                   0x13            /* 0x13: HIGH */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO1_OUTPUT_RESERVED_MASK          (TAS6754_GPIO1_OUTPUT_RESERVED_7_6_MASK | \
                                                    TAS6754_GPIO1_OUTPUT_RESERVED_5_MASK)

/* Reset value */
#define TAS6754_GPIO1_OUTPUT_SELECT_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO2_OUTPUT_SELECT (0x96)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO2_OUTPUT_RESERVED_7_6_MASK      GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_GPIO2_OUTPUT_RESERVED_5_MASK        BIT(5)          /* Bit 5: Reserved */
#define TAS6754_GPIO2_OUTPUT_SELECT_MASK            GENMASK(4, 0)   /* Bits 4-0: GPIO2 OUTPUT */

/* GPIO2 OUTPUT values (bits 4-0) */
#define TAS6754_GPIO2_OUTPUT_LOW                    0x00            /* 0x00: LOW */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_ALL          0x02            /* 0x02: Auto Mute All Channels */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH4          0x03            /* 0x03: Auto Mute Channel 4 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH3          0x04            /* 0x04: Auto Mute Channel 3 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH2          0x05            /* 0x05: Auto Mute Channel 2 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH1          0x06            /* 0x06: Auto Mute Channel 1 */
#define TAS6754_GPIO2_OUTPUT_SDOUT2                 0x08            /* 0x08: SDOUT2 */
#define TAS6754_GPIO2_OUTPUT_SDOUT1                 0x09            /* 0x09: SDOUT1 */
#define TAS6754_GPIO2_OUTPUT_WARN                   0x0A            /* 0x0A: WARN */
#define TAS6754_GPIO2_OUTPUT_FAULT                  0x0B            /* 0x0B: FAULT */
#define TAS6754_GPIO2_OUTPUT_CLOCK_SYNC             0x0E            /* 0x0E: Clock sync out to secondary devices */
#define TAS6754_GPIO2_OUTPUT_INVALID_CLOCK          0x0F            /* 0x0F: Invalid Clock */
#define TAS6754_GPIO2_OUTPUT_HIGH                   0x13            /* 0x13: HIGH */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO2_OUTPUT_RESERVED_MASK          (TAS6754_GPIO2_OUTPUT_RESERVED_7_6_MASK | \
                                                    TAS6754_GPIO2_OUTPUT_RESERVED_5_MASK)

/* Reset value */
#define TAS6754_GPIO2_OUTPUT_SELECT_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INPUT_SLEEP_HIZ (0x9B)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK          BIT(7)          /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_MASK          GENMASK(6, 4)   /* Bits 6-4: GPIO INPUT FOR DEEP SLEEP */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK          BIT(3)          /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_HIZ_MASK                 GENMASK(2, 0)   /* Bits 2-0: GPIO INPUT FOR HI Z */

/* GPIO INPUT FOR DEEP SLEEP values (bits 6-4) */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_NA            0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_GPIO1         0x10            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_GPIO2         0x20            /* 010: GPIO2 */

/* GPIO INPUT FOR HI Z values (bits 2-0) */
#define TAS6754_GPIO_INPUT_HIZ_NA                   0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_HIZ_GPIO1                0x01            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_HIZ_GPIO2                0x02            /* 010: GPIO2 */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INPUT_RESERVED_MASK            (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_GPIO_INPUT_SLEEP_HIZ_RESET          0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INPUT_PLAY_SLEEP (0x9C)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK          BIT(7)          /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_PLAY_MASK                GENMASK(6, 4)   /* Bits 6-4: GPIO INPUT PLAY */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK          BIT(3)          /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_SLEEP_MASK               GENMASK(2, 0)   /* Bits 2-0: GPIO INPUT SLEEP */

/* GPIO INPUT PLAY values (bits 6-4) */
#define TAS6754_GPIO_INPUT_PLAY_NA                  0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_PLAY_GPIO1               0x10            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_PLAY_GPIO2               0x20            /* 010: GPIO2 */

/* GPIO INPUT SLEEP values (bits 2-0) */
#define TAS6754_GPIO_INPUT_SLEEP_NA                 0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_SLEEP_GPIO1              0x01            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SLEEP_GPIO2              0x02            /* 010: GPIO2 */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INPUT_PS_RESERVED_MASK         (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_GPIO_INPUT_PLAY_SLEEP_RESET         0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INPUT_MUTE (0x9D)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK          BIT(7)          /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_6_4_MASK        GENMASK(6, 4)   /* Bits 6-4: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK          BIT(3)          /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_MUTE_MASK                GENMASK(2, 0)   /* Bits 2-0: GPIO INPUT MUTE */

/* GPIO INPUT MUTE values (bits 2-0) */
#define TAS6754_GPIO_INPUT_MUTE_NA                  0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_MUTE_GPIO1               0x01            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_MUTE_GPIO2               0x02            /* 010: GPIO2 */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INPUT_MUTE_RESERVED_MASK       (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_6_4_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_GPIO_INPUT_MUTE_RESET               0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INPUT_SYNC (0x9E)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK          BIT(7)          /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_6_4_MASK        GENMASK(6, 4)   /* Bits 6-4: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK          BIT(3)          /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_SYNC_MASK                GENMASK(2, 0)   /* Bits 2-0: GPIO INPUT SYNC */

/* GPIO INPUT SYNC values (bits 2-0) */
#define TAS6754_GPIO_INPUT_SYNC_NA                  0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_SYNC_GPIO1               0x01            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SYNC_GPIO2               0x02            /* 010: GPIO2 */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INPUT_SYNC_RESERVED_MASK       (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_6_4_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_GPIO_INPUT_SYNC_RESET               0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INPUT_SDIN2 (0x9F)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK          BIT(7)          /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_SDIN2_MASK               GENMASK(6, 4)   /* Bits 6-4: GPIO INPUT SDIN2 */
#define TAS6754_GPIO_INPUT_RESERVED_3_0_MASK        GENMASK(3, 0)   /* Bits 3-0: Reserved */

/* GPIO INPUT SDIN2 values (bits 6-4) */
#define TAS6754_GPIO_INPUT_SDIN2_NA                 0x00            /* 000: N/A */
#define TAS6754_GPIO_INPUT_SDIN2_GPIO1              0x10            /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SDIN2_GPIO2              0x20            /* 010: GPIO2 */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INPUT_SDIN2_RESERVED_MASK      (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                    TAS6754_GPIO_INPUT_RESERVED_3_0_MASK)

/* Reset value */
#define TAS6754_GPIO_INPUT_SDIN2_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_CTRL (0xA0)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_CTRL_GPIO1_IO_SELECT_MASK      BIT(7)          /* Bit 7: GPIO1 IO SELECT */
#define TAS6754_GPIO_CTRL_GPIO2_IO_SELECT_MASK      BIT(6)          /* Bit 6: GPIO2 IO SELECT */
#define TAS6754_GPIO_CTRL_RESERVED_5_MASK           BIT(5)          /* Bit 5: Reserved */
#define TAS6754_GPIO_CTRL_RESERVED_4_MASK           BIT(4)          /* Bit 4: Reserved */
#define TAS6754_GPIO_CTRL_GPO1_MODE_MASK            BIT(3)          /* Bit 3: GPO1 MODE */
#define TAS6754_GPIO_CTRL_GPO2_MODE_MASK            BIT(2)          /* Bit 2: GPO2 MODE */
#define TAS6754_GPIO_CTRL_RESERVED_1_MASK           BIT(1)          /* Bit 1: Reserved */
#define TAS6754_GPIO_CTRL_RESERVED_0_MASK           BIT(0)          /* Bit 0: Reserved */

/* GPIO1 IO SELECT values (bit 7) */
#define TAS6754_GPIO_CTRL_GPIO1_AS_INPUT            (0x00 << 7)  /* 0: Set GPIO1 as input */
#define TAS6754_GPIO_CTRL_GPIO1_AS_OUTPUT           (0x01 << 7)  /* 1: Set GPIO1 as output */

/* GPIO2 IO SELECT values (bit 6) */
#define TAS6754_GPIO_CTRL_GPIO2_AS_INPUT            (0x00 << 6)  /* 0: Set GPIO2 as input */
#define TAS6754_GPIO_CTRL_GPIO2_AS_OUTPUT           (0x01 << 6)  /* 1: Set GPIO2 as output */

/* GPO1 MODE values (bit 3) */
#define TAS6754_GPIO_CTRL_GPO1_OUTPUT_BUFFER        (0x00 << 3)  /* 0: Output Buffer mode */
#define TAS6754_GPIO_CTRL_GPO1_OPEN_DRAIN           (0x01 << 3)  /* 1: Open drain mode */

/* GPO2 MODE values (bit 2) */
#define TAS6754_GPIO_CTRL_GPO2_OUTPUT_BUFFER        (0x00 << 2)  /* 0: Output Buffer mode */
#define TAS6754_GPIO_CTRL_GPO2_OPEN_DRAIN           (0x01 << 2)  /* 1: Open drain mode */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_CTRL_RESERVED_MASK             (TAS6754_GPIO_CTRL_RESERVED_5_MASK | \
                                                    TAS6754_GPIO_CTRL_RESERVED_4_MASK | \
                                                    TAS6754_GPIO_CTRL_RESERVED_1_MASK | \
                                                    TAS6754_GPIO_CTRL_RESERVED_0_MASK)

/* Reset value */
#define TAS6754_GPIO_CTRL_RESET                     0x22            /* Reset value for the register */
/************************************************************************
* 					TAS6754_GPIO_INVERT (0xA1)
************************************************************************/
/* Bit field masks */
#define TAS6754_GPIO_INVERT_GPO1_INV_MASK           BIT(7)          /* Bit 7: GPO1 INV */
#define TAS6754_GPIO_INVERT_GPO2_INV_MASK           BIT(6)          /* Bit 6: GPO2 INV */
#define TAS6754_GPIO_INVERT_RESERVED_5_MASK         BIT(5)          /* Bit 5: Reserved */
#define TAS6754_GPIO_INVERT_RESERVED_4_1_MASK       GENMASK(4, 1)   /* Bits 4-1: Reserved */
#define TAS6754_GPIO_INVERT_GPO_PU_DISABLE_MASK     BIT(0)          /* Bit 0: GPO PU DISABLE */

/* GPO1 INV values (bit 7) */
#define TAS6754_GPIO_INVERT_GPO1_NON_INVERTED       (0x00 << 7)  /* 0: GPIO1 Output signal is non-inverted */
#define TAS6754_GPIO_INVERT_GPO1_INVERTED           (0x01 << 7)  /* 1: GPIO1 Output signal is inverted */

/* GPO2 INV values (bit 6) */
#define TAS6754_GPIO_INVERT_GPO2_NON_INVERTED       (0x00 << 6)  /* 0: GPIO2 Output signal is non-inverted */
#define TAS6754_GPIO_INVERT_GPO2_INVERTED           (0x01 << 6)  /* 1: GPIO2 Output signal is inverted */

/* GPO PU DISABLE values (bit 0) */
#define TAS6754_GPIO_INVERT_GPO_PU_ENABLE           (0x00 << 0)  /* 0: Enable internal 110kΩ pull-up of GP outputs in open drain */
#define TAS6754_GPIO_INVERT_GPO_PU_DISABLE          (0x01 << 0)  /* 1: Disable internal 110kΩ pull-up of GP outputs in open drain */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_GPIO_INVERT_RESERVED_MASK           (TAS6754_GPIO_INVERT_RESERVED_5_MASK | \
                                                    TAS6754_GPIO_INVERT_RESERVED_4_1_MASK)

/* Reset value */
#define TAS6754_GPIO_INVERT_RESET                   0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_CTRL (0xB0)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_ABORT_MASK                   BIT(7)          /* Bit 7: LDG ABORT */
#define TAS6754_DC_LDG_BUFFER_WAIT_TIME_MASK        GENMASK(6, 5)   /* Bits 6-5: LDG BUFFER WAIT TIME */
#define TAS6754_DC_LDG_RESERVED_MASK                GENMASK(4, 3)   /* Bits 4-3: Reserved */
#define TAS6754_DC_LDG_WAIT_BYPASS_MASK             BIT(2)          /* Bit 2: LDG WAIT BYPASS */
#define TAS6754_DC_LDG_SLOL_DISABLE_MASK            BIT(1)          /* Bit 1: LDG SLOL DISABLE */
#define TAS6754_DC_LDG_BYPASS_MASK                  BIT(0)          /* Bit 0: LDG BYPASS */

/* LDG ABORT values (bit 7) */
#define TAS6754_DC_LDG_ABORT_NORMAL                 (0x00 << 7)  /* 0: Normal operation */
#define TAS6754_DC_LDG_ABORT_ABORT                  (0x01 << 7)  /* 1: Abort DC load diagnostic */

/* LDG BUFFER WAIT TIME values (bits 6-5) */
#define TAS6754_DC_LDG_BUFFER_WAIT_1MS              (0x00 << 5)  /* 00: Buffer wait time 1ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_2MS              (0x01 << 5)  /* 01: Buffer wait time 2ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_5MS              (0x02 << 5)  /* 10: Buffer wait time 5ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_10MS             (0x03 << 5)  /* 11: Buffer wait time 10ms */

/* LDG WAIT BYPASS values (bit 2) */
#define TAS6754_DC_LDG_WAIT_BYPASS_DISABLE          (0x00 << 2)  /* 0: Enable the waiting loop at the end of shorted/open load detection */
#define TAS6754_DC_LDG_WAIT_BYPASS_ENABLE           (0x01 << 2)  /* 1: Bypass the waiting loop at the end of shorted/open load detection */

/* LDG SLOL DISABLE values (bit 1) */
#define TAS6754_DC_LDG_SLOL_ENABLE                  (0x00 << 1)  /* 0: Shorted load and open load detection are enabled */
#define TAS6754_DC_LDG_SLOL_DISABLE                 (0x01 << 1)  /* 1: Shorted load, open load and line out detection are disabled */

/* LDG BYPASS values (bit 0) */
#define TAS6754_DC_LDG_BYPASS_DISABLE               (0x00 << 0)  /* 0: Automatic DC diagnostic after a channel fault occurs in Hi-Z or PLAY state */
#define TAS6754_DC_LDG_BYPASS_ENABLE                (0x01 << 0)  /* 1: DC diagnostic will not run automatically */

/* Reset value */
#define TAS6754_DC_LDG_CTRL_RESET                   0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_LO_CTRL (0xB1)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_LO_RESERVED_7_MASK           BIT(7)          /* Bit 7: Reserved */
#define TAS6754_DC_LDG_LO_RESERVED_6_MASK           BIT(6)          /* Bit 6: Reserved */
#define TAS6754_DC_LDG_LO_RESERVED_5_4_MASK         GENMASK(5, 4)   /* Bits 5-4: Reserved */
#define TAS6754_DC_LDG_LO_CH1_ENABLE_MASK           BIT(3)          /* Bit 3: CH1 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH2_ENABLE_MASK           BIT(2)          /* Bit 2: CH2 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH3_ENABLE_MASK           BIT(1)          /* Bit 1: CH3 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH4_ENABLE_MASK           BIT(0)          /* Bit 0: CH4 LO LDG ENABLE */

/* CH1 LO LDG ENABLE values (bit 3) */
#define TAS6754_DC_LDG_LO_CH1_DISABLE               (0x00 << 3)  /* 0: Disable DC Load Diagnostics for line-out load on Channel 1 */
#define TAS6754_DC_LDG_LO_CH1_ENABLE                (0x01 << 3)  /* 1: Enable DC Load Diagnostics for line-out load on Channel 1 */

/* CH2 LO LDG ENABLE values (bit 2) */
#define TAS6754_DC_LDG_LO_CH2_DISABLE               (0x00 << 2)  /* 0: Disable DC Load Diagnostics for line-out load on Channel 2 */
#define TAS6754_DC_LDG_LO_CH2_ENABLE                (0x01 << 2)  /* 1: Enable DC Load Diagnostics for line-out load on Channel 2 */

/* CH3 LO LDG ENABLE values (bit 1) */
#define TAS6754_DC_LDG_LO_CH3_DISABLE               (0x00 << 1)  /* 0: Disable DC Load Diagnostics for line-out load on Channel 3 */
#define TAS6754_DC_LDG_LO_CH3_ENABLE                (0x01 << 1)  /* 1: Enable DC Load Diagnostics for line-out load on Channel 3 */

/* CH4 LO LDG ENABLE values (bit 0) */
#define TAS6754_DC_LDG_LO_CH4_DISABLE               (0x00 << 0)  /* 0: Disable DC Load Diagnostics for line-out load on Channel 4 */
#define TAS6754_DC_LDG_LO_CH4_ENABLE                (0x01 << 0)  /* 1: Enable DC Load Diagnostics for line-out load on Channel 4 */

/* Combined channel mask (for convenience) */
#define TAS6754_DC_LDG_LO_ALL_CH_MASK               (TAS6754_DC_LDG_LO_CH1_ENABLE_MASK | \
                                                    TAS6754_DC_LDG_LO_CH2_ENABLE_MASK | \
                                                    TAS6754_DC_LDG_LO_CH3_ENABLE_MASK | \
                                                    TAS6754_DC_LDG_LO_CH4_ENABLE_MASK)

/* Combined reserved bits mask (for convenience) */
#define TAS6754_DC_LDG_LO_RESERVED_MASK             (TAS6754_DC_LDG_LO_RESERVED_7_MASK | \
                                                    TAS6754_DC_LDG_LO_RESERVED_6_MASK | \
                                                    TAS6754_DC_LDG_LO_RESERVED_5_4_MASK)

/* Reset value */
#define TAS6754_DC_LDG_LO_CTRL_RESET                0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_TIME_CTRL (0xB2)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_RAMP_SL_OL_MASK              GENMASK(7, 6)   /* Bits 7-6: LDG RAMP SL OL */
#define TAS6754_DC_LDG_SETTLING_SL_OL_MASK          GENMASK(5, 4)   /* Bits 5-4: LDG SETTLING SL OL */
#define TAS6754_DC_LDG_RAMP_S2PG_MASK               GENMASK(3, 2)   /* Bits 3-2: LDG RAMP S2PG */
#define TAS6754_DC_LDG_SETTLING_S2PG_MASK           GENMASK(1, 0)   /* Bits 1-0: LDG SETTLING S2PG */

/* LDG RAMP SL OL values (bits 7-6) - Ramp time for shorted load and open load diagnostics */
#define TAS6754_DC_LDG_RAMP_SL_OL_15MS              (0x00 << 6)  /* 00: 15 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_30MS              (0x01 << 6)  /* 01: 30 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_10MS              (0x02 << 6)  /* 10: 10 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_20MS              (0x03 << 6)  /* 11: 20 ms */

/* LDG SETTLING SL OL values (bits 5-4) - Settling time for shorted load and open load diagnostics */
#define TAS6754_DC_LDG_SETTLING_SL_OL_10MS          (0x00 << 4)  /* 00: 10 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_5MS           (0x01 << 4)  /* 01: 5 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_20MS          (0x02 << 4)  /* 10: 20 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_15MS          (0x03 << 4)  /* 11: 15 ms */

/* LDG RAMP S2PG values (bits 3-2) - Ramp time for short to power and short to ground diagnostics */
#define TAS6754_DC_LDG_RAMP_S2PG_5MS                (0x00 << 2)  /* 00: 5 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_2_5MS              (0x01 << 2)  /* 01: 2.5 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_10MS               (0x02 << 2)  /* 10: 10 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_15MS               (0x03 << 2)  /* 11: 15 ms */

/* LDG SETTLING S2PG values (bits 1-0) - Settling time for short to power and short to ground diagnostics */
#define TAS6754_DC_LDG_SETTLING_S2PG_10MS           (0x00 << 0)  /* 00: 10 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_5MS            (0x01 << 0)  /* 01: 5 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_20MS           (0x02 << 0)  /* 10: 20 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_30MS           (0x03 << 0)  /* 11: 30 ms */

/* Reset value */
#define TAS6754_DC_LDG_TIME_CTRL_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_SL_CH1_CH2_CTRL (0xB3)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_SL_CH1_MASK                  GENMASK(7, 4)   /* Bits 7-4: CH1 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH2_MASK                  GENMASK(3, 0)   /* Bits 3-0: CH2 DC LDG SL */

/* CH1 DC LDG SL values (bits 7-4) - DC load diagnostic shorted-load threshold Channel 1 */
#define TAS6754_DC_LDG_SL_CH1_0_5_OHM               (0x00 << 4)  /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH1_1_0_OHM               (0x01 << 4)  /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH1_1_5_OHM               (0x02 << 4)  /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH1_2_0_OHM               (0x03 << 4)  /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH1_2_5_OHM               (0x04 << 4)  /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH1_3_0_OHM               (0x05 << 4)  /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH1_3_5_OHM               (0x06 << 4)  /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH1_4_0_OHM               (0x07 << 4)  /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH1_4_5_OHM               (0x08 << 4)  /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH1_5_0_OHM               (0x09 << 4)  /* 1001: 5.0Ω */

/* CH2 DC LDG SL values (bits 3-0) - DC load diagnostic shorted-load threshold Channel 2 */
#define TAS6754_DC_LDG_SL_CH2_0_5_OHM               (0x00 << 0)  /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH2_1_0_OHM               (0x01 << 0)  /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH2_1_5_OHM               (0x02 << 0)  /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH2_2_0_OHM               (0x03 << 0)  /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH2_2_5_OHM               (0x04 << 0)  /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH2_3_0_OHM               (0x05 << 0)  /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH2_3_5_OHM               (0x06 << 0)  /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH2_4_0_OHM               (0x07 << 0)  /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH2_4_5_OHM               (0x08 << 0)  /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH2_5_0_OHM               (0x09 << 0)  /* 1001: 5.0Ω */

/* Helper macro to set CH1 threshold value (0-9 for 0.5Ω to 5.0Ω) */
#define TAS6754_DC_LDG_SL_CH1_SET(val)              ((val & 0x0F) << 4)

/* Helper macro to set CH2 threshold value (0-9 for 0.5Ω to 5.0Ω) */
#define TAS6754_DC_LDG_SL_CH2_SET(val)              (val & 0x0F)

/* Reset value */
#define TAS6754_DC_LDG_SL_CH1_CH2_CTRL_RESET        0x11            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_SL_CH3_CH4_CTRL (0xB4)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_SL_CH3_MASK                  GENMASK(7, 4)   /* Bits 7-4: CH3 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH4_MASK                  GENMASK(3, 0)   /* Bits 3-0: CH4 DC LDG SL */

/* CH3 DC LDG SL values (bits 7-4) - DC load diagnostic shorted-load threshold Channel 3 */
#define TAS6754_DC_LDG_SL_CH3_0_5_OHM               (0x00 << 4)  /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH3_1_0_OHM               (0x01 << 4)  /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH3_1_5_OHM               (0x02 << 4)  /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH3_2_0_OHM               (0x03 << 4)  /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH3_2_5_OHM               (0x04 << 4)  /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH3_3_0_OHM               (0x05 << 4)  /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH3_3_5_OHM               (0x06 << 4)  /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH3_4_0_OHM               (0x07 << 4)  /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH3_4_5_OHM               (0x08 << 4)  /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH3_5_0_OHM               (0x09 << 4)  /* 1001: 5.0Ω */

/* CH4 DC LDG SL values (bits 3-0) - DC load diagnostic shorted-load threshold Channel 4 */
#define TAS6754_DC_LDG_SL_CH4_0_5_OHM               (0x00 << 0)  /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH4_1_0_OHM               (0x01 << 0)  /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH4_1_5_OHM               (0x02 << 0)  /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH4_2_0_OHM               (0x03 << 0)  /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH4_2_5_OHM               (0x04 << 0)  /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH4_3_0_OHM               (0x05 << 0)  /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH4_3_5_OHM               (0x06 << 0)  /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH4_4_0_OHM               (0x07 << 0)  /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH4_4_5_OHM               (0x08 << 0)  /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH4_5_0_OHM               (0x09 << 0)  /* 1001: 5.0Ω */

/* Helper macro to set CH3 threshold value (0-9 for 0.5Ω to 5.0Ω) */
#define TAS6754_DC_LDG_SL_CH3_SET(val)              ((val & 0x0F) << 4)

/* Helper macro to set CH4 threshold value (0-9 for 0.5Ω to 5.0Ω) */
#define TAS6754_DC_LDG_SL_CH4_SET(val)              (val & 0x0F)

/* Reset value */
#define TAS6754_DC_LDG_SL_CH3_CH4_CTRL_RESET        0x11            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_CTRL (0xB5)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_RESERVED_MASK                GENMASK(7, 5)   /* Bits 7-5: Reserved */
#define TAS6754_AC_LDG_DIAG_GAIN_MASK               BIT(4)          /* Bit 4: AC DIAG GAIN */
#define TAS6754_AC_LDG_CH1_DIAG_START_MASK          BIT(3)          /* Bit 3: CH1 AC DIAG START */
#define TAS6754_AC_LDG_CH2_DIAG_START_MASK          BIT(2)          /* Bit 2: CH2 AC DIAG START */
#define TAS6754_AC_LDG_CH3_DIAG_START_MASK          BIT(1)          /* Bit 1: CH3 AC DIAG START */
#define TAS6754_AC_LDG_CH4_DIAG_START_MASK          BIT(0)          /* Bit 0: CH4 AC DIAG START */

/* AC DIAG GAIN values (bit 4) */
#define TAS6754_AC_LDG_DIAG_GAIN_1                  (0x00 << 4)  /* 0: Gain 1 */
#define TAS6754_AC_LDG_DIAG_GAIN_8                  (0x01 << 4)  /* 1: Gain 8 */

/* CH1 AC DIAG START values (bit 3) */
#define TAS6754_AC_LDG_CH1_NORMAL                   (0x00 << 3)  /* 0: Normal operation */
#define TAS6754_AC_LDG_CH1_START                    (0x01 << 3)  /* 1: Start AC diagnostic on Channel 1 */

/* CH2 AC DIAG START values (bit 2) */
#define TAS6754_AC_LDG_CH2_NORMAL                   (0x00 << 2)  /* 0: Normal operation */
#define TAS6754_AC_LDG_CH2_START                    (0x01 << 2)  /* 1: Start AC diagnostic on Channel 2 */

/* CH3 AC DIAG START values (bit 1) */
#define TAS6754_AC_LDG_CH3_NORMAL                   (0x00 << 1)  /* 0: Normal operation */
#define TAS6754_AC_LDG_CH3_START                    (0x01 << 1)  /* 1: Start AC diagnostic on Channel 3 */

/* CH4 AC DIAG START values (bit 0) */
#define TAS6754_AC_LDG_CH4_NORMAL                   (0x00 << 0)  /* 0: Normal operation */
#define TAS6754_AC_LDG_CH4_START                    (0x01 << 0)  /* 1: Start AC diagnostic on Channel 4 */

/* Combined channel start mask (for convenience) */
#define TAS6754_AC_LDG_ALL_CH_START_MASK            (TAS6754_AC_LDG_CH1_DIAG_START_MASK | \
                                                    TAS6754_AC_LDG_CH2_DIAG_START_MASK | \
                                                    TAS6754_AC_LDG_CH3_DIAG_START_MASK | \
                                                    TAS6754_AC_LDG_CH4_DIAG_START_MASK)

/* Reset value */
#define TAS6754_AC_LDG_CTRL_RESET                   0x10            /* Reset value for the register */
/************************************************************************
* 					TAS6754_TWEETER_DETECT_CTRL (0xB6)
************************************************************************/
/* Bit field masks */
#define TAS6754_TWEETER_DETECT_RESERVED_7_4_MASK    GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_TWEETER_DETECT_AVG_MASK             BIT(3)          /* Bit 3: TWEETER DETECT AVG */
#define TAS6754_TWEETER_DETECT_RESERVED_2_MASK      BIT(2)          /* Bit 2: Reserved */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_MASK       BIT(1)          /* Bit 1: TWEETER DETECT CALC TYPE */
#define TAS6754_TWEETER_DETECT_DISABLE_MASK         BIT(0)          /* Bit 0: TWEETER DETECT DISABLE */

/* TWEETER DETECT AVG values (bit 3) */
#define TAS6754_TWEETER_DETECT_AVG_4096             (0x00 << 3)  /* 0: 4096-sample averaging in decimation filter */
#define TAS6754_TWEETER_DETECT_AVG_32768            (0x01 << 3)  /* 1: 32768-sample averaging in decimation filter */

/* TWEETER DETECT CALC TYPE values (bit 1) */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_2          (0x00 << 1)  /* 0: AC pass/fail judgement type 2 - Calculate magnitude as Re(Z)+0.5*Im(Z) */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_1          (0x01 << 1)  /* 1: AC pass/fail judgement type 1 - Calculate magnitude as Re(Z) */

/* TWEETER DETECT DISABLE values (bit 0) */
#define TAS6754_TWEETER_DETECT_ENABLE               (0x00 << 0)  /* 0: Enable Tweeter detection judgement */
#define TAS6754_TWEETER_DETECT_DISABLE              (0x01 << 0)  /* 1: Disable Tweeter detection calculation */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_TWEETER_DETECT_RESERVED_MASK        (TAS6754_TWEETER_DETECT_RESERVED_7_4_MASK | \
                                                    TAS6754_TWEETER_DETECT_RESERVED_2_MASK)

/* Reset value */
#define TAS6754_TWEETER_DETECT_CTRL_RESET           0x08            /* Reset value for the register */
/************************************************************************
* 					TAS6754_TWEETER_DETECT_THRESH (0xB7)
************************************************************************/
/* Bit field masks */
#define TAS6754_TWEETER_DETECT_THRESH_MASK          0xFF            /* Bits 7-0: TWEETER DETECT THRESHOLD */

/* Helper macros for threshold calculation */
#define TAS6754_TWEETER_DETECT_THRESH_GAIN_1(ohms)  ((uint8_t)((ohms) / 0.8))  /* Convert ohms to register value when AC DIAG GAIN = 0 */
#define TAS6754_TWEETER_DETECT_THRESH_GAIN_8(ohms)  ((uint8_t)((ohms) / 0.1))  /* Convert ohms to register value when AC DIAG GAIN = 1 */

/* Common threshold values for Gain = 1 (0.8Ω per code) */
#define TAS6754_TWEETER_DETECT_THRESH_0_8_OHM_G1    0x01            /* 0.8Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_1_6_OHM_G1    0x02            /* 1.6Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_4_0_OHM_G1    0x05            /* 4.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_8_0_OHM_G1    0x0A            /* 8.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_16_0_OHM_G1   0x14            /* 16.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_40_0_OHM_G1   0x32            /* 40.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_80_0_OHM_G1   0x64            /* 80.0Ω with Gain = 1 */

/* Common threshold values for Gain = 8 (0.1Ω per code) */
#define TAS6754_TWEETER_DETECT_THRESH_0_1_OHM_G8    0x01            /* 0.1Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_0_5_OHM_G8    0x05            /* 0.5Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_1_0_OHM_G8    0x0A            /* 1.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_2_0_OHM_G8    0x14            /* 2.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_5_0_OHM_G8    0x32            /* 5.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_10_0_OHM_G8   0x64            /* 10.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_20_0_OHM_G8   0xC8            /* 20.0Ω with Gain = 8 */

/* Reset value */
#define TAS6754_TWEETER_DETECT_THRESH_RESET         0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_FREQ_CTRL (0xB8)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_STIMULUS_FREQ_MASK           0xFF            /* Bits 7-0: AC LDG STIMULUS FREQUENCY */

/* Helper macro for frequency calculation */
#define TAS6754_AC_LDG_FREQ_HZ(freq)                ((uint8_t)((freq) / 93.75))  /* Convert frequency in Hz to register value */

/* Common frequency values */
#define TAS6754_AC_LDG_FREQ_93_75_HZ                0x01            /* 93.75 Hz */
#define TAS6754_AC_LDG_FREQ_187_5_HZ                0x02            /* 187.5 Hz */
#define TAS6754_AC_LDG_FREQ_281_25_HZ               0x03            /* 281.25 Hz */
#define TAS6754_AC_LDG_FREQ_500_HZ                  0x05            /* ~500 Hz (468.75 Hz) */
#define TAS6754_AC_LDG_FREQ_1_KHZ                   0x0B            /* ~1 kHz (1031.25 Hz) */
#define TAS6754_AC_LDG_FREQ_2_KHZ                   0x15            /* ~2 kHz (1968.75 Hz) */
#define TAS6754_AC_LDG_FREQ_5_KHZ                   0x35            /* ~5 kHz (4921.88 Hz) */
#define TAS6754_AC_LDG_FREQ_10_KHZ                  0x6A            /* ~10 kHz (9843.75 Hz) */
#define TAS6754_AC_LDG_FREQ_18_75_KHZ               0xC8            /* 18.75 kHz */
#define TAS6754_AC_LDG_FREQ_20_KHZ                  0xD5            /* ~20 kHz (19687.5 Hz) */
#define TAS6754_AC_LDG_FREQ_23_91_KHZ               0xFF            /* 23.91 kHz */

/* Reset value */
#define TAS6754_AC_LDG_FREQ_CTRL_RESET              0xC8            /* Reset value for the register (18.75 kHz) */
/************************************************************************
* 					TAS6754_TEMP_CH1_CH2 (0xBB)
************************************************************************/
/* Bit field masks */
#define TAS6754_TEMP_CH1_CH2_RESERVED_MASK          GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_TEMP_CH2_MASK                       GENMASK(5, 3)   /* Bits 5-3: CH2 TEMP */
#define TAS6754_TEMP_CH1_MASK                       GENMASK(2, 0)   /* Bits 2-0: CH1 TEMP */

/* Temperature range values for CH2 (bits 5-3) */
#define TAS6754_TEMP_CH2_BELOW_95C                  (0x00 << 3)  /* 000: < 95°C */
#define TAS6754_TEMP_CH2_95C_110C                   (0x01 << 3)  /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH2_110C_125C                  (0x02 << 3)  /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH2_125C_135C                  (0x03 << 3)  /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH2_135C_145C                  (0x04 << 3)  /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH2_145C_155C                  (0x05 << 3)  /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH2_155C_165C                  (0x06 << 3)  /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH2_ABOVE_165C                 (0x07 << 3)  /* 111: > 165°C */

/* Temperature range values for CH1 (bits 2-0) */
#define TAS6754_TEMP_CH1_BELOW_95C                  (0x00 << 0)  /* 000: < 95°C */
#define TAS6754_TEMP_CH1_95C_110C                   (0x01 << 0)  /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH1_110C_125C                  (0x02 << 0)  /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH1_125C_135C                  (0x03 << 0)  /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH1_135C_145C                  (0x04 << 0)  /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH1_145C_155C                  (0x05 << 0)  /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH1_155C_165C                  (0x06 << 0)  /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH1_ABOVE_165C                 (0x07 << 0)  /* 111: > 165°C */

/* Helper macros to extract temperature range values */
#define TAS6754_GET_TEMP_CH2(reg_val)               ((reg_val & TAS6754_TEMP_CH2_MASK) >> 3)
#define TAS6754_GET_TEMP_CH1(reg_val)               (reg_val & TAS6754_TEMP_CH1_MASK)

/* Reset value */
#define TAS6754_TEMP_CH1_CH2_RESET                  0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_TEMP_CH3_CH4 (0xBC)
************************************************************************/
/* Bit field masks */
#define TAS6754_TEMP_CH3_CH4_RESERVED_MASK          GENMASK(7, 6)   /* Bits 7-6: Reserved */
#define TAS6754_TEMP_CH4_MASK                       GENMASK(5, 3)   /* Bits 5-3: CH4 TEMP */
#define TAS6754_TEMP_CH3_MASK                       GENMASK(2, 0)   /* Bits 2-0: CH3 TEMP */

/* Temperature range values for CH4 (bits 5-3) */
#define TAS6754_TEMP_CH4_BELOW_95C                  (0x00 << 3)  /* 000: < 95°C */
#define TAS6754_TEMP_CH4_95C_110C                   (0x01 << 3)  /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH4_110C_125C                  (0x02 << 3)  /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH4_125C_135C                  (0x03 << 3)  /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH4_135C_145C                  (0x04 << 3)  /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH4_145C_155C                  (0x05 << 3)  /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH4_155C_165C                  (0x06 << 3)  /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH4_ABOVE_165C                 (0x07 << 3)  /* 111: > 165°C */

/* Temperature range values for CH3 (bits 2-0) */
#define TAS6754_TEMP_CH3_BELOW_95C                  (0x00 << 0)  /* 000: < 95°C */
#define TAS6754_TEMP_CH3_95C_110C                   (0x01 << 0)  /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH3_110C_125C                  (0x02 << 0)  /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH3_125C_135C                  (0x03 << 0)  /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH3_135C_145C                  (0x04 << 0)  /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH3_145C_155C                  (0x05 << 0)  /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH3_155C_165C                  (0x06 << 0)  /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH3_ABOVE_165C                 (0x07 << 0)  /* 111: > 165°C */

/* Helper macros to extract temperature range values */
#define TAS6754_GET_TEMP_CH4(reg_val)               ((reg_val & TAS6754_TEMP_CH4_MASK) >> 3)
#define TAS6754_GET_TEMP_CH3(reg_val)               (reg_val & TAS6754_TEMP_CH3_MASK)

/* Reset value */
#define TAS6754_TEMP_CH3_CH4_RESET                  0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_WARN_OT_MAX_FLAG (0xBD)
************************************************************************/
/* Bit field masks */
#define TAS6754_WARN_OT_MAX_RESERVED_MASK           GENMASK(7, 3)   /* Bits 7-3: Reserved */
#define TAS6754_WARN_OT_MAX_ALL_CH_TEMP_MASK        GENMASK(2, 0)   /* Bits 2-0: ALL CHANNEL MAX TEMP */

/* Temperature range values for ALL CHANNEL MAX TEMP (bits 2-0) */
#define TAS6754_WARN_OT_MAX_BELOW_95C               0x00            /* 000: < 95°C */
#define TAS6754_WARN_OT_MAX_95C_110C                0x01            /* 001: 95°C~110°C */
#define TAS6754_WARN_OT_MAX_110C_125C               0x02            /* 010: 110°C~125°C */
#define TAS6754_WARN_OT_MAX_125C_135C               0x03            /* 011: 125°C~135°C */
#define TAS6754_WARN_OT_MAX_135C_145C               0x04            /* 100: 135°C~145°C */
#define TAS6754_WARN_OT_MAX_145C_155C               0x05            /* 101: 145°C~155°C */
#define TAS6754_WARN_OT_MAX_155C_165C               0x06            /* 110: 155°C~165°C */
#define TAS6754_WARN_OT_MAX_ABOVE_165C              0x07            /* 111: > 165°C */

/* Reset value */
#define TAS6754_WARN_OT_MAX_FLAG_RESET              0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_REPORT_CH1_CH2 (0xC0)
************************************************************************/
/* Bit field masks - Channel 1 */
#define TAS6754_DC_LDG_CH1_S2G_MASK                 BIT(7)          /* Bit 7: CH1 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH1_S2P_MASK                 BIT(6)          /* Bit 6: CH1 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH1_OL_MASK                  BIT(5)          /* Bit 5: CH1 OL (Open Load) */
#define TAS6754_DC_LDG_CH1_SL_MASK                  BIT(4)          /* Bit 4: CH1 SL (Shorted Load) */

/* Bit field masks - Channel 2 */
#define TAS6754_DC_LDG_CH2_S2G_MASK                 BIT(3)          /* Bit 3: CH2 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH2_S2P_MASK                 BIT(2)          /* Bit 2: CH2 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH2_OL_MASK                  BIT(1)          /* Bit 1: CH2 OL (Open Load) */
#define TAS6754_DC_LDG_CH2_SL_MASK                  BIT(0)          /* Bit 0: CH2 SL (Shorted Load) */

/* Status values - Channel 1 */
#define TAS6754_DC_LDG_CH1_S2G_NOT_DETECTED         (0x00 << 7)  /* 0: No short-to-GND detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2G_DETECTED             (0x01 << 7)  /* 1: Short-to-GND detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2P_NOT_DETECTED         (0x00 << 6)  /* 0: No short-to-power detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2P_DETECTED             (0x01 << 6)  /* 1: Short-to-power detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_OL_NOT_DETECTED          (0x00 << 5)  /* 0: No open load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_OL_DETECTED              (0x01 << 5)  /* 1: Open load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_SL_NOT_DETECTED          (0x00 << 4)  /* 0: No shorted load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_SL_DETECTED              (0x01 << 4)  /* 1: Shorted load detected on Channel 1 */

/* Status values - Channel 2 */
#define TAS6754_DC_LDG_CH2_S2G_NOT_DETECTED         (0x00 << 3)  /* 0: No short-to-GND detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2G_DETECTED             (0x01 << 3)  /* 1: Short-to-GND detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2P_NOT_DETECTED         (0x00 << 2)  /* 0: No short-to-power detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2P_DETECTED             (0x01 << 2)  /* 1: Short-to-power detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_OL_NOT_DETECTED          (0x00 << 1)  /* 0: No open load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_OL_DETECTED              (0x01 << 1)  /* 1: Open load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_SL_NOT_DETECTED          (0x00 << 0)  /* 0: No shorted load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_SL_DETECTED              (0x01 << 0)  /* 1: Shorted load detected on Channel 2 */

/* Combined masks (for convenience) */
#define TAS6754_DC_LDG_CH1_ALL_MASK                 (TAS6754_DC_LDG_CH1_S2G_MASK | \
                                                    TAS6754_DC_LDG_CH1_S2P_MASK | \
                                                    TAS6754_DC_LDG_CH1_OL_MASK | \
                                                    TAS6754_DC_LDG_CH1_SL_MASK)

#define TAS6754_DC_LDG_CH2_ALL_MASK                 (TAS6754_DC_LDG_CH2_S2G_MASK | \
                                                    TAS6754_DC_LDG_CH2_S2P_MASK | \
                                                    TAS6754_DC_LDG_CH2_OL_MASK | \
                                                    TAS6754_DC_LDG_CH2_SL_MASK)

/* Reset value */
#define TAS6754_DC_LDG_REPORT_CH1_CH2_RESET         0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_REPORT_CH3_CH4 (0xC1)
************************************************************************/
/* Bit field masks - Channel 3 */
#define TAS6754_DC_LDG_CH3_S2G_MASK                 BIT(7)          /* Bit 7: CH3 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH3_S2P_MASK                 BIT(6)          /* Bit 6: CH3 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH3_OL_MASK                  BIT(5)          /* Bit 5: CH3 OL (Open Load) */
#define TAS6754_DC_LDG_CH3_SL_MASK                  BIT(4)          /* Bit 4: CH3 SL (Shorted Load) */

/* Bit field masks - Channel 4 */
#define TAS6754_DC_LDG_CH4_S2G_MASK                 BIT(3)          /* Bit 3: CH4 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH4_S2P_MASK                 BIT(2)          /* Bit 2: CH4 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH4_OL_MASK                  BIT(1)          /* Bit 1: CH4 OL (Open Load) */
#define TAS6754_DC_LDG_CH4_SL_MASK                  BIT(0)          /* Bit 0: CH4 SL (Shorted Load) */

/* Status values - Channel 3 */
#define TAS6754_DC_LDG_CH3_S2G_NOT_DETECTED         (0x00 << 7)  /* 0: No short-to-GND detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2G_DETECTED             (0x01 << 7)  /* 1: Short-to-GND detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2P_NOT_DETECTED         (0x00 << 6)  /* 0: No short-to-power detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2P_DETECTED             (0x01 << 6)  /* 1: Short-to-power detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_OL_NOT_DETECTED          (0x00 << 5)  /* 0: No open load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_OL_DETECTED              (0x01 << 5)  /* 1: Open load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_SL_NOT_DETECTED          (0x00 << 4)  /* 0: No shorted load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_SL_DETECTED              (0x01 << 4)  /* 1: Shorted load detected on Channel 3 */

/* Status values - Channel 4 */
#define TAS6754_DC_LDG_CH4_S2G_NOT_DETECTED         (0x00 << 3)  /* 0: No short-to-GND detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2G_DETECTED             (0x01 << 3)  /* 1: Short-to-GND detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2P_NOT_DETECTED         (0x00 << 2)  /* 0: No short-to-power detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2P_DETECTED             (0x01 << 2)  /* 1: Short-to-power detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_OL_NOT_DETECTED          (0x00 << 1)  /* 0: No open load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_OL_DETECTED              (0x01 << 1)  /* 1: Open load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_SL_NOT_DETECTED          (0x00 << 0)  /* 0: No shorted load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_SL_DETECTED              (0x01 << 0)  /* 1: Shorted load detected on Channel 4 */

/* Combined masks (for convenience) */
#define TAS6754_DC_LDG_CH3_ALL_MASK                 (TAS6754_DC_LDG_CH3_S2G_MASK | \
                                                    TAS6754_DC_LDG_CH3_S2P_MASK | \
                                                    TAS6754_DC_LDG_CH3_OL_MASK | \
                                                    TAS6754_DC_LDG_CH3_SL_MASK)

#define TAS6754_DC_LDG_CH4_ALL_MASK                 (TAS6754_DC_LDG_CH4_S2G_MASK | \
                                                    TAS6754_DC_LDG_CH4_S2P_MASK | \
                                                    TAS6754_DC_LDG_CH4_OL_MASK | \
                                                    TAS6754_DC_LDG_CH4_SL_MASK)

/* Reset value */
#define TAS6754_DC_LDG_REPORT_CH3_CH4_RESET         0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_DC_LDG_RESULT (0xC2)
************************************************************************/
/* Bit field masks - Line-out Load Detection Results */
#define TAS6754_DC_LDG_CH1_LO_RESULT_MASK           BIT(7)          /* Bit 7: CH1 LO LDG RESULT */
#define TAS6754_DC_LDG_CH2_LO_RESULT_MASK           BIT(6)          /* Bit 6: CH2 LO LDG RESULT */
#define TAS6754_DC_LDG_CH3_LO_RESULT_MASK           BIT(5)          /* Bit 5: CH3 LO LDG RESULT */
#define TAS6754_DC_LDG_CH4_LO_RESULT_MASK           BIT(4)          /* Bit 4: CH4 LO LDG RESULT */

/* Bit field masks - DC Load Diagnostic Results */
#define TAS6754_DC_LDG_CH1_RESULT_MASK              BIT(3)          /* Bit 3: CH1 DC LDG RESULT */
#define TAS6754_DC_LDG_CH2_RESULT_MASK              BIT(2)          /* Bit 2: CH2 DC LDG RESULT */
#define TAS6754_DC_LDG_CH3_RESULT_MASK              BIT(1)          /* Bit 1: CH3 DC LDG RESULT */
#define TAS6754_DC_LDG_CH4_RESULT_MASK              BIT(0)          /* Bit 0: CH4 DC LDG RESULT */

/* Line-out Load Detection Result values */
#define TAS6754_DC_LDG_CH1_LO_NOT_DETECTED          (0x00 << 7)  /* 0: Lineout load not detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_LO_DETECTED              (0x01 << 7)  /* 1: Lineout load detected on Channel 1 */
#define TAS6754_DC_LDG_CH2_LO_NOT_DETECTED          (0x00 << 6)  /* 0: Lineout load not detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_LO_DETECTED              (0x01 << 6)  /* 1: Lineout load detected on Channel 2 */
#define TAS6754_DC_LDG_CH3_LO_NOT_DETECTED          (0x00 << 5)  /* 0: Lineout load not detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_LO_DETECTED              (0x01 << 5)  /* 1: Lineout load detected on Channel 3 */
#define TAS6754_DC_LDG_CH4_LO_NOT_DETECTED          (0x00 << 4)  /* 0: Lineout load not detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_LO_DETECTED              (0x01 << 4)  /* 1: Lineout load detected on Channel 4 */

/* DC Load Diagnostic Result values */
#define TAS6754_DC_LDG_CH1_FAILED                   (0x00 << 3)  /* 0: DC Load Diagnostic did not complete without faults on Channel 1 */
#define TAS6754_DC_LDG_CH1_PASSED                   (0x01 << 3)  /* 1: DC Load Diagnostic completed without faults on Channel 1 */
#define TAS6754_DC_LDG_CH2_FAILED                   (0x00 << 2)  /* 0: DC Load Diagnostic did not complete without faults on Channel 2 */
#define TAS6754_DC_LDG_CH2_PASSED                   (0x01 << 2)  /* 1: DC Load Diagnostic completed without faults on Channel 2 */
#define TAS6754_DC_LDG_CH3_FAILED                   (0x00 << 1)  /* 0: DC Load Diagnostic did not complete without faults on Channel 3 */
#define TAS6754_DC_LDG_CH3_PASSED                   (0x01 << 1)  /* 1: DC Load Diagnostic completed without faults on Channel 3 */
#define TAS6754_DC_LDG_CH4_FAILED                   (0x00 << 0)  /* 0: DC Load Diagnostic did not complete without faults on Channel 4 */
#define TAS6754_DC_LDG_CH4_PASSED                   (0x01 << 0)  /* 1: DC Load Diagnostic completed without faults on Channel 4 */

/* Combined masks (for convenience) */
#define TAS6754_DC_LDG_ALL_LO_RESULT_MASK           (TAS6754_DC_LDG_CH1_LO_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH2_LO_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH3_LO_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH4_LO_RESULT_MASK)

#define TAS6754_DC_LDG_ALL_RESULT_MASK              (TAS6754_DC_LDG_CH1_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH2_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH3_RESULT_MASK | \
                                                    TAS6754_DC_LDG_CH4_RESULT_MASK)

/* Reset value */
#define TAS6754_DC_LDG_RESULT_RESET                 0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH1_R (0xC3)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH1_R_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH1_R_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH1_R_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH1_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH1_R_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH1_R_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH1_R_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH1_R_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH1_I (0xC4)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH1_I_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH1_I_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH1_I_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH1_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH1_I_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH1_I_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH1_I_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH1_I_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH2_R (0xC5)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH2_R_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH2_R_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH2_R_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH2_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH2_R_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH2_R_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH2_R_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH2_R_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH2_I (0xC6)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH2_I_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH2_I_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH2_I_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH2_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH2_I_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH2_I_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH2_I_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH2_I_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH3_R (0xC7)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH3_R_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH3_R_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH3_R_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH3_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH3_R_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH3_R_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH3_R_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH3_R_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH3_I (0xC8)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH3_I_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH3_I_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH3_I_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH3_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH3_I_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH3_I_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH3_I_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH3_I_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH4_R (0xC9)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH4_R_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH4_R_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH4_R_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH4_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH4_R_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH4_R_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH4_R_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH4_R_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH4_I (0xCA)
************************************************************************/
/* Bit field masks */
#define TAS6754_AC_LDG_CH4_I_SIGN_MASK              BIT(7)          /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH4_I_VALUE_MASK             GENMASK(6, 0)   /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH4_I_MASK                   0xFF            /* Bits 7-0: Full value including sign */

/* Helper macros for impedance calculation */
#define TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val)       ((val) & TAS6754_AC_LDG_CH4_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val)     ((val) & TAS6754_AC_LDG_CH4_I_VALUE_MASK)

/* Conversion macros for different gain settings */
#define TAS6754_AC_LDG_CH4_I_TO_OHMS_GAIN_1(val)    ((TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val) * 0.8f)

#define TAS6754_AC_LDG_CH4_I_TO_OHMS_GAIN_8(val)    ((TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                    TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val) * 0.1f)

/* Reset value */
#define TAS6754_AC_LDG_REPORT_CH4_I_RESET           0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_TWEETER_REPORT (0xCB)
************************************************************************/
/* Bit field masks */
#define TAS6754_TWEETER_REPORT_RESERVED_MASK        GENMASK(7, 4)   /* Bits 7-4: Reserved */
#define TAS6754_TWEETER_REPORT_CH1_MASK             BIT(3)          /* Bit 3: CH1 TW DET */
#define TAS6754_TWEETER_REPORT_CH2_MASK             BIT(2)          /* Bit 2: CH2 TW DET */
#define TAS6754_TWEETER_REPORT_CH3_MASK             BIT(1)          /* Bit 1: CH3 TW DET */
#define TAS6754_TWEETER_REPORT_CH4_MASK             BIT(0)          /* Bit 0: CH4 TW DET */

/* Tweeter detection status values */
#define TAS6754_TWEETER_REPORT_CH1_NOT_DETECTED     (0x00 << 3)  /* 0: No tweeter detected on Channel 1 */
#define TAS6754_TWEETER_REPORT_CH1_DETECTED         (0x01 << 3)  /* 1: Tweeter detected on Channel 1 */
#define TAS6754_TWEETER_REPORT_CH2_NOT_DETECTED     (0x00 << 2)  /* 0: No tweeter detected on Channel 2 */
#define TAS6754_TWEETER_REPORT_CH2_DETECTED         (0x01 << 2)  /* 1: Tweeter detected on Channel 2 */
#define TAS6754_TWEETER_REPORT_CH3_NOT_DETECTED     (0x00 << 1)  /* 0: No tweeter detected on Channel 3 */
#define TAS6754_TWEETER_REPORT_CH3_DETECTED         (0x01 << 1)  /* 1: Tweeter detected on Channel 3 */
#define TAS6754_TWEETER_REPORT_CH4_NOT_DETECTED     (0x00 << 0)  /* 0: No tweeter detected on Channel 4 */
#define TAS6754_TWEETER_REPORT_CH4_DETECTED         (0x01 << 0)  /* 1: Tweeter detected on Channel 4 */

/* Combined channel mask (for convenience) */
#define TAS6754_TWEETER_REPORT_ALL_CH_MASK          (TAS6754_TWEETER_REPORT_CH1_MASK | \
                                                    TAS6754_TWEETER_REPORT_CH2_MASK | \
                                                    TAS6754_TWEETER_REPORT_CH3_MASK | \
                                                    TAS6754_TWEETER_REPORT_CH4_MASK)

/* Reset value */
#define TAS6754_TWEETER_REPORT_RESET                0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CH1_RTLDG_IMP_MSB (0xD1)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH1_RTLDG_IMP_MSB_MASK              0xFF            /* Bits 7-0: CH1 RTLDG IMPEDANCE MSB */

/* Reset value */
#define TAS6754_CH1_RTLDG_IMP_MSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation (to be used with LSB register) */
#define TAS6754_CH1_RTLDG_IMP_GET_MSB(val)          ((val) & 0xFF)
#define TAS6754_CH1_RTLDG_IMP_COMBINE(msb, lsb)     (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH1_RTLDG_IMP_LSB (0xD2)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH1_RTLDG_IMP_LSB_MASK              0xFF            /* Bits 7-0: CH1 RTLDG IMPEDANCE LSB */

/* Reset value */
#define TAS6754_CH1_RTLDG_IMP_LSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation */
#define TAS6754_CH1_RTLDG_IMP_GET_LSB(val)          ((val) & 0xFF)
#define TAS6754_CH1_RTLDG_IMP_TO_OHMS(msb, lsb)     (((float)TAS6754_CH1_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH2_RTLDG_IMP_MSB (0xD3)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH2_RTLDG_IMP_MSB_MASK              0xFF            /* Bits 7-0: CH2 RTLDG IMPEDANCE MSB */

/* Reset value */
#define TAS6754_CH2_RTLDG_IMP_MSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation (to be used with LSB register) */
#define TAS6754_CH2_RTLDG_IMP_GET_MSB(val)          ((val) & 0xFF)
#define TAS6754_CH2_RTLDG_IMP_COMBINE(msb, lsb)     (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH2_RTLDG_IMP_LSB (0xD4)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH2_RTLDG_IMP_LSB_MASK              0xFF            /* Bits 7-0: CH2 RTLDG IMPEDANCE LSB */

/* Reset value */
#define TAS6754_CH2_RTLDG_IMP_LSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation */
#define TAS6754_CH2_RTLDG_IMP_GET_LSB(val)          ((val) & 0xFF)
#define TAS6754_CH2_RTLDG_IMP_TO_OHMS(msb, lsb)     (((float)TAS6754_CH2_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH3_RTLDG_IMP_MSB (0xD5)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH3_RTLDG_IMP_MSB_MASK              0xFF            /* Bits 7-0: CH3 RTLDG IMPEDANCE MSB */

/* Reset value */
#define TAS6754_CH3_RTLDG_IMP_MSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation (to be used with LSB register) */
#define TAS6754_CH3_RTLDG_IMP_GET_MSB(val)          ((val) & 0xFF)
#define TAS6754_CH3_RTLDG_IMP_COMBINE(msb, lsb)     (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH3_RTLDG_IMP_LSB (0xD6)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH3_RTLDG_IMP_LSB_MASK              0xFF            /* Bits 7-0: CH3 RTLDG IMPEDANCE LSB */

/* Reset value */
#define TAS6754_CH3_RTLDG_IMP_LSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation */
#define TAS6754_CH3_RTLDG_IMP_GET_LSB(val)          ((val) & 0xFF)
#define TAS6754_CH3_RTLDG_IMP_TO_OHMS(msb, lsb)     (((float)TAS6754_CH3_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH4_RTLDG_IMP_MSB (0xD7)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH4_RTLDG_IMP_MSB_MASK              0xFF            /* Bits 7-0: CH4 RTLDG IMPEDANCE MSB */

/* Reset value */
#define TAS6754_CH4_RTLDG_IMP_MSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation (to be used with LSB register) */
#define TAS6754_CH4_RTLDG_IMP_GET_MSB(val)          ((val) & 0xFF)
#define TAS6754_CH4_RTLDG_IMP_COMBINE(msb, lsb)     (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH4_RTLDG_IMP_LSB (0xD8)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH4_RTLDG_IMP_LSB_MASK              0xFF            /* Bits 7-0: CH4 RTLDG IMPEDANCE LSB */

/* Reset value */
#define TAS6754_CH4_RTLDG_IMP_LSB_RESET             0x00            /* Reset value for the register */

/* Helper macros for impedance calculation */
#define TAS6754_CH4_RTLDG_IMP_GET_LSB(val)          ((val) & 0xFF)
#define TAS6754_CH4_RTLDG_IMP_TO_OHMS(msb, lsb)     (((float)TAS6754_CH4_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_DC_LDG_DCR_MSB (0xD9)
************************************************************************/
/* Bit field masks */
#define TAS6754_DC_LDG_DCR_CH1_MSB_MASK             GENMASK(7, 6)   /* Bits 7-6: CH1 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH2_MSB_MASK             GENMASK(5, 4)   /* Bits 5-4: CH2 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH3_MSB_MASK             GENMASK(3, 2)   /* Bits 3-2: CH3 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH4_MSB_MASK             GENMASK(1, 0)   /* Bits 1-0: CH4 DC RESISTANCE MSB */

/* Bit positions for shifting */
#define TAS6754_DC_LDG_DCR_CH1_MSB_SHIFT            6
#define TAS6754_DC_LDG_DCR_CH2_MSB_SHIFT            4
#define TAS6754_DC_LDG_DCR_CH3_MSB_SHIFT            2
#define TAS6754_DC_LDG_DCR_CH4_MSB_SHIFT            0

/* Helper macros for extracting MSB values */
#define TAS6754_DC_LDG_DCR_GET_CH1_MSB(val)         (((val) & TAS6754_DC_LDG_DCR_CH1_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH1_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH2_MSB(val)         (((val) & TAS6754_DC_LDG_DCR_CH2_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH2_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH3_MSB(val)         (((val) & TAS6754_DC_LDG_DCR_CH3_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH3_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH4_MSB(val)         (((val) & TAS6754_DC_LDG_DCR_CH4_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH4_MSB_SHIFT)

/* Helper macros for combining MSB and LSB values (to be used with LSB registers) */
#define TAS6754_DC_LDG_DCR_COMBINE(msb, lsb)        (((uint16_t)(msb) << 8) | (lsb))

/* Reset value */
#define TAS6754_DC_LDG_DCR_MSB_RESET                0x00            /* Reset value for the register */
/************************************************************************
* 					TAS6754_CH1_DC_LDG_DCR_LSB (0xDA)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH1_DC_LDG_DCR_LSB_MASK             0xFF            /* Bits 7-0: CH1 DC RESISTANCE LSB */

/* Reset value */
#define TAS6754_CH1_DC_LDG_DCR_LSB_RESET            0x00            /* Reset value for the register */

/* Helper macros for resistance calculation */
#define TAS6754_CH1_DC_LDG_DCR_GET_LSB(val)         ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit resistance value
 * msb_val should be the 2-bit value extracted from DC_LDG_DCR_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_CH1_DC_LDG_DCR_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Convert combined 10-bit value to ohms (0.1Ω/code) */
#define TAS6754_CH1_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val) \
                                                    (((float)TAS6754_CH1_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH2_DC_LDG_DCR_LSB (0xDB)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH2_DC_LDG_DCR_LSB_MASK             0xFF            /* Bits 7-0: CH2 DC RESISTANCE LSB */

/* Reset value */
#define TAS6754_CH2_DC_LDG_DCR_LSB_RESET            0x00            /* Reset value for the register */

/* Helper macros for resistance calculation */
#define TAS6754_CH2_DC_LDG_DCR_GET_LSB(val)         ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit resistance value
 * msb_val should be the 2-bit value extracted from DC_LDG_DCR_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_CH2_DC_LDG_DCR_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Convert combined 10-bit value to ohms (0.1Ω/code) */
#define TAS6754_CH2_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val) \
                                                    (((float)TAS6754_CH2_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH3_DC_LDG_DCR_LSB (0xDC)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH3_DC_LDG_DCR_LSB_MASK             0xFF            /* Bits 7-0: CH3 DC RESISTANCE LSB */

/* Reset value */
#define TAS6754_CH3_DC_LDG_DCR_LSB_RESET            0x00            /* Reset value for the register */

/* Helper macros for resistance calculation */
#define TAS6754_CH3_DC_LDG_DCR_GET_LSB(val)         ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit resistance value
 * msb_val should be the 2-bit value extracted from DC_LDG_DCR_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_CH3_DC_LDG_DCR_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Convert combined 10-bit value to ohms (0.1Ω/code) */
#define TAS6754_CH3_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val) \
                                                    (((float)TAS6754_CH3_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH4_DC_LDG_DCR_LSB (0xDD)
************************************************************************/
/* Bit field masks */
#define TAS6754_CH4_DC_LDG_DCR_LSB_MASK             0xFF            /* Bits 7-0: CH4 DC RESISTANCE LSB */

/* Reset value */
#define TAS6754_CH4_DC_LDG_DCR_LSB_RESET            0x00            /* Reset value for the register */

/* Helper macros for resistance calculation */
#define TAS6754_CH4_DC_LDG_DCR_GET_LSB(val)         ((val) & 0xFF)

/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit resistance value
 * msb_val should be the 2-bit value extracted from DC_LDG_DCR_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_CH4_DC_LDG_DCR_COMBINE(msb_val, lsb_val) \
                                                    (((uint16_t)(msb_val) << 8) | (lsb_val))

/* Convert combined 10-bit value to ohms (0.1Ω/code) */
#define TAS6754_CH4_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val) \
                                                    (((float)TAS6754_CH4_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_OTW_CTRL_CH1_CH2 (0xE2)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTW_CTRL_RESERVED_7_MASK            BIT(7)          /* Bit 7: Reserved */
#define TAS6754_OTW_CTRL_CH1_SEL_MASK               GENMASK(6, 4)   /* Bits 6-4: CH1 OTW SEL */
#define TAS6754_OTW_CTRL_RESERVED_3_MASK            BIT(3)          /* Bit 3: Reserved */
#define TAS6754_OTW_CTRL_CH2_SEL_MASK               GENMASK(2, 0)   /* Bits 2-0: CH2 OTW SEL */

/* CH1 OTW SEL values (bits 6-4) */
#define TAS6754_OTW_CTRL_CH1_DISABLED               (0x00 << 4)  /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH1_95C                    (0x01 << 4)  /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH1_110C                   (0x02 << 4)  /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH1_125C                   (0x03 << 4)  /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH1_135C                   (0x04 << 4)  /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH1_145C                   (0x05 << 4)  /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH1_155C                   (0x06 << 4)  /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH1_165C                   (0x07 << 4)  /* 111: >165°C */

/* CH2 OTW SEL values (bits 2-0) */
#define TAS6754_OTW_CTRL_CH2_DISABLED               (0x00 << 0)  /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH2_95C                    (0x01 << 0)  /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH2_110C                   (0x02 << 0)  /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH2_125C                   (0x03 << 0)  /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH2_135C                   (0x04 << 0)  /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH2_145C                   (0x05 << 0)  /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH2_155C                   (0x06 << 0)  /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH2_165C                   (0x07 << 0)  /* 111: >165°C */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_OTW_CTRL_CH1_CH2_RESERVED_MASK      (TAS6754_OTW_CTRL_RESERVED_7_MASK | \
                                                    TAS6754_OTW_CTRL_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_OTW_CTRL_CH1_CH2_RESET              0x11            /* Reset value for the register */
/************************************************************************
* 					TAS6754_OTW_CTRL_CH3_CH4 (0xE3)
************************************************************************/
/* Bit field masks */
#define TAS6754_OTW_CTRL_RESERVED_7_MASK            BIT(7)          /* Bit 7: Reserved */
#define TAS6754_OTW_CTRL_CH3_SEL_MASK               GENMASK(6, 4)   /* Bits 6-4: CH3 OTW SEL */
#define TAS6754_OTW_CTRL_RESERVED_3_MASK            BIT(3)          /* Bit 3: Reserved */
#define TAS6754_OTW_CTRL_CH4_SEL_MASK               GENMASK(2, 0)   /* Bits 2-0: CH4 OTW SEL */

/* CH3 OTW SEL values (bits 6-4) */
#define TAS6754_OTW_CTRL_CH3_DISABLED               (0x00 << 4)  /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH3_95C                    (0x01 << 4)  /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH3_110C                   (0x02 << 4)  /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH3_125C                   (0x03 << 4)  /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH3_135C                   (0x04 << 4)  /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH3_145C                   (0x05 << 4)  /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH3_155C                   (0x06 << 4)  /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH3_165C                   (0x07 << 4)  /* 111: >165°C */

/* CH4 OTW SEL values (bits 2-0) */
#define TAS6754_OTW_CTRL_CH4_DISABLED               (0x00 << 0)  /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH4_95C                    (0x01 << 0)  /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH4_110C                   (0x02 << 0)  /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH4_125C                   (0x03 << 0)  /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH4_135C                   (0x04 << 0)  /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH4_145C                   (0x05 << 0)  /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH4_155C                   (0x06 << 0)  /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH4_165C                   (0x07 << 0)  /* 111: >165°C */

/* Combined reserved bits mask (for convenience) */
#define TAS6754_OTW_CTRL_CH3_CH4_RESERVED_MASK      (TAS6754_OTW_CTRL_RESERVED_7_MASK | \
                                                    TAS6754_OTW_CTRL_RESERVED_3_MASK)

/* Reset value */
#define TAS6754_OTW_CTRL_CH3_CH4_RESET              0x11            /* Reset value for the register */

#endif /* __TAS6754_H__ */