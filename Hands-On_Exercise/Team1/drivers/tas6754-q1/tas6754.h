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

#define TAS6754_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \ //TODO: double check datasheet
			 			 SNDRV_PCM_FMTBIT_S24_LE)


/* Register Address Map */
#define TAS6754_RESET             	    	                (0x01) /* W     Reset Control */
#define TAS6754_OUTPUT_CTRL              	                (0x02) /* R/W   Output Configuration Control */
#define TAS6754_STATE_CTRL_CH1_CH2       	                (0x03) /* R/W   State Control Channel 1 & Channel 2 */
#define TAS6754_STATE_CTRL_CH3_CH4       	                (0x04) /* R/W   State Control Channel 3 & Channel 4 */
#define TAS6754_ISENSE_CTRL              	                (0x05) /* R/W   Current Sense Control */
#define TAS6754_DC_DETECT_CTRL           	                (0x06) /* R/W   DC Detection Control */
#define TAS6754_SCLK_INV_CTRL            	                (0x20) /* R/W   SCLK Polarity Control */
#define TAS6754_AUDIO_INTERFACE_CTRL     	                (0x21) /* R/W   Audio Interface Control */
#define TAS6754_SDIN_CTRL                	                (0x23) /* R/W   SDIN Control */
#define TAS6754_SDOUT_CTRL               	                (0x25) /* R/W   SDOUT Control */
#define TAS6754_SDIN_OFFSET_MSB          	                (0x27) /* R/W   SDIN Offset MSB */
#define TAS6754_SDIN_AUDIO_OFFSET        	                (0x28) /* R/W   SDIN Audio Path Offset */
#define TAS6754_SDIN_LL_OFFSET           	                (0x29) /* R/W   SDIN Low Latency Path Offset */
#define TAS6754_SDIN_CH_SWAP             	                (0x2A) /* R/W   SDIN Channel Swap Control */
#define TAS6754_SDOUT_OFFSET_MSB         	                (0x2C) /* R/W   SDOUT Offset MSB */
#define TAS6754_VPREDICT_OFFSET          	                (0x2D) /* R/W   Vpredict SDOUT Offset */
#define TAS6754_ISENSE_OFFSET            	                (0x2E) /* R/W   Current Sense SDOUT Offset */
#define TAS6754_SDOUT_EN                 	                (0x31) /* R/W   SDOUT Enable */
#define TAS6754_LL_EN                    	                (0x32) /* R/W   Low Latency Path Enable */
#define TAS6754_RTLDG_EN                 	                (0x37) /* R/W   Real-time Load Diagnostic Open Load/Shorted Load Enable */
#define TAS6754_DC_BLOCK_BYP             	                (0x39) /* R/W   DC Blocking Bypass */
#define TAS6754_DSP_CTRL                 	                (0x3A) /* R/W   DSP Control */
#define TAS6754_PAGE_AUTO_INC            	                (0x3B) /* R/W   Page Auto Increment */
#define TAS6754_DIG_VOL_CH1              	                (0x40) /* R/W   Digital Volume Channel 1 */
#define TAS6754_DIG_VOL_CH2              	                (0x41) /* R/W   Digital Volume Channel 2 */
#define TAS6754_DIG_VOL_CH3              	                (0x42) /* R/W   Digital Volume Channel 3 */
#define TAS6754_DIG_VOL_CH4              	                (0x43) /* R/W   Digital Volume Channel 4 */
#define TAS6754_DIG_VOL_RAMP_CTRL        	                (0x44) /* R/W   Digital Volume Ramp Control */
#define TAS6754_DIG_VOL_COMBINE_CTRL     	                (0x46) /* R/W   Digital Volume Combination Control */
#define TAS6754_AUTO_MUTE_EN             	                (0x47) /* R/W   Auto Mute Enable */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2 	                (0x48) /* R/W   Auto Mute Time Channel 1 & Channel 2 */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4 	                (0x49) /* R/W   Auto Mute Time Channel 3 & Channel 4 */
#define TAS6754_ANALOG_GAIN_CH1_CH2      	                (0x4A) /* R/W   Analog Gain Channel 1 & Channel 2 */
#define TAS6754_ANALOG_GAIN_CH3_CH4      	                (0x4B) /* R/W   Analog Gain Channel 3 & Channel 4 */
#define TAS6754_ANALOG_GAIN_RAMP_CTRL    	                (0x4E) /* R/W   Analog Gain Ramp Control */
#define TAS6754_PULSE_INJECTION_EN       	                (0x52) /* R/W   Pulse Injection Enable */
#define TAS6754_CBC_CTRL                 	                (0x54) /* R/W   CBC Control */
#define TAS6754_CURRENT_LIMIT_CTRL       	                (0x55) /* R/W   Current Limit Control */
#define TAS6754_ISENSE_CAL               	                (0x5B) /* R/W   Current Sense Calibration */
#define TAS6754_PWM_PHASE_CTRL           	                (0x60) /* R/W   PWM Phase Control */
#define TAS6754_SS_CTRL                  	                (0x61) /* R/W   Spread Spectrum Control */
#define TAS6754_SS_RANGE_CTRL            	                (0x62) /* R/W   Spread Spectrum Range Control */
#define TAS6754_SS_DWELL_CTRL            	                (0x66) /* R/W   Spread Spectrum DWELL Control */
#define TAS6754_RAMP_PHASE_CTRL_GPO      	                (0x68) /* R/W   Switching Clock Phase Control for GPO */
#define TAS6754_PWM_PHASE_M_CTRL_CH1     	                (0x69) /* R/W   PWM Phase Manual Control Channel 1 */
#define TAS6754_PWM_PHASE_M_CTRL_CH2     	                (0x6A) /* R/W   PWM Phase Manual Control Channel 2 */
#define TAS6754_PWM_PHASE_M_CTRL_CH3     	                (0x6B) /* R/W   PWM Phase Manual Control Channel 3 */
#define TAS6754_PWM_PHASE_M_CTRL_CH4     	                (0x6C) /* R/W   PWM Phase Manual Control Channel 4 */
#define TAS6754_AUTO_MUTE_STATUS         	                (0x71) /* R/W   Auto Mute Status */
#define TAS6754_STATE_REPORT_CH1_CH2     	                (0x72) /* R     Status Channel 1 & Channel 2 */
#define TAS6754_STATE_REPORT_CH3_CH4     	                (0x73) /* R     Status Channel 3 & Channel 4 */
#define TAS6754_PVDD_SENSE               	                (0x74) /* R     PVDD Voltage Sense */
#define TAS6754_TEMP_GLOBAL              	                (0x75) /* R     Global Temperature Readout */
#define TAS6754_FS_MON                   	                (0x76) /* R     FS Monitor */
#define TAS6754_SCLK_MON                 	                (0x77) /* R     SCLK Monitor */
#define TAS6754_REPORT_ROUTING_1         	                (0x7C) /* R/W   Enable Faults and Warnings to GPIO */
#define TAS6754_POWER_FAULT_STATUS_1     	                (0x7D) /* R     Power Fault Status 1 */
#define TAS6754_POWER_FAULT_STATUS_2     	                (0x80) /* R     Power Fault Status 2 */
#define TAS6754_OT_FAULT                 	                (0x81) /* R     Temperature (OTSD) and Fault Status */
#define TAS6754_OTW_STATUS               	                (0x82) /* R     Temperature (OTW) Warning Status */
#define TAS6754_CLIP_WARN_STATUS         	                (0x83) /* R     Channel Clip Detect Status */
#define TAS6754_CBC_WARNING_STATUS       	                (0x85) /* R     CBC Warning Report */
#define TAS6754_POWER_FAULT_LATCHED      	                (0x86) /* R     Power Fault Latched */
#define TAS6754_OTSD_LATCHED             	                (0x87) /* R     Temperature (OTSD) Fault Latched */
#define TAS6754_OTW_LATCHED              	                (0x88) /* R     Temperature (OTW) Warning Latched */
#define TAS6754_CLIP_WARN_LATCHED        	                (0x89) /* R     Channel Clip Detect Warning Latched */
#define TAS6754_CLK_FAULT_LATCHED        	                (0x8A) /* R     Clock Error Latched */
#define TAS6754_RTLDG_OL_SL_FAULT_LATCHED	                (0x8B) /* R     Real-Time Load Diagnostic OL/SL Latched */
#define TAS6754_CBC_FAULT_WARN_LATCHED   	                (0x8D) /* R     Channel Load Current Fault Latched */
#define TAS6754_OC_DC_FAULT_LATCHED      	                (0x8E) /* R     Channel Over Current and DC Detection Fault Latched */
#define TAS6754_OTSD_RECOVERY_EN         	                (0x8F) /* R/W   Overtemperature Shutdown Auto-recovery Enable */
#define TAS6754_REPORT_ROUTING_2         	                (0x90) /* R/W   Enable Faults to GPIO */
#define TAS6754_REPORT_ROUTING_3         	                (0x91) /* R/W   Enable Warnings to GPIO */
#define TAS6754_REPORT_ROUTING_4         	                (0x92) /* R/W   Enable Faults and Warnings to GPIO */
#define TAS6754_CLIP_DETECT_CTRL         	                (0x93) /* R/W   Clip Detect Control */
#define TAS6754_REPORT_ROUTING_5         	                (0x94) /* R/W   Enable Faults and Warnings Reported to GPIO */
#define TAS6754_GPIO1_OUTPUT_SELECT      	                (0x95) /* R/W   Select Signals to GPIOs */
#define TAS6754_GPIO2_OUTPUT_SELECT      	                (0x96) /* R/W   Select Signals to GPIOs */
#define TAS6754_GPIO_INPUT_SLEEP_HIZ     	                (0x9B) /* R/W   Select Signals from GPIOs */
#define TAS6754_GPIO_INPUT_PLAY_SLEEP    	                (0x9C) /* R/W   Select Signals from GPIOs */
#define TAS6754_GPIO_INPUT_MUTE          	                (0x9D) /* R/W   Select Signals from GPIOs */
#define TAS6754_GPIO_INPUT_SYNC          	                (0x9E) /* R/W   Select Signals from GPIOs */
#define TAS6754_GPIO_INPUT_SDIN2         	                (0x9F) /* R/W   Select Signals from GPIOs */
#define TAS6754_GPIO_CTRL                	                (0xA0) /* R/W   General GPIO Control */
#define TAS6754_GPIO_INVERT              	                (0xA1) /* R/W   Invert GPIO Signals */
#define TAS6754_DC_LDG_CTRL              	                (0xB0) /* R/W   DC Load Diagnostics Control */
#define TAS6754_DC_LDG_LO_CTRL           	                (0xB1) /* R/W   DC Load Diagnostic Line-out Control */
#define TAS6754_DC_LDG_TIME_CTRL         	                (0xB2) /* R/W   DC Load Diagnostic Timing Control */
#define TAS6754_DC_LDG_SL_CH1_CH2_CTRL   	                (0xB3) /* R/W   DC Load Diagnostic Shorted-load Threshold Channel 1 & Channel 2 */
#define TAS6754_DC_LDG_SL_CH3_CH4_CTRL   	                (0xB4) /* R/W   DC Load Diagnostic Shorted-load Threshold Channel 3 & Channel 4 */
#define TAS6754_AC_LDG_CTRL              	                (0xB5) /* R/W   AC Load Diagnostic Control */
#define TAS6754_TWEETER_DETECT_CTRL      	                (0xB6) /* R/W   Tweeter Detection Control */
#define TAS6754_TWEETER_DETECT_THRESH    	                (0xB7) /* R/W   Tweeter Detection Threshold */
#define TAS6754_AC_LDG_FREQ_CTRL         	                (0xB8) /* R/W   AC Load Diagnostic Frequency Control */
#define TAS6754_TEMP_CH1_CH2             	                (0xBB) /* R     Temperature Range Channel 1 & Channel 2 */
#define TAS6754_TEMP_CH3_CH4             	                (0xBC) /* R     Temperature Range Channel 3 & Channel 4 */
#define TAS6754_WARN_OT_MAX_FLAG         	                (0xBD) /* R     OT Warning MAX Level Report */
#define TAS6754_DC_LDG_REPORT_CH1_CH2    	                (0xC0) /* R     DC Load Diagnostic Report Channel 1 & Channel 2 */
#define TAS6754_DC_LDG_REPORT_CH3_CH4    	                (0xC1) /* R     DC Load Diagnostic Report Channel 3 & Channel 4 */
#define TAS6754_DC_LDG_RESULT            	                (0xC2) /* R     DC Load Diagnostic Result Report */
#define TAS6754_AC_LDG_REPORT_CH1_R      	                (0xC3) /* R     AC Load Diagnostic Report Real Channel 1 */
#define TAS6754_AC_LDG_REPORT_CH1_I      	                (0xC4) /* R     AC Load Diagnostic Report Imaginary Channel 1 */
#define TAS6754_AC_LDG_REPORT_CH2_R      	                (0xC5) /* R     AC Load Diagnostic Report Real Channel 2 */
#define TAS6754_AC_LDG_REPORT_CH2_I      	                (0xC6) /* R     AC Load Diagnostic Report Imaginary Channel 2 */
#define TAS6754_AC_LDG_REPORT_CH3_R      	                (0xC7) /* R     AC Load Diagnostic Report Real Channel 3 */
#define TAS6754_AC_LDG_REPORT_CH3_I      	                (0xC8) /* R     AC Load Diagnostic Report Imaginary Channel 3 */
#define TAS6754_AC_LDG_REPORT_CH4_R      	                (0xC9) /* R     AC Load Diagnostic Report Real Channel 4 */
#define TAS6754_AC_LDG_REPORT_CH4_I      	                (0xCA) /* R     AC Load Diagnostic Report Imaginary Channel 4 */
#define TAS6754_TWEETER_REPORT           	                (0xCB) /* R     Tweeter Detection Report */
#define TAS6754_CH1_RTLDG_IMP_MSB        	                (0xD1) /* R     Real-time Load Diagnostic Channel 1 Impedance MSB */
#define TAS6754_CH1_RTLDG_IMP_LSB        	                (0xD2) /* R     Real-time Load Diagnostic Channel 1 Impedance LSB */
#define TAS6754_CH2_RTLDG_IMP_MSB        	                (0xD3) /* R     Real-time Load Diagnostic Channel 2 Impedance MSB */
#define TAS6754_CH2_RTLDG_IMP_LSB        	                (0xD4) /* R     Real-time Load Diagnostic Channel 2 Impedance LSB */
#define TAS6754_CH3_RTLDG_IMP_MSB        	                (0xD5) /* R     Real-time Load Diagnostic Channel 3 Impedance MSB */
#define TAS6754_CH3_RTLDG_IMP_LSB        	                (0xD6) /* R     Real-time Load Diagnostic Channel 3 Impedance LSB */
#define TAS6754_CH4_RTLDG_IMP_MSB        	                (0xD7) /* R     Real-time Load Diagnostic Channel 4 Impedance MSB */
#define TAS6754_CH4_RTLDG_IMP_LSB        	                (0xD8) /* R     Real-time Load Diagnostic Channel 4 Impedance LSB */
#define TAS6754_DC_LDG_DCR_MSB           	                (0xD9) /* R     DC Diagnostic DC Resistance Measurement MSB */
#define TAS6754_CH1_DC_LDG_DCR_LSB       	                (0xDA) /* R     DC Diagnostic Channel 1 DC Resistance Measurement LSB */
#define TAS6754_CH2_DC_LDG_DCR_LSB       	                (0xDB) /* R     DC Diagnostic Channel 2 DC Resistance Measurement LSB */
#define TAS6754_CH3_DC_LDG_DCR_LSB       	                (0xDC) /* R     DC Diagnostic Channel 3 DC Resistance Measurement LSB */
#define TAS6754_CH4_DC_LDG_DCR_LSB       	                (0xDD) /* R     DC Diagnostic Channel 4 DC Resistance Measurement LSB */
#define TAS6754_OTW_CTRL_CH1_CH2         	                (0xE2) /* R/W   Overtemperature Warning (OTW) Level Report Control Channel 1 & Channel 2 */
#define TAS6754_OTW_CTRL_CH3_CH4         	                (0xE3) /* R/W   Overtemperature Warning (OTW) Level Report Control Channel 3 & Channel 4 */
#define TAS6754_MAX							                (TAS6754_OTW_CTRL_CH3_CH4)

/************************************************************************
 *					TAS6754_RESET (0x01)
 ************************************************************************/
#define TAS6754_RESET_RESERVED_7_5_MASK                     GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_RESET_DEVICE_RESET_MASK                     BIT(4)              /* Bit 4: DEVICE RESET */
#define TAS6754_RESET_CLEAR_FAULT_MASK                      BIT(3)              /* Bit 3: CLEAR FAULT */
#define TAS6754_RESET_RESERVED_2_MASK                       BIT(2)              /* Bit 2: Reserved */
#define TAS6754_RESET_RESERVED_1_MASK                       BIT(1)              /* Bit 1: Reserved */
#define TAS6754_RESET_REGISTER_RESET_MASK                   BIT(0)              /* Bit 0: REGISTER RESET */
#define TAS6754_RESET_DEVICE_NORMAL                         (0x00 << 4)         /* 0: Normal operation */
#define TAS6754_RESET_DEVICE_RESET                          (0x01 << 4)         /* 1: Device will be reset */
#define TAS6754_RESET_CLEAR_FAULT_NORMAL                    (0x00 << 3)         /* 0: Normal operation */
#define TAS6754_RESET_CLEAR_FAULT_CLEAR                     (0x01 << 3)         /* 1: Clear analog fault */
#define TAS6754_RESET_REGISTER_NORMAL                       (0x00 << 0)         /* 0: Normal operation */
#define TAS6754_RESET_REGISTER_RESET                        (0x01 << 0)         /* 1: Reset registers */
#define TAS6754_RESET_RESERVED_MASK                         (TAS6754_RESET_RESERVED_7_5_MASK | \
                                                            TAS6754_RESET_RESERVED_2_MASK | \
                                                            TAS6754_RESET_RESERVED_1_MASK)
/************************************************************************
 *					TAS6754_OUTPUT_CTRL (0x02)
 ************************************************************************/
#define TAS6754_OUTPUT_CTRL_CH1_LO_MODE_MASK                BIT(7)              /* Bit 7: CH1 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH2_LO_MODE_MASK                BIT(6)              /* Bit 6: CH2 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH3_LO_MODE_MASK                BIT(5)              /* Bit 5: CH3 LO MODE */
#define TAS6754_OUTPUT_CTRL_CH4_LO_MODE_MASK                BIT(4)              /* Bit 4: CH4 LO MODE */
#define TAS6754_OUTPUT_CTRL_RESERVED_3_MASK                 BIT(3)              /* Bit 3: Reserved */
#define TAS6754_OUTPUT_CTRL_RESERVED_2_MASK                 BIT(2)              /* Bit 2: Reserved */
#define TAS6754_OUTPUT_CTRL_RESERVED_1_0_MASK               GENMASK(1, 0)       /* Bits 1-0: Reserved */
#define TAS6754_OUTPUT_CTRL_CH1_SPEAKER_MODE                (0x00 << 7)         /* 0: Channel 1 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH1_LINE_OUT_MODE               (0x01 << 7)         /* 1: Channel 1 is in line output mode */
#define TAS6754_OUTPUT_CTRL_CH2_SPEAKER_MODE                (0x00 << 6)         /* 0: Channel 2 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH2_LINE_OUT_MODE               (0x01 << 6)         /* 1: Channel 2 is in line output mode */
#define TAS6754_OUTPUT_CTRL_CH3_SPEAKER_MODE                (0x00 << 5)         /* 0: Channel 3 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH3_LINE_OUT_MODE               (0x01 << 5)         /* 1: Channel 3 is in line output mode */
#define TAS6754_OUTPUT_CTRL_CH4_SPEAKER_MODE                (0x00 << 4)         /* 0: Channel 4 is in normal / speaker mode */
#define TAS6754_OUTPUT_CTRL_CH4_LINE_OUT_MODE               (0x01 << 4)         /* 1: Channel 4 is in line output mode */
#define TAS6754_OUTPUT_CTRL_RESERVED_MASK                   (TAS6754_OUTPUT_CTRL_RESERVED_3_MASK | \
                                                            TAS6754_OUTPUT_CTRL_RESERVED_2_MASK | \
                                                            TAS6754_OUTPUT_CTRL_RESERVED_1_0_MASK)
#define TAS6754_OUTPUT_CTRL_ALL_CHANNELS_MASK               (TAS6754_OUTPUT_CTRL_CH1_LO_MODE_MASK | \
                                                            TAS6754_OUTPUT_CTRL_CH2_LO_MODE_MASK | \
                                                            TAS6754_OUTPUT_CTRL_CH3_LO_MODE_MASK | \
                                                            TAS6754_OUTPUT_CTRL_CH4_LO_MODE_MASK)
/************************************************************************
 *				TAS6754_STATE_CTRL_CH1_CH2 (0x03)
 ************************************************************************/
#define TAS6754_STATE_CTRL_CH1_MUTE_MASK                    BIT(7)              /* Bit 7: CH1 MUTE */
#define TAS6754_STATE_CTRL_CH1_STATE_MASK                   GENMASK(6, 4)       /* Bits 6-4: CH1 STATE CTRL */
#define TAS6754_STATE_CTRL_CH2_MUTE_MASK                    BIT(3)              /* Bit 3: CH2 MUTE */
#define TAS6754_STATE_CTRL_CH2_STATE_MASK                   GENMASK(2, 0)       /* Bits 2-0: CH2 STATE CTRL */
#define TAS6754_STATE_CTRL_CH1_NORMAL_VOLUME                (0x00 << 7)         /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH1_MUTE                         (0x01 << 7)         /* 1: Mute */
#define TAS6754_STATE_CTRL_CH1_DEEP_SLEEP                   (0x00 << 4)         /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH1_LOAD_DIAG                    (0x01 << 4)         /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH1_SLEEP                        (0x02 << 4)         /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH1_HIZ                          (0x03 << 4)         /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH1_PLAY                         (0x04 << 4)         /* 100: PLAY */
#define TAS6754_STATE_CTRL_CH2_NORMAL_VOLUME                (0x00 << 3)         /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH2_MUTE                         (0x01 << 3)         /* 1: Mute */
#define TAS6754_STATE_CTRL_CH2_DEEP_SLEEP                   (0x00 << 0)         /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH2_LOAD_DIAG                    (0x01 << 0)         /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH2_SLEEP                        (0x02 << 0)         /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH2_HIZ                          (0x03 << 0)         /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH2_PLAY                         (0x04 << 0)         /* 100: PLAY */
#define TAS6754_STATE_CTRL_CH1_CH2_STATE_DEEP_SLEEP	        (TAS6754_STATE_CTRL_CH1_DEEP_SLEEP | \
					                                        TAS6754_STATE_CTRL_CH2_DEEP_SLEEP)
#define TAS6754_STATE_CTRL_CH1_CH2_STATE_HIZ		        (TAS6754_STATE_CTRL_CH1_HIZ | \
					                                        TAS6754_STATE_CTRL_CH2_HIZ)
/************************************************************************
 *					TAS6754_STATE_CTRL_CH3_CH4 (0x04)
 ************************************************************************/
#define TAS6754_STATE_CTRL_CH3_MUTE_MASK                    BIT(7)              /* Bit 7: CH3 MUTE */
#define TAS6754_STATE_CTRL_CH3_STATE_MASK                   GENMASK(6, 4)       /* Bits 6-4: CH3 STATE CTRL */
#define TAS6754_STATE_CTRL_CH4_MUTE_MASK                    BIT(3)              /* Bit 3: CH4 MUTE */
#define TAS6754_STATE_CTRL_CH4_STATE_MASK                   GENMASK(2, 0)       /* Bits 2-0: CH4 STATE CTRL */
#define TAS6754_STATE_CTRL_CH3_NORMAL_VOLUME                (0x00 << 7)         /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH3_MUTE                         (0x01 << 7)         /* 1: Mute */
#define TAS6754_STATE_CTRL_CH3_DEEP_SLEEP                   (0x00 << 4)         /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH3_LOAD_DIAG                    (0x01 << 4)         /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH3_SLEEP                        (0x02 << 4)         /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH3_HIZ                          (0x03 << 4)         /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH3_PLAY                         (0x04 << 4)         /* 100: PLAY */
#define TAS6754_STATE_CTRL_CH4_NORMAL_VOLUME                (0x00 << 3)         /* 0: Normal volume */
#define TAS6754_STATE_CTRL_CH4_MUTE                         (0x01 << 3)         /* 1: Mute */
#define TAS6754_STATE_CTRL_CH4_DEEP_SLEEP                   (0x00 << 0)         /* 000: DEEP SLEEP */
#define TAS6754_STATE_CTRL_CH4_LOAD_DIAG                    (0x01 << 0)         /* 001: LOAD DIAG */
#define TAS6754_STATE_CTRL_CH4_SLEEP                        (0x02 << 0)         /* 010: SLEEP */
#define TAS6754_STATE_CTRL_CH4_HIZ                          (0x03 << 0)         /* 011: HI-Z */
#define TAS6754_STATE_CTRL_CH4_PLAY                         (0x04 << 0)         /* 100: PLAY */
#define TAS6754_STATE_CTRL_CH3_CH4_STATE_DEEP_SLEEP	        (TAS6754_STATE_CTRL_CH3_DEEP_SLEEP | \
					                                        TAS6754_STATE_CTRL_CH4_DEEP_SLEEP)
#define TAS6754_STATE_CTRL_CH3_CH4_STATE_HIZ		        (TAS6754_STATE_CTRL_CH3_HIZ | \
					                                        TAS6754_STATE_CTRL_CH4_HIZ)
/************************************************************************
 *					TAS6754_ISENSE_CTRL (0x05)
 ************************************************************************/
#define TAS6754_ISENSE_CTRL_RESERVED_MASK                   GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_ISENSE_CTRL_CH1_DISABLE_MASK                BIT(3)              /* Bit 3: CH1 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH2_DISABLE_MASK                BIT(2)              /* Bit 2: CH2 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH3_DISABLE_MASK                BIT(1)              /* Bit 1: CH3 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH4_DISABLE_MASK                BIT(0)              /* Bit 0: CH4 ISENSE DISABLE */
#define TAS6754_ISENSE_CTRL_CH1_DISABLE                     (0x00 << 3)         /* 0: Disable Current Sense Channel 1 */
#define TAS6754_ISENSE_CTRL_CH1_ENABLE                      (0x01 << 3)         /* 1: Enable Current Sense Channel 1 */
#define TAS6754_ISENSE_CTRL_CH2_DISABLE                     (0x00 << 2)         /* 0: Disable Current Sense Channel 2 */
#define TAS6754_ISENSE_CTRL_CH2_ENABLE                      (0x01 << 2)         /* 1: Enable Current Sense Channel 2 */
#define TAS6754_ISENSE_CTRL_CH3_DISABLE                     (0x00 << 1)         /* 0: Disable Current Sense Channel 3 */
#define TAS6754_ISENSE_CTRL_CH3_ENABLE                      (0x01 << 1)         /* 1: Enable Current Sense Channel 3 */
#define TAS6754_ISENSE_CTRL_CH4_DISABLE                     (0x00 << 0)         /* 0: Disable Current Sense Channel 4 */
#define TAS6754_ISENSE_CTRL_CH4_ENABLE                      (0x01 << 0)         /* 1: Enable Current Sense Channel 4 */
#define TAS6754_ISENSE_CTRL_ALL_CH_MASK                     (TAS6754_ISENSE_CTRL_CH1_DISABLE_MASK | \
                                                            TAS6754_ISENSE_CTRL_CH2_DISABLE_MASK | \
                                                            TAS6754_ISENSE_CTRL_CH3_DISABLE_MASK | \
                                                            TAS6754_ISENSE_CTRL_CH4_DISABLE_MASK)
/************************************************************************
 *					TAS6754_DC_DETECT_CTRL (0x06)
 ************************************************************************/
#define TAS6754_DC_DETECT_CTRL_RESERVED_MASK                GENMASK(7, 1)       /* Bits 7-1: Reserved */
#define TAS6754_DC_DETECT_CTRL_DC_DETECT_MASK               BIT(0)              /* Bit 0: DC DETECT */
#define TAS6754_DC_DETECT_CTRL_ENABLE                       (0x00 << 0)         /* 0: Enable DC Detection */
#define TAS6754_DC_DETECT_CTRL_DISABLE                      (0x01 << 0)         /* 1: Disable DC Detection */
/************************************************************************
 *					TAS6754_SCLK_INV_CTRL (0x20)
 ************************************************************************/
#define TAS6754_SCLK_INV_CTRL_RESERVED_7_MASK               BIT(7)              /* Bit 7: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_6_MASK               BIT(6)              /* Bit 6: Reserved */
#define TAS6754_SCLK_INV_CTRL_SCLK_INV_TX_MASK              BIT(5)              /* Bit 5: SCLK INV TX */
#define TAS6754_SCLK_INV_CTRL_SCLK_INV_MASK                 BIT(4)              /* Bit 4: SCLK INV */
#define TAS6754_SCLK_INV_CTRL_RESERVED_3_2_MASK             GENMASK(3, 2)       /* Bits 3-2: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_1_MASK               BIT(1)              /* Bit 1: Reserved */
#define TAS6754_SCLK_INV_CTRL_RESERVED_0_MASK               BIT(0)              /* Bit 0: Reserved */
#define TAS6754_SCLK_INV_CTRL_TX_NORMAL                     (0x00 << 5)         /* 0: Normal SCLK mode */
#define TAS6754_SCLK_INV_CTRL_TX_INVERTED                   (0x01 << 5)         /* 1: Inverted SCLK mode */
#define TAS6754_SCLK_INV_CTRL_NORMAL                        (0x00 << 4)         /* 0: Normal SCLK mode */
#define TAS6754_SCLK_INV_CTRL_INVERTED                      (0x01 << 4)         /* 1: Inverted SCLK mode */
#define TAS6754_SCLK_INV_CTRL_RESERVED_MASK                 (TAS6754_SCLK_INV_CTRL_RESERVED_7_MASK | \
                                                            TAS6754_SCLK_INV_CTRL_RESERVED_6_MASK | \
                                                            TAS6754_SCLK_INV_CTRL_RESERVED_3_2_MASK | \
                                                            TAS6754_SCLK_INV_CTRL_RESERVED_1_MASK | \
                                                            TAS6754_SCLK_INV_CTRL_RESERVED_0_MASK)
/************************************************************************
 *					TAS6754_AUDIO_INTERFACE_CTRL (0x21)
 ***********************************************************************/
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_MASK       BIT(7)              /* Bit 7: LAST SAMPLE HOLD */
#define TAS6754_AUDIO_INTERFACE_RESERVED_6_MASK             BIT(6)              /* Bit 6: Reserved */
#define TAS6754_AUDIO_INTERFACE_RESERVED_5_MASK             BIT(5)              /* Bit 5: Reserved */
#define TAS6754_AUDIO_INTERFACE_TDM_MASK                    BIT(4)              /* Bit 4: TDM */
#define TAS6754_AUDIO_INTERFACE_ASI_FORMAT_MASK             GENMASK(3, 2)       /* Bits 3-2: ASI FORMAT */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_WIDTH_MASK         GENMASK(1, 0)       /* Bits 1-0: FS PULSE WIDTH */
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_ENABLE     (0x00 << 7)         /* 0: Enable last sample hold */
#define TAS6754_AUDIO_INTERFACE_LAST_SAMPLE_HOLD_DISABLE    (0x01 << 7)         /* 1: Disable last sample hold */
#define TAS6754_AUDIO_INTERFACE_DSP_MODE                    (0x00 << 4)         /* 0: DSP mode */
#define TAS6754_AUDIO_INTERFACE_TDM_MODE                    (0x01 << 4)         /* 1: TDM mode */
#define TAS6754_AUDIO_INTERFACE_FORMAT_I2S                  (0x00 << 2)         /* 00: I2S */
#define TAS6754_AUDIO_INTERFACE_FORMAT_TDM_DSP              (0x01 << 2)         /* 01: TDM/DSP */
#define TAS6754_AUDIO_INTERFACE_FORMAT_RTJ                  (0x02 << 2)         /* 10: RTJ */
#define TAS6754_AUDIO_INTERFACE_FORMAT_LTJ                  (0x03 << 2)         /* 11: LTJ */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_GTE8               (0x00 << 0)         /* 00: High width of FSYNC in TDM/DSP mode is equal to or greater than 8 cycles of SCLK */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_LT8                (0x01 << 0)         /* 01: High width of FSYNC in TDM/DSP mode is less than 8 cycles of SCLK */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_RESERVED_2         (0x02 << 0)         /* 10: Reserved */
#define TAS6754_AUDIO_INTERFACE_FS_PULSE_RESERVED_3         (0x03 << 0)         /* 11: Reserved */
#define TAS6754_AUDIO_INTERFACE_RESERVED_MASK               (TAS6754_AUDIO_INTERFACE_RESERVED_6_MASK | \
                                                            TAS6754_AUDIO_INTERFACE_RESERVED_5_MASK)
/************************************************************************
 *					TAS6754_SDIN_CTRL (0x23)
 ************************************************************************/
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN_MASK               BIT(7)              /* Bit 7: TDM AUDIO SDIN */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN_MASK                  BIT(6)              /* Bit 6: TDM LL SDIN */
#define TAS6754_SDIN_CTRL_RESERVED_5_MASK                   BIT(5)              /* Bit 5: Reserved */
#define TAS6754_SDIN_CTRL_RESERVED_4_MASK                   BIT(4)              /* Bit 4: Reserved */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_MASK                  GENMASK(3, 2)       /* Bits 3-2: SDIN WL SELECT 1 */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_MASK                  GENMASK(1, 0)       /* Bits 1-0: SDIN WL SELECT 2 */
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN1                   (0x00 << 7)         /* 0: Select data from SDIN1 */
#define TAS6754_SDIN_CTRL_TDM_AUDIO_SDIN2                   (0x01 << 7)         /* 1: Select data from SDIN2 */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN1                      (0x00 << 6)         /* 0: Select data from SDIN1 */
#define TAS6754_SDIN_CTRL_TDM_LL_SDIN2                      (0x01 << 6)         /* 1: Select data from SDIN2 */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_16BIT                 (0x00 << 2)         /* 00: 16 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_20BIT                 (0x01 << 2)         /* 01: 20 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_24BIT                 (0x02 << 2)         /* 10: 24 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_1_32BIT                 (0x03 << 2)         /* 11: 32 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_16BIT                 (0x00 << 0)         /* 00: 16 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_20BIT                 (0x01 << 0)         /* 01: 20 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_24BIT                 (0x02 << 0)         /* 10: 24 bits */
#define TAS6754_SDIN_CTRL_WL_SELECT_2_32BIT                 (0x03 << 0)         /* 11: 32 bits */
#define TAS6754_SDIN_CTRL_RESERVED_MASK                     (TAS6754_SDIN_CTRL_RESERVED_5_MASK | \
                                                            TAS6754_SDIN_CTRL_RESERVED_4_MASK)
/************************************************************************
 *					TAS6754_SDOUT_CTRL (0x25)
 ************************************************************************/
#define TAS6754_SDOUT_CTRL_SDOUT_SELECT_MASK                GENMASK(7, 4)       /* Bits 7-4: SDOUT SELECT */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_MASK                 GENMASK(3, 2)       /* Bits 3-2: SDOUT WL SELECT 1 */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_MASK                 GENMASK(1, 0)       /* Bits 1-0: SDOUT WL SELECT 2 */
#define TAS6754_SDOUT_CTRL_TDM_SDOUT1                       (0x00 << 4)         /* 0000: In TDM mode SDOUT output place on SDOUT1 */
#define TAS6754_SDOUT_CTRL_NONTDM_12_34                     (0x01 << 4)         /* 0001: For non-TDM mode, output Channel 1/2 in SDOUT1 line and Channel 3/4 in SDOUT2 line */
#define TAS6754_SDOUT_CTRL_NONTDM_34_12                     (0x02 << 4)         /* 0010: For non-TDM mode, output Channel 3/4 in SDOUT1 line and Channel 1/2 in SDOUT2 line */
#define TAS6754_SDOUT_CTRL_TDM_SDOUT2                       (0x0F << 4)         /* 1111: In TDM mode SDOUT output place on SDOUT2 */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_16BIT                (0x00 << 2)         /* 00: 16 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_20BIT                (0x01 << 2)         /* 01: 20 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_24BIT                (0x02 << 2)         /* 10: 24 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_1_32BIT                (0x03 << 2)         /* 11: 32 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_16BIT                (0x00 << 0)         /* 00: 16 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_20BIT                (0x01 << 0)         /* 01: 20 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_24BIT                (0x02 << 0)         /* 10: 24 bits */
#define TAS6754_SDOUT_CTRL_WL_SELECT_2_32BIT                (0x03 << 0)         /* 11: 32 bits */
/************************************************************************
 *					 TAS6754_SDIN_OFFSET_MSB (0x27)
 ************************************************************************/
#define TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_MASK             GENMASK(7, 6)       /* Bits 7-6: AUDIO PATH OFFSET MSB */
#define TAS6754_SDIN_OFFSET_LL_PATH_MSB_MASK                GENMASK(5, 4)       /* Bits 5-4: LL PATH OFFSET MSB */
#define TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_MASK             GENMASK(3, 2)       /* Bits 3-2: LL CH SWAP MSB */
#define TAS6754_SDIN_OFFSET_RESERVED_MASK                   GENMASK(1, 0)       /* Bits 1-0: Reserved */
#define TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT            (6)
#define TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT               (4)
#define TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT            (2)
#define TAS6754_SDIN_OFFSET_GET_AUDIO_PATH_MSB(val)         (((val) & TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_MASK) >> TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_GET_LL_PATH_MSB(val)            (((val) & TAS6754_SDIN_OFFSET_LL_PATH_MSB_MASK) >> TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_GET_LL_CH_SWAP_MSB(val)         (((val) & TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_MASK) >> TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_SET_AUDIO_PATH_MSB(val)         (((val) & 0x03) << TAS6754_SDIN_OFFSET_AUDIO_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_SET_LL_PATH_MSB(val)            (((val) & 0x03) << TAS6754_SDIN_OFFSET_LL_PATH_MSB_SHIFT)
#define TAS6754_SDIN_OFFSET_SET_LL_CH_SWAP_MSB(val)         (((val) & 0x03) << TAS6754_SDIN_OFFSET_LL_CH_SWAP_MSB_SHIFT)
/************************************************************************
 *					TAS6754_SDIN_AUDIO_OFFSET (0x28)
 ************************************************************************/
#define TAS6754_SDIN_AUDIO_OFFSET_LSB_MASK                  (0xFF)              /* Bits 7-0: AUDIO PATH OFFSET LSB */
#define TAS6754_SDIN_AUDIO_OFFSET_GET_LSB(val)              ((val) & 0xFF)
/* 
 * Combine MSB (2 bits) and LSB (8 bits) to get 10-bit offset value
 * msb_val should be the 2-bit value extracted from SDIN_OFFSET_MSB register
 * lsb_val should be the 8-bit value from this register
 */
#define TAS6754_SDIN_AUDIO_OFFSET_COMBINE(msb_val, lsb_val) (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_SDIN_AUDIO_OFFSET_SET(offset)               ((offset) & 0x1FF)  /* Limit to 9 valid bits */
#define TAS6754_SDIN_AUDIO_OFFSET_0                         (0x000)             /* No offset */
#define TAS6754_SDIN_AUDIO_OFFSET_1                         (0x001)             /* 1 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_2                         (0x002)             /* 2 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_8                         (0x008)             /* 8 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_16                        (0x010)             /* 16 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_32                        (0x020)             /* 32 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_64                        (0x040)             /* 64 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_128                       (0x080)             /* 128 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_256                       (0x100)             /* 256 SCLK offset */
#define TAS6754_SDIN_AUDIO_OFFSET_511                       (0x1FF)             /* Maximum valid offset (511 SCLK) */
/************************************************************************
 *					TAS6754_SDIN_LL_OFFSET (0x29)
 ************************************************************************/
#define TAS6754_SDIN_LL_OFFSET_LSB_MASK                     (0xFF)              /* Bits 7-0: LOW LATENCY PATH OFFSET LSB */
#define TAS6754_SDIN_LL_OFFSET_GET_LSB(val)                 ((val) & 0xFF)
#define TAS6754_SDIN_LL_OFFSET_COMBINE(msb_val, lsb_val)    (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_SDIN_LL_OFFSET_SET(offset)                  ((offset) & 0x1FF)  /* Limit to 9 valid bits */
#define TAS6754_SDIN_LL_OFFSET_0                            (0x000)             /* 0 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_1                            (0x001)             /* 1 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_2                            (0x002)             /* 2 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_8                            (0x008)             /* 8 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_16                           (0x010)             /* 16 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_32                           (0x020)             /* 32 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_64                           (0x040)             /* 64 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_96                           (0x060)             /* 96 SCLK offset (default) */
#define TAS6754_SDIN_LL_OFFSET_128                          (0x080)             /* 128 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_256                          (0x100)             /* 256 SCLK offset */
#define TAS6754_SDIN_LL_OFFSET_511                          (0x1FF)             /* Maximum valid offset (511 SCLK) */
/************************************************************************
 *					TAS6754_SDIN_CH_SWAP (0x2A)
 ************************************************************************/
#define TAS6754_SDIN_CH_SWAP_LL_LSB_MASK                    GENMASK(7, 5)       /* Bits 7-5: LOW LATENCY CHANNEL SWAP LSB */
#define TAS6754_SDIN_CH_SWAP_AUDIO_MASK                     GENMASK(4, 0)       /* Bits 4-0: AUDIO CHANNEL SWAP */
#define TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT                   (5)
#define TAS6754_SDIN_CH_SWAP_GET_LL_LSB(val)                (((val) & TAS6754_SDIN_CH_SWAP_LL_LSB_MASK) >> TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT)
#define TAS6754_SDIN_CH_SWAP_GET_AUDIO(val)                 ((val) & TAS6754_SDIN_CH_SWAP_AUDIO_MASK)
#define TAS6754_SDIN_CH_SWAP_LL_COMBINE(msb_val, lsb_val)   (((uint8_t)(msb_val) << 3) | (lsb_val))
#define TAS6754_SDIN_CH_SWAP_SET_LL_LSB(val)                (((val) & 0x07) << TAS6754_SDIN_CH_SWAP_LL_LSB_SHIFT)
#define TAS6754_SDIN_CH_SWAP_SET_AUDIO(val)                 ((val) & 0x1F)
#define TAS6754_SDIN_CH_SWAP_AUDIO_DEFAULT                  (0x00)              /* Default mapping */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_1_2                 (0x01)              /* Swap channels 1 and 2 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_3_4                 (0x02)              /* Swap channels 3 and 4 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_1_3                 (0x04)              /* Swap channels 1 and 3 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_SWAP_2_4                 (0x08)              /* Swap channels 2 and 4 */
#define TAS6754_SDIN_CH_SWAP_AUDIO_REVERSE                  (0x0F)              /* Reverse channel order */
/************************************************************************
 *					TAS6754_SDOUT_OFFSET_MSB (0x2C)
 ************************************************************************/
#define TAS6754_SDOUT_OFFSET_VPREDICT_MSB_MASK              GENMASK(7, 6)       /* Bits 7-6: VPREDICT OFFSET MSB */
#define TAS6754_SDOUT_OFFSET_ISENSE_MSB_MASK                GENMASK(5, 4)       /* Bits 5-4: ISENSE OFFSET MSB */
#define TAS6754_SDOUT_OFFSET_RESERVED_3_2_MASK              GENMASK(3, 2)       /* Bits 3-2: Reserved */
#define TAS6754_SDOUT_OFFSET_RESERVED_1_0_MASK              GENMASK(1, 0)       /* Bits 1-0: Reserved */
#define TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT             (6)
#define TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT               (4)
#define TAS6754_SDOUT_OFFSET_RESERVED_3_2_SHIFT             (2)
#define TAS6754_SDOUT_OFFSET_RESERVED_1_0_SHIFT             (0)
#define TAS6754_SDOUT_OFFSET_GET_VPREDICT_MSB(val)          (((val) & TAS6754_SDOUT_OFFSET_VPREDICT_MSB_MASK) >> TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_GET_ISENSE_MSB(val)            (((val) & TAS6754_SDOUT_OFFSET_ISENSE_MSB_MASK) >> TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_SET_VPREDICT_MSB(val)          (((val) & 0x03) << TAS6754_SDOUT_OFFSET_VPREDICT_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_SET_ISENSE_MSB(val)            (((val) & 0x03) << TAS6754_SDOUT_OFFSET_ISENSE_MSB_SHIFT)
#define TAS6754_SDOUT_OFFSET_RESERVED_MASK                  (TAS6754_SDOUT_OFFSET_RESERVED_3_2_MASK | \
                                                            TAS6754_SDOUT_OFFSET_RESERVED_1_0_MASK)
/************************************************************************
 *					TAS6754_VPREDICT_OFFSET (0x2D)
 ************************************************************************/
#define TAS6754_VPREDICT_OFFSET_LSB_MASK                    GENMASK(7, 0)       /* Bits 7-0: VPREDICT OFFSET LSB */
#define TAS6754_VPREDICT_OFFSET_LSB_SHIFT                   (0)
#define TAS6754_VPREDICT_OFFSET_GET_LSB(val)                ((val) & TAS6754_VPREDICT_OFFSET_LSB_MASK)
#define TAS6754_VPREDICT_OFFSET_SET_LSB(val)                ((val) & 0xFF)
#define TAS6754_VPREDICT_OFFSET_GET_FULL(msb_reg, lsb_reg)  ((TAS6754_SDOUT_OFFSET_GET_VPREDICT_MSB(msb_reg) << 8) | TAS6754_VPREDICT_OFFSET_GET_LSB(lsb_reg))
#define TAS6754_VPREDICT_OFFSET_MAX_VALID                   (511)               /* Maximum valid offset within TDM frame */
#define TAS6754_VPREDICT_OFFSET_OUTSIDE_TDM                 (1023)              /* Offset value outside TDM frame (all bits set)
/************************************************************************
 *					TAS6754_ISENSE_OFFSET (0x2E)
 ************************************************************************/
#define TAS6754_ISENSE_OFFSET_LSB_MASK                      GENMASK(7, 0)       /* Bits 7-0: ISENSE OFFSET LSB */
#define TAS6754_ISENSE_OFFSET_LSB_SHIFT                     (0)
#define TAS6754_ISENSE_OFFSET_GET_LSB(val)                  ((val) & TAS6754_ISENSE_OFFSET_LSB_MASK)
#define TAS6754_ISENSE_OFFSET_SET_LSB(val)                  ((val) & 0xFF)
#define TAS6754_ISENSE_OFFSET_GET_FULL(msb_reg, lsb_reg)    ((TAS6754_SDOUT_OFFSET_GET_ISENSE_MSB(msb_reg) << 8) | TAS6754_ISENSE_OFFSET_GET_LSB(lsb_reg))
#define TAS6754_ISENSE_OFFSET_MAX                           (511)               /* Maximum valid offset value */
/************************************************************************
 *					TAS6754_SDOUT_EN (0x31)
 ************************************************************************/
#define TAS6754_SDOUT_EN_RESERVED_MASK                      GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_SDOUT_EN_NON_TDM_SELECT_MASK                GENMASK(5, 4)       /* Bits 5-4: NON-TDM SDOUT SELECT */
#define TAS6754_SDOUT_EN_ENABLE_MASK                        GENMASK(3, 0)       /* Bits 3-0: SDOUT ENABLE */
#define TAS6754_SDOUT_EN_RESERVED_SHIFT                     (6)
#define TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT               (4)
#define TAS6754_SDOUT_EN_ENABLE_SHIFT                       (0)
#define TAS6754_SDOUT_EN_GET_NON_TDM_SELECT(val)            (((val) & TAS6754_SDOUT_EN_NON_TDM_SELECT_MASK) >> TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT)
#define TAS6754_SDOUT_EN_GET_ENABLE(val)                    (((val) & TAS6754_SDOUT_EN_ENABLE_MASK) >> TAS6754_SDOUT_EN_ENABLE_SHIFT)
#define TAS6754_SDOUT_EN_SET_NON_TDM_SELECT(val)            (((val) & 0x03) << TAS6754_SDOUT_EN_NON_TDM_SELECT_SHIFT)
#define TAS6754_SDOUT_EN_SET_ENABLE(val)                    (((val) & 0x0F) << TAS6754_SDOUT_EN_ENABLE_SHIFT)
#define TAS6754_SDOUT_EN_NON_TDM_VPREDICT                   (0x00)              /* Vpredict Channel 1/2/3/4 */
#define TAS6754_SDOUT_EN_NON_TDM_ISENSE                     (0x01)              /* Isense Channel 1/2/3/4 */
#define TAS6754_SDOUT_EN_TDM_DISABLE_ALL                    (0x00)              /* Disable all output data channels */
#define TAS6754_SDOUT_EN_TDM_VPREDICT                       (0x01)              /* Enable Vpredict Channel 1/2/3/4 output */
#define TAS6754_SDOUT_EN_TDM_ISENSE                         (0x02)              /* Enable Isense Channel 1/2/3/4 output */
#define TAS6754_SDOUT_EN_TDM_ALL                            (0x03)              /* Enable both Vpredict and Isense */
#define TAS6754_SDOUT_EN_NON_TDM_DISABLE_ALL                (0x00)              /* Disable all output data channels */
#define TAS6754_SDOUT_EN_NON_TDM_ENABLE_ALL                 (0x03)              /* Enable all output data channels */
/************************************************************************
 *					TAS6754_LL_EN (0x32)
 ************************************************************************/
#define TAS6754_LL_EN_RESERVED_7_MASK                       BIT(7)              /* Bit 7: Reserved */
#define TAS6754_LL_EN_RESERVED_6_3_MASK                     GENMASK(6, 3)       /* Bits 6-3: Reserved */
#define TAS6754_LL_EN_RESERVED_2_MASK                       BIT(2)              /* Bit 2: Reserved */
#define TAS6754_LL_EN_FFLP_ENABLE_MASK                      BIT(1)              /* Bit 1: FFLP ENABLE */
#define TAS6754_LL_EN_LLP_ENABLE_MASK                       BIT(0)              /* Bit 0: LLP ENABLE */
#define TAS6754_LL_EN_RESERVED_7_SHIFT                      (7)
#define TAS6754_LL_EN_RESERVED_6_3_SHIFT                    (3)
#define TAS6754_LL_EN_RESERVED_2_SHIFT                      (2)
#define TAS6754_LL_EN_FFLP_ENABLE_SHIFT                     (1)
#define TAS6754_LL_EN_LLP_ENABLE_SHIFT                      (0)
#define TAS6754_LL_EN_GET_FFLP_ENABLE(val)                  (((val) & TAS6754_LL_EN_FFLP_ENABLE_MASK) >> TAS6754_LL_EN_FFLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_GET_LLP_ENABLE(val)                   (((val) & TAS6754_LL_EN_LLP_ENABLE_MASK) >> TAS6754_LL_EN_LLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_SET_FFLP_ENABLE(val)                  (((val) & 0x01) << TAS6754_LL_EN_FFLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_SET_LLP_ENABLE(val)                   (((val) & 0x01) << TAS6754_LL_EN_LLP_ENABLE_SHIFT)
#define TAS6754_LL_EN_RESERVED_MASK                         (TAS6754_LL_EN_RESERVED_7_MASK | \
                                                            TAS6754_LL_EN_RESERVED_6_3_MASK | \
                                                            TAS6754_LL_EN_RESERVED_2_MASK)
#define TAS6754_LL_EN_FFLP_DISABLED                         (0x00)              /* Full Feature Low Latency path disabled */
#define TAS6754_LL_EN_FFLP_ENABLED                          (0x01)              /* Full Feature Low Latency path enabled */
#define TAS6754_LL_EN_LLP_DISABLED                          (0x00)              /* Low Latency path disabled */
#define TAS6754_LL_EN_LLP_ENABLED                           (0x01)              /* Low Latency path enabled */
#define TAS6754_LL_EN_ALL_DISABLED                          (0x00)              /* Both paths disabled */
#define TAS6754_LL_EN_ONLY_LLP                              (0x01)              /* Only Low Latency path enabled */
#define TAS6754_LL_EN_ONLY_FFLP                             (0x02)              /* Only Full Feature Low Latency path enabled */
#define TAS6754_LL_EN_ALL_ENABLED                           (0x03)              /* Both paths enabled */
/************************************************************************
*					TAS6754_RTLDG_EN (0x37)
************************************************************************/
#define TAS6754_RTLDG_EN_RESERVED_MASK                      GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_RTLDG_EN_CLIP_MASK_MASK                     BIT(4)              /* Bit 4: RTLDG CLIP MASK */
#define TAS6754_RTLDG_EN_CH1_OLSL_MASK                      BIT(3)              /* Bit 3: CH1 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH2_OLSL_MASK                      BIT(2)              /* Bit 2: CH2 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH3_OLSL_MASK                      BIT(1)              /* Bit 1: CH3 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_CH4_OLSL_MASK                      BIT(0)              /* Bit 0: CH4 RTLDG OLSL ENABLE */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_MASK                   GENMASK(3, 0)       /* Bits 3-0: All channel OLSL enables */
#define TAS6754_RTLDG_EN_RESERVED_SHIFT                     (5)
#define TAS6754_RTLDG_EN_CLIP_MASK_SHIFT                    (4)
#define TAS6754_RTLDG_EN_CH1_OLSL_SHIFT                     (3)
#define TAS6754_RTLDG_EN_CH2_OLSL_SHIFT                     (2)
#define TAS6754_RTLDG_EN_CH3_OLSL_SHIFT                     (1)
#define TAS6754_RTLDG_EN_CH4_OLSL_SHIFT                     (0)
#define TAS6754_RTLDG_EN_GET_CLIP_MASK(val)                 (((val) & TAS6754_RTLDG_EN_CLIP_MASK_MASK) >> TAS6754_RTLDG_EN_CLIP_MASK_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH1_OLSL(val)                  (((val) & TAS6754_RTLDG_EN_CH1_OLSL_MASK) >> TAS6754_RTLDG_EN_CH1_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH2_OLSL(val)                  (((val) & TAS6754_RTLDG_EN_CH2_OLSL_MASK) >> TAS6754_RTLDG_EN_CH2_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH3_OLSL(val)                  (((val) & TAS6754_RTLDG_EN_CH3_OLSL_MASK) >> TAS6754_RTLDG_EN_CH3_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_CH4_OLSL(val)                  (((val) & TAS6754_RTLDG_EN_CH4_OLSL_MASK) >> TAS6754_RTLDG_EN_CH4_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_GET_ALL_CH_OLSL(val)               (((val) & TAS6754_RTLDG_EN_ALL_CH_OLSL_MASK) >> 0)
#define TAS6754_RTLDG_EN_SET_CLIP_MASK(val)                 (((val) & 0x01) << TAS6754_RTLDG_EN_CLIP_MASK_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH1_OLSL(val)                  (((val) & 0x01) << TAS6754_RTLDG_EN_CH1_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH2_OLSL(val)                  (((val) & 0x01) << TAS6754_RTLDG_EN_CH2_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH3_OLSL(val)                  (((val) & 0x01) << TAS6754_RTLDG_EN_CH3_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_CH4_OLSL(val)                  (((val) & 0x01) << TAS6754_RTLDG_EN_CH4_OLSL_SHIFT)
#define TAS6754_RTLDG_EN_SET_ALL_CH_OLSL(val)               (((val) & 0x0F) << 0)
#define TAS6754_RTLDG_EN_CLIP_MASK_DISABLED                 (0x00)              /* Real-time load diagnostic report is not masked when clip */
#define TAS6754_RTLDG_EN_CLIP_MASK_ENABLED                  (0x01)              /* Real-time load diagnostic report is masked when clip fault occurs */
#define TAS6754_RTLDG_EN_CH_OLSL_DISABLED                   (0x00)              /* Disable Real-time load diagnostic open load/shorted load */
#define TAS6754_RTLDG_EN_CH_OLSL_ENABLED                    (0x01)              /* Enable Real-time load diagnostic open load/shorted load */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_DISABLED               (0x00)              /* All channels OLSL disabled */
#define TAS6754_RTLDG_EN_ALL_CH_OLSL_ENABLED                (0x0F)              /* All channels OLSL enabled */
/************************************************************************
*					TAS6754_DC_BLOCK_BYP (0x39)
************************************************************************/
#define TAS6754_DC_BLOCK_BYP_RESERVED_MASK                  GENMASK(7, 1)       /* Bits 7-1: Reserved */
#define TAS6754_DC_BLOCK_BYP_BYPASS_MASK                    BIT(0)              /* Bit 0: DC BLOCK BYPASS */
#define TAS6754_DC_BLOCK_BYP_RESERVED_SHIFT                 (1)
#define TAS6754_DC_BLOCK_BYP_BYPASS_SHIFT                   (0)
#define TAS6754_DC_BLOCK_BYP_GET_BYPASS(val)                ((val) & TAS6754_DC_BLOCK_BYP_BYPASS_MASK)
#define TAS6754_DC_BLOCK_BYP_SET_BYPASS(val)                ((val) & 0x01)
#define TAS6754_DC_BLOCK_BYP_ENABLED                        (0x00)              /* Enable DC Blocking */
#define TAS6754_DC_BLOCK_BYP_BYPASSED                       (0x01)              /* Bypass DC Blocking */
/************************************************************************
*					TAS6754_DSP_CTRL (0x3A)
************************************************************************/
#define TAS6754_DSP_CTRL_RESERVED_7_6_MASK                  GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_MASK               BIT(5)              /* Bit 5: AUDIO SDOUT SELECT */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_MASK              BIT(4)              /* Bit 4: PVDD FOLDBACK ENABLE */
#define TAS6754_DSP_CTRL_RESERVED_3_1_MASK                  GENMASK(3, 1)       /* Bits 3-1: Reserved */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_MASK           BIT(0)              /* Bit 0: THERMAL FOLDBACK ENABLE */
#define TAS6754_DSP_CTRL_RESERVED_7_6_SHIFT                 (6)
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT              (5)
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT             (4)
#define TAS6754_DSP_CTRL_RESERVED_3_1_SHIFT                 (1)
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT          (0)
#define TAS6754_DSP_CTRL_GET_AUDIO_SDOUT_SEL(val)           (((val) & TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_MASK) >> TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT)
#define TAS6754_DSP_CTRL_GET_PVDD_FOLDBACK_EN(val)          (((val) & TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_MASK) >> TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_GET_THERMAL_FOLDBACK_EN(val)       (((val) & TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_MASK) >> TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_SET_AUDIO_SDOUT_SEL(val)           (((val) & 0x01) << TAS6754_DSP_CTRL_AUDIO_SDOUT_SEL_SHIFT)
#define TAS6754_DSP_CTRL_SET_PVDD_FOLDBACK_EN(val)          (((val) & 0x01) << TAS6754_DSP_CTRL_PVDD_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_SET_THERMAL_FOLDBACK_EN(val)       (((val) & 0x01) << TAS6754_DSP_CTRL_THERMAL_FOLDBACK_EN_SHIFT)
#define TAS6754_DSP_CTRL_RESERVED_MASK                      (TAS6754_DSP_CTRL_RESERVED_7_6_MASK | \
                                                            TAS6754_DSP_CTRL_RESERVED_3_1_MASK)
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_DISABLED               (0x00)              /* Post-processed audio signal not routed to SDOUT */
#define TAS6754_DSP_CTRL_AUDIO_SDOUT_ENABLED                (0x01)              /* Post-processed audio signal routed to SDOUT. Replaces Vpredict data */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_DISABLED             (0x00)              /* Disable PVDD Foldback */
#define TAS6754_DSP_CTRL_PVDD_FOLDBACK_ENABLED              (0x01)              /* Enable PVDD Foldback */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_DISABLED          (0x00)              /* Disable Thermal Foldback */
#define TAS6754_DSP_CTRL_THERMAL_FOLDBACK_ENABLED           (0x01)              /* Enable Thermal Foldback */
/************************************************************************
*					TAS6754_PAGE_AUTO_INC (0x3B)
************************************************************************/
#define TAS6754_PAGE_AUTO_INC_RESERVED_7_4_MASK             GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_PAGE_AUTO_INC_DISABLE_MASK                  BIT(3)              /* Bit 3: PAGE AUTO INCREMENT DISABLE */
#define TAS6754_PAGE_AUTO_INC_RESERVED_2_MASK               BIT(2)              /* Bit 2: Reserved */
#define TAS6754_PAGE_AUTO_INC_RESERVED_1_0_MASK             GENMASK(1, 0)       /* Bits 1-0: Reserved */
#define TAS6754_PAGE_AUTO_INC_RESERVED_7_4_SHIFT            (4)
#define TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT                 (3)
#define TAS6754_PAGE_AUTO_INC_RESERVED_2_SHIFT              (2)
#define TAS6754_PAGE_AUTO_INC_RESERVED_1_0_SHIFT            (0)
#define TAS6754_PAGE_AUTO_INC_GET_DISABLE(val)              (((val) & TAS6754_PAGE_AUTO_INC_DISABLE_MASK) >> TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT)
#define TAS6754_PAGE_AUTO_INC_SET_DISABLE(val)              (((val) & 0x01) << TAS6754_PAGE_AUTO_INC_DISABLE_SHIFT)
#define TAS6754_PAGE_AUTO_INC_RESERVED_MASK                 (TAS6754_PAGE_AUTO_INC_RESERVED_7_4_MASK | \
                                                            TAS6754_PAGE_AUTO_INC_RESERVED_2_MASK | \
                                                            TAS6754_PAGE_AUTO_INC_RESERVED_1_0_MASK)
#define TAS6754_PAGE_AUTO_INC_ENABLED                       (0x00)              /* Enable Page auto increment */
#define TAS6754_PAGE_AUTO_INC_DISABLED                      (0x01)              /* Disable Page auto increment */
/************************************************************************
*					TAS6754_DIG_VOL_CH1 (0x40)
************************************************************************/
#define TAS6754_DIG_VOL_CH1_MASK                            GENMASK(7, 0)       /* Bits 7-0: CH1 DIGITAL VOLUME */
#define TAS6754_DIG_VOL_CH1_SHIFT                           (0)
#define TAS6754_DIG_VOL_CH1_GET_VOL(val)                    ((val) & TAS6754_DIG_VOL_CH1_MASK)
#define TAS6754_DIG_VOL_CH1_SET_VOL(val)                    ((val) & 0xFF)
#define TAS6754_DIG_VOL_CH1_0DB                             (0x30)              /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH1_MINUS_0_5DB                     (0x31)              /* -0.5 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_1DB                       (0x32)              /* -1.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_6DB                       (0x3C)              /* -6.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_12DB                      (0x48)              /* -12.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_20DB                      (0x58)              /* -20.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_40DB                      (0x80)              /* -40.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_60DB                      (0xA8)              /* -60.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_80DB                      (0xD0)              /* -80.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_100DB                     (0xF8)              /* -100.0 dB */
#define TAS6754_DIG_VOL_CH1_MINUS_103DB                     (0xFE)              /* -103.0 dB */
#define TAS6754_DIG_VOL_CH1_MUTE                            (0xFF)              /* Mute */
#define TAS6754_DIG_VOL_CH1_DB_TO_REG(db)                   ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)
#define TAS6754_DIG_VOL_CH1_REG_TO_DB(reg)                  ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))
/************************************************************************
*					TAS6754_DIG_VOL_CH2 (0x41)
************************************************************************/
#define TAS6754_DIG_VOL_CH2_MASK                            GENMASK(7, 0)       /* Bits 7-0: CH2 DIGITAL VOLUME */
#define TAS6754_DIG_VOL_CH2_SHIFT                           (0)
#define TAS6754_DIG_VOL_CH2_GET_VOL(val)                    ((val) & TAS6754_DIG_VOL_CH2_MASK)
#define TAS6754_DIG_VOL_CH2_SET_VOL(val)                    ((val) & 0xFF)
#define TAS6754_DIG_VOL_CH2_0DB                             (0x30)              /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH2_MINUS_0_5DB                     (0x31)              /* -0.5 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_1DB                       (0x32)              /* -1.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_6DB                       (0x3C)              /* -6.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_12DB                      (0x48)              /* -12.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_20DB                      (0x58)              /* -20.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_40DB                      (0x80)              /* -40.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_60DB                      (0xA8)              /* -60.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_80DB                      (0xD0)              /* -80.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_100DB                     (0xF8)              /* -100.0 dB */
#define TAS6754_DIG_VOL_CH2_MINUS_103DB                     (0xFE)              /* -103.0 dB */
#define TAS6754_DIG_VOL_CH2_MUTE                            (0xFF)              /* Mute */
#define TAS6754_DIG_VOL_CH2_DB_TO_REG(db)                   ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)
#define TAS6754_DIG_VOL_CH2_REG_TO_DB(reg)                  ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))
/************************************************************************
*					TAS6754_DIG_VOL_CH3 (0x42)
************************************************************************/
#define TAS6754_DIG_VOL_CH3_MASK                            GENMASK(7, 0)       /* Bits 7-0: CH3 DIGITAL VOLUME */
#define TAS6754_DIG_VOL_CH3_SHIFT                           (0)
#define TAS6754_DIG_VOL_CH3_GET_VOL(val)                    ((val) & TAS6754_DIG_VOL_CH3_MASK)
#define TAS6754_DIG_VOL_CH3_SET_VOL(val)                    ((val) & 0xFF)
#define TAS6754_DIG_VOL_CH3_0DB                             (0x30)              /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH3_MINUS_0_5DB                     (0x31)              /* -0.5 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_1DB                       (0x32)              /* -1.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_6DB                       (0x3C)              /* -6.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_12DB                      (0x48)              /* -12.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_20DB                      (0x58)              /* -20.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_40DB                      (0x80)              /* -40.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_60DB                      (0xA8)              /* -60.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_80DB                      (0xD0)              /* -80.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_100DB                     (0xF8)              /* -100.0 dB */
#define TAS6754_DIG_VOL_CH3_MINUS_103DB                     (0xFE)              /* -103.0 dB */
#define TAS6754_DIG_VOL_CH3_MUTE                            (0xFF)              /* Mute */
#define TAS6754_DIG_VOL_CH3_DB_TO_REG(db)                   ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)
#define TAS6754_DIG_VOL_CH3_REG_TO_DB(reg)                  ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))
/************************************************************************
*					TAS6754_DIG_VOL_CH4 (0x43)
************************************************************************/
#define TAS6754_DIG_VOL_CH4_MASK                            GENMASK(7, 0)       /* Bits 7-0: CH4 DIGITAL VOLUME */
#define TAS6754_DIG_VOL_CH4_SHIFT                           (0)
#define TAS6754_DIG_VOL_CH4_GET_VOL(val)                    ((val) & TAS6754_DIG_VOL_CH4_MASK)
#define TAS6754_DIG_VOL_CH4_SET_VOL(val)                    ((val) & 0xFF)
#define TAS6754_DIG_VOL_CH4_0DB                             (0x30)              /* 0.0 dB (Reset value) */
#define TAS6754_DIG_VOL_CH4_MINUS_0_5DB                     (0x31)              /* -0.5 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_1DB                       (0x32)              /* -1.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_6DB                       (0x3C)              /* -6.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_12DB                      (0x48)              /* -12.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_20DB                      (0x58)              /* -20.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_40DB                      (0x80)              /* -40.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_60DB                      (0xA8)              /* -60.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_80DB                      (0xD0)              /* -80.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_100DB                     (0xF8)              /* -100.0 dB */
#define TAS6754_DIG_VOL_CH4_MINUS_103DB                     (0xFE)              /* -103.0 dB */
#define TAS6754_DIG_VOL_CH4_MUTE                            (0xFF)              /* Mute */
#define TAS6754_DIG_VOL_CH4_DB_TO_REG(db)                   ((db <= 0) ? (0x30 - ((uint8_t)(-db * 2))) : 0x30)
#define TAS6754_DIG_VOL_CH4_REG_TO_DB(reg)                  ((reg == 0xFF) ? -INFINITY : ((float)(0x30 - (reg)) / 2.0))
/************************************************************************
*                   Common Definitions for Digital Volume Registers
*					TAS6754_DIG_VOL_CH1 (0x40)
*					TAS6754_DIG_VOL_CH2 (0x41)
*					TAS6754_DIG_VOL_CH3 (0x42)
*					TAS6754_DIG_VOL_CH4 (0x43)
************************************************************************/
#define TAS6754_DIG_VOL_CHx_MAX				 			    (0x30)              /* Maximum volume 0.0 dB */
#define TAS6754_DIG_VOL_CHx_MIN				 			    (0xFE)              /* Minimum volume -103.0 dB */
#define TAS6754_DIG_VOL_CHx_MUTE                   		    (0xFF)              /* Mute: no sound */
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
#define TAS6754_DIG_VOL_CHx_DB_TO_VOLUME(dB)			    ((dB <= -103.0) ? TAS6754_DIG_VOL_MIN : \
                                                		    ((dB == -0.0) ? TAS6754_DIG_VOL_MAX : \
                                                		    (TAS6754_DIG_VOL_MAX + (uint8_t)((-dB) * 2))))
/************************************************************************
* 					TAS6754_DIG_VOL_RAMP_CTRL (0x44)
************************************************************************/
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_MASK                 GENMASK(7, 6)       /* Bits 7-6: DIGITAL VOLUME RAMP DOWN FREQUENCY */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_MASK                 GENMASK(5, 4)       /* Bits 5-4: DIGITAL VOLUME RAMP DOWN STEP */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_MASK                   GENMASK(3, 2)       /* Bits 3-2: DIGITAL VOLUME RAMP UP FREQUENCY */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_MASK                   GENMASK(1, 0)       /* Bits 1-0: DIGITAL VOLUME RAMP UP STEP */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT                (6)
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT                (4)
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT                  (2)
#define TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT                  (0)
#define TAS6754_DIG_VOL_RAMP_GET_DOWN_FREQ(val)             (((val) & TAS6754_DIG_VOL_RAMP_DOWN_FREQ_MASK) >> TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_DOWN_STEP(val)             (((val) & TAS6754_DIG_VOL_RAMP_DOWN_STEP_MASK) >> TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_UP_FREQ(val)               (((val) & TAS6754_DIG_VOL_RAMP_UP_FREQ_MASK) >> TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_GET_UP_STEP(val)               (((val) & TAS6754_DIG_VOL_RAMP_UP_STEP_MASK) >> TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_DOWN_FREQ(val)             (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_DOWN_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_DOWN_STEP(val)             (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_DOWN_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_UP_FREQ(val)               (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_UP_FREQ_SHIFT)
#define TAS6754_DIG_VOL_RAMP_SET_UP_STEP(val)               (((val) & 0x03) << TAS6754_DIG_VOL_RAMP_UP_STEP_SHIFT)
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_4FS                  (0x00)              /* Update every 4 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_1FS                  (0x01)              /* Update every 1 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_32FS                 (0x02)              /* Update every 32 FS periods */
#define TAS6754_DIG_VOL_RAMP_DOWN_FREQ_INSTANT              (0x03)              /* Directly set the volume to zero (Instant mute) */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_4DB                  (0x00)              /* Decrement by 4 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_2DB                  (0x01)              /* Decrement by 2 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_1DB                  (0x02)              /* Decrement by 1 dB for each update */
#define TAS6754_DIG_VOL_RAMP_DOWN_STEP_0_5DB                (0x03)              /* Decrement by 0.5 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_4FS                    (0x00)              /* Update every 4 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_16FS                   (0x01)              /* Update every 16 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_32FS                   (0x02)              /* Update every 32 FS periods */
#define TAS6754_DIG_VOL_RAMP_UP_FREQ_INSTANT                (0x03)              /* Directly restore the volume (Instant unmute) */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_4DB                    (0x00)              /* Increment by 4 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_2DB                    (0x01)              /* Increment by 2 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_1DB                    (0x02)              /* Increment by 1 dB for each update */
#define TAS6754_DIG_VOL_RAMP_UP_STEP_0_5DB                  (0x03)              /* Increment by 0.5 dB for each update */
#define TAS6754_DIG_VOL_RAMP_SLOW                           (0x2A)              /* Slow ramping: 32FS/1dB down, 16FS/1dB up */
#define TAS6754_DIG_VOL_RAMP_MEDIUM                         (0x55)              /* Medium ramping: 1FS/2dB down, 16FS/2dB up */
#define TAS6754_DIG_VOL_RAMP_FAST                           (0x00)              /* Fast ramping: 4FS/4dB down, 4FS/4dB up */
#define TAS6754_DIG_VOL_RAMP_INSTANT                        (0xCF)              /* Instant volume changes */
/************************************************************************
* 					TAS6754_DIG_VOL_COMBINE_CTRL (0x46)
************************************************************************/
#define TAS6754_DIG_VOL_COMBINE_RESERVED_MASK               GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_DIG_VOL_COMBINE_CH3_4_MASK                  GENMASK(3, 2)       /* Bits 3-2: DIGITAL VOLUME COMBINE CH3/4 */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_MASK                  GENMASK(1, 0)       /* Bits 1-0: DIGITAL VOLUME COMBINE CH1/2 */
#define TAS6754_DIG_VOL_COMBINE_RESERVED_SHIFT              (4)
#define TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT                 (2)
#define TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT                 (0)
#define TAS6754_DIG_VOL_COMBINE_GET_CH3_4(val)              (((val) & TAS6754_DIG_VOL_COMBINE_CH3_4_MASK) >> TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_GET_CH1_2(val)              (((val) & TAS6754_DIG_VOL_COMBINE_CH1_2_MASK) >> TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_SET_CH3_4(val)              (((val) & 0x03) << TAS6754_DIG_VOL_COMBINE_CH3_4_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_SET_CH1_2(val)              (((val) & 0x03) << TAS6754_DIG_VOL_COMBINE_CH1_2_SHIFT)
#define TAS6754_DIG_VOL_COMBINE_CH3_4_INDEPENDENT           (0x00)              /* The volume for Channel 3 and Channel 4 are independent */
#define TAS6754_DIG_VOL_COMBINE_CH4_FOLLOWS_CH3             (0x01)              /* Channel 4 volume follows Channel 3 setting */
#define TAS6754_DIG_VOL_COMBINE_CH3_FOLLOWS_CH4             (0x02)              /* Channel 3 volume follows Channel 4 setting */
#define TAS6754_DIG_VOL_COMBINE_CH3_4_RESERVED              (0x03)              /* Reserved */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_INDEPENDENT           (0x00)              /* The volume for Channel 1 and Channel 2 are independent */
#define TAS6754_DIG_VOL_COMBINE_CH2_FOLLOWS_CH1             (0x01)              /* Channel 2 volume follows Channel 1 setting */
#define TAS6754_DIG_VOL_COMBINE_CH1_FOLLOWS_CH2             (0x02)              /* Channel 1 volume follows Channel 2 setting */
#define TAS6754_DIG_VOL_COMBINE_CH1_2_RESERVED              (0x03)              /* Reserved */
#define TAS6754_DIG_VOL_COMBINE_ALL_INDEPENDENT             (0x00)              /* All channels have independent volume control */
#define TAS6754_DIG_VOL_COMBINE_STEREO_PAIRS                (0x05)              /* CH2 follows CH1, CH4 follows CH3 (stereo pairs) */
#define TAS6754_DIG_VOL_COMBINE_LEFT_RIGHT                  (0x0A)              /* CH1 follows CH2, CH3 follows CH4 (left/right pairs) */
/************************************************************************
* 					TAS6754_AUTO_MUTE_EN (0x47)
************************************************************************/
#define TAS6754_AUTO_MUTE_EN_RESERVED_MASK                  GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_AUTO_MUTE_EN_COMBINE_MASK                   BIT(4)              /* Bit 4: AUTO MUTE COMBINE */
#define TAS6754_AUTO_MUTE_EN_CH4_MASK                       BIT(3)              /* Bit 3: CH4 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH3_MASK                       BIT(2)              /* Bit 2: CH3 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH2_MASK                       BIT(1)              /* Bit 1: CH2 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_CH1_MASK                       BIT(0)              /* Bit 0: CH1 AUTO MUTE ENABLE */
#define TAS6754_AUTO_MUTE_EN_ALL_CH_MASK                    GENMASK(3, 0)       /* Bits 3-0: All channel auto mute enables */
#define TAS6754_AUTO_MUTE_EN_RESERVED_SHIFT                 (5)
#define TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT                  (4)
#define TAS6754_AUTO_MUTE_EN_CH4_SHIFT                      (3)
#define TAS6754_AUTO_MUTE_EN_CH3_SHIFT                      (2)
#define TAS6754_AUTO_MUTE_EN_CH2_SHIFT                      (1)
#define TAS6754_AUTO_MUTE_EN_CH1_SHIFT                      (0)
#define TAS6754_AUTO_MUTE_EN_GET_COMBINE(val)               (((val) & TAS6754_AUTO_MUTE_EN_COMBINE_MASK) >> TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH4(val)                   (((val) & TAS6754_AUTO_MUTE_EN_CH4_MASK) >> TAS6754_AUTO_MUTE_EN_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH3(val)                   (((val) & TAS6754_AUTO_MUTE_EN_CH3_MASK) >> TAS6754_AUTO_MUTE_EN_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH2(val)                   (((val) & TAS6754_AUTO_MUTE_EN_CH2_MASK) >> TAS6754_AUTO_MUTE_EN_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_CH1(val)                   (((val) & TAS6754_AUTO_MUTE_EN_CH1_MASK) >> TAS6754_AUTO_MUTE_EN_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_EN_GET_ALL_CH(val)                ((val) & TAS6754_AUTO_MUTE_EN_ALL_CH_MASK)
#define TAS6754_AUTO_MUTE_EN_SET_COMBINE(val)               (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_COMBINE_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH4(val)                   (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH3(val)                   (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH2(val)                   (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_CH1(val)                   (((val) & 0x01) << TAS6754_AUTO_MUTE_EN_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_EN_SET_ALL_CH(val)                ((val) & 0x0F)
#define TAS6754_AUTO_MUTE_EN_COMBINE_INDEPENDENT            (0x00)              /* Auto mute channel independently */
#define TAS6754_AUTO_MUTE_EN_COMBINE_ALL_CHANNELS           (0x01)              /* Auto mute channels only when all four channels are about to be auto muted */
#define TAS6754_AUTO_MUTE_EN_CH_DISABLED                    (0x00)              /* Disable Channel auto mute */
#define TAS6754_AUTO_MUTE_EN_CH_ENABLED                     (0x01)              /* Enable Channel auto mute */
#define TAS6754_AUTO_MUTE_EN_ALL_DISABLED                   (0x00)              /* All channels auto mute disabled */
#define TAS6754_AUTO_MUTE_EN_ALL_ENABLED                    (0x0F)              /* All channels auto mute enabled */
#define TAS6754_AUTO_MUTE_EN_ALL_ENABLED_COMBINED           (0x1F)              /* All channels auto mute enabled and combined */
#define TAS6754_AUTO_MUTE_EN_STEREO_PAIRS                   (0x05)              /* Only CH1 and CH3 auto mute enabled (for stereo pairs) */
/************************************************************************
*					TAS6754_AUTO_MUTE_TIMING_CH1_CH2 (0x48)
************************************************************************/
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK            BIT(7)              /* Bit 7: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH1_MASK                   GENMASK(6, 4)       /* Bits 6-4: CH1 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK            BIT(3)              /* Bit 3: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH2_MASK                   GENMASK(2, 0)       /* Bits 2-0: CH2 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_SHIFT           (7)
#define TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT                  (4)
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_SHIFT           (3)
#define TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT                  (0)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH1(val)               (((val) & TAS6754_AUTO_MUTE_TIMING_CH1_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH2(val)               (((val) & TAS6754_AUTO_MUTE_TIMING_CH2_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH1(val)               (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH1_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH2(val)               (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH2_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_MASK              (TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK | \
                                                            TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK)
#define TAS6754_AUTO_MUTE_TIMING_11_5MS                     (0x00)              /* 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_53MS                       (0x01)              /* 53 ms */
#define TAS6754_AUTO_MUTE_TIMING_106_5MS                    (0x02)              /* 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_266_5MS                    (0x03)              /* 266.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_535MS                      (0x04)              /* 0.535 sec */
#define TAS6754_AUTO_MUTE_TIMING_1_065S                     (0x05)              /* 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_2_665S                     (0x06)              /* 2.665 sec */
#define TAS6754_AUTO_MUTE_TIMING_5_33S                      (0x07)              /* 5.33 sec */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_SHORT              (0x00)              /* Both channels: 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_MEDIUM             (0x22)              /* Both channels: 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_LONG               (0x55)              /* Both channels: 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_CH1_CH2_MAX                (0x77)              /* Both channels: 5.33 sec */
/************************************************************************
* 					TAS6754_AUTO_MUTE_TIMING_CH3_CH4 (0x49)
************************************************************************/
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK            BIT(7)              /* Bit 7: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH3_MASK                   GENMASK(6, 4)       /* Bits 6-4: CH3 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK            BIT(3)              /* Bit 3: Reserved */
#define TAS6754_AUTO_MUTE_TIMING_CH4_MASK                   GENMASK(2, 0)       /* Bits 2-0: CH4 AUTO MUTE TIMING */
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_7_SHIFT           (7)
#define TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT                  (4)
#define TAS6754_AUTO_MUTE_TIMING_RESERVED_3_SHIFT           (3)
#define TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT                  (0)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH3(val)               (((val) & TAS6754_AUTO_MUTE_TIMING_CH3_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_GET_CH4(val)               (((val) & TAS6754_AUTO_MUTE_TIMING_CH4_MASK) >> TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH3(val)               (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH3_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_SET_CH4(val)               (((val) & 0x07) << TAS6754_AUTO_MUTE_TIMING_CH4_SHIFT)
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_RESERVED_MASK      (TAS6754_AUTO_MUTE_TIMING_RESERVED_7_MASK | \
                                                            TAS6754_AUTO_MUTE_TIMING_RESERVED_3_MASK)
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_SHORT              (0x00)              /* Both channels: 11.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_MEDIUM             (0x22)              /* Both channels: 106.5 ms */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_LONG               (0x55)              /* Both channels: 1.065 sec */
#define TAS6754_AUTO_MUTE_TIMING_CH3_CH4_MAX                (0x77)              /* Both channels: 5.33 sec */
/************************************************************************
* 					TAS6754_ANALOG_GAIN_CH1_CH2 (0x4A)
************************************************************************/
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED_MASK           GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_MASK               GENMASK(5, 0)       /* Bits 5-0: CH 1/2 ANALOG GAIN */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED_SHIFT          (6)
#define TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT              (0)
#define TAS6754_ANALOG_GAIN_CH1_CH2_GET_GAIN(val)           (((val) & TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_MASK) >> TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT)
#define TAS6754_ANALOG_GAIN_CH1_CH2_SET_GAIN(val)           (((val) & 0x3F) << TAS6754_ANALOG_GAIN_CH1_CH2_GAIN_SHIFT)
#define TAS6754_ANALOG_GAIN_CH1_CH2_0DB                     (0x00)              /* 0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_0_5DB             (0x01)              /* -0.5 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_1DB               (0x02)              /* -1.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_3DB               (0x06)              /* -3.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_6DB               (0x0C)              /* -6.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_9DB               (0x12)              /* -9.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_12DB              (0x18)              /* -12.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_15DB              (0x1E)              /* -15.0 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_MINUS_15_5DB            (0x1F)              /* -15.5 dB */
#define TAS6754_ANALOG_GAIN_CH1_CH2_RESERVED                (0x3F)              /* Reserved value */
#define TAS6754_ANALOG_GAIN_CH1_CH2_DB_TO_REG(db)           ((db <= 0 && db > -15.5) ? (uint8_t)(-db * 2) : \
                                                            (db == -15.5) ? 0x1F : 0x00)
#define TAS6754_ANALOG_GAIN_CH1_CH2_REG_TO_DB(reg)          ((reg <= 0x1F) ? (-((float)(reg)) / 2.0) : 0.0)
/************************************************************************
* 					TAS6754_ANALOG_GAIN_CH3_CH4 (0x4B)
************************************************************************/
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED_MASK           GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_MASK               GENMASK(5, 0)       /* Bits 5-0: CH 3/4 ANALOG GAIN */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED_SHIFT          (6)
#define TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT              (0)
#define TAS6754_ANALOG_GAIN_CH3_CH4_GET_GAIN(val)           (((val) & TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_MASK) >> TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT)
#define TAS6754_ANALOG_GAIN_CH3_CH4_SET_GAIN(val)           (((val) & 0x3F) << TAS6754_ANALOG_GAIN_CH3_CH4_GAIN_SHIFT)
#define TAS6754_ANALOG_GAIN_CH3_CH4_0DB                     (0x00)              /* 0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_0_5DB             (0x01)              /* -0.5 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_1DB               (0x02)              /* -1.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_3DB               (0x06)              /* -3.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_6DB               (0x0C)              /* -6.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_9DB               (0x12)              /* -9.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_12DB              (0x18)              /* -12.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_15DB              (0x1E)              /* -15.0 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_MINUS_15_5DB            (0x1F)              /* -15.5 dB */
#define TAS6754_ANALOG_GAIN_CH3_CH4_RESERVED                (0x3F)              /* Reserved value */
#define TAS6754_ANALOG_GAIN_CH3_CH4_DB_TO_REG(db)           ((db <= 0 && db > -15.5) ? (uint8_t)(-db * 2) : \
                                                            (db == -15.5) ? 0x1F : 0x00)
#define TAS6754_ANALOG_GAIN_CH3_CH4_REG_TO_DB(reg)          ((reg <= 0x1F) ? (-((float)(reg)) / 2.0) : 0.0)
/************************************************************************
* 					TAS6754_ANALOG_GAIN_RAMP_CTRL (0x4E)
************************************************************************/
#define TAS6754_ANALOG_GAIN_RAMP_RESERVED_MASK              GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_MASK                  GENMASK(3, 2)       /* Bits 3-2: ANALOG GAIN RAMP STEP */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_MASK              BIT(1)              /* Bit 1: ANALOG GAIN RAMP DOWN DISABLE */
#define TAS6754_ANALOG_GAIN_RAMP_UP_DIS_MASK                BIT(0)              /* Bit 0: ANALOG GAIN RAMP UP DISABLE */
#define TAS6754_ANALOG_GAIN_RAMP_RESERVED_SHIFT             (4)
#define TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT                 (2)
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT             (1)
#define TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT               (0)
#define TAS6754_ANALOG_GAIN_RAMP_GET_STEP(val)              (((val) & TAS6754_ANALOG_GAIN_RAMP_STEP_MASK) >> TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_GET_DOWN_DIS(val)          (((val) & TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_MASK) >> TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_GET_UP_DIS(val)            (((val) & TAS6754_ANALOG_GAIN_RAMP_UP_DIS_MASK) >> TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_SET_STEP(val)              (((val) & 0x03) << TAS6754_ANALOG_GAIN_RAMP_STEP_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_SET_DOWN_DIS(val)          (((val) & 0x01) << TAS6754_ANALOG_GAIN_RAMP_DOWN_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_SET_UP_DIS(val)            (((val) & 0x01) << TAS6754_ANALOG_GAIN_RAMP_UP_DIS_SHIFT)
#define TAS6754_ANALOG_GAIN_RAMP_STEP_15US                  (0x00)              /* 15us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_60US                  (0x01)              /* 60us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_200US                 (0x02)              /* 200us/step */
#define TAS6754_ANALOG_GAIN_RAMP_STEP_400US                 (0x03)              /* 400us/step */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_ENABLED               (0x00)              /* Enable Analog Gain ramp down */
#define TAS6754_ANALOG_GAIN_RAMP_DOWN_DISABLED              (0x01)              /* Disable Analog Gain ramp down */
#define TAS6754_ANALOG_GAIN_RAMP_UP_ENABLED                 (0x00)              /* Enable Analog Gain ramp up */
#define TAS6754_ANALOG_GAIN_RAMP_UP_DISABLED                (0x01)              /* Disable Analog Gain ramp up */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_ENABLED_FAST           (0x00)              /* Both ramps enabled, 15us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_ENABLED_SLOW           (0x0C)              /* Both ramps enabled, 400us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ALL_DISABLED               (0x03)              /* Both ramps disabled */
#define TAS6754_ANALOG_GAIN_RAMP_ONLY_UP_MEDIUM             (0x05)              /* Only ramp up enabled, 60us/step */
#define TAS6754_ANALOG_GAIN_RAMP_ONLY_DOWN_MEDIUM           (0x09)              /* Only ramp down enabled, 60us/step */
/************************************************************************
* 					TAS6754_PULSE_INJECTION_EN (0x52)
************************************************************************/
#define TAS6754_PULSE_INJECTION_EN_ENABLE_MASK              BIT(7)              /* Bit 7: PULSE INJECTION ENABLE */
#define TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_MASK        GENMASK(6, 3)       /* Bits 6-3: Reserved */
#define TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_MASK        GENMASK(2, 0)       /* Bits 2-0: Reserved */
#define TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT             (7)
#define TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_SHIFT       (3)
#define TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_SHIFT       (0)
#define TAS6754_PULSE_INJECTION_EN_GET_ENABLE(val)          (((val) & TAS6754_PULSE_INJECTION_EN_ENABLE_MASK) >> TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT)
#define TAS6754_PULSE_INJECTION_EN_SET_ENABLE(val)          (((val) & 0x01) << TAS6754_PULSE_INJECTION_EN_ENABLE_SHIFT)
#define TAS6754_PULSE_INJECTION_EN_RESERVED_MASK            (TAS6754_PULSE_INJECTION_EN_RESERVED_6_3_MASK | \
                                                            TAS6754_PULSE_INJECTION_EN_RESERVED_2_0_MASK)
#define TAS6754_PULSE_INJECTION_DISABLED                    (0x00)              /* Disable pulse injection */
#define TAS6754_PULSE_INJECTION_ENABLED                     (0x01)              /* Enable pulse injection */
#define TAS6754_PULSE_INJECTION_EN_WRITE_MASK               (0x87)              /* Mask for writing (preserve reserved bits) */
#define TAS6754_PULSE_INJECTION_EN_WRITE_DISABLED           (0x03)              /* Value to write to disable pulse injection */
#define TAS6754_PULSE_INJECTION_EN_WRITE_ENABLED            (0x83)              /* Value to write to enable pulse injection */
/************************************************************************
* 					TAS6754_CBC_CTRL (0x54)
************************************************************************/
#define TAS6754_CBC_CTRL_RESERVED_7_4_MASK                  GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_CBC_CTRL_RESERVED_3_MASK                    BIT(3)              /* Bit 3: Reserved */
#define TAS6754_CBC_CTRL_RESERVED_2_MASK                    BIT(2)              /* Bit 2: Reserved */
#define TAS6754_CBC_CTRL_FAULT_DISABLE_MASK                 BIT(1)              /* Bit 1: CBC FAULT DISABLE */
#define TAS6754_CBC_CTRL_WARN_DISABLE_MASK                  BIT(0)              /* Bit 0: CBC WARN DISABLE */
#define TAS6754_CBC_CTRL_RESERVED_7_4_SHIFT                 (4)
#define TAS6754_CBC_CTRL_RESERVED_3_SHIFT                   (3)
#define TAS6754_CBC_CTRL_RESERVED_2_SHIFT                   (2)
#define TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT                (1)
#define TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT                 (0)
#define TAS6754_CBC_CTRL_GET_FAULT_DISABLE(val)             (((val) & TAS6754_CBC_CTRL_FAULT_DISABLE_MASK) >> TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_GET_WARN_DISABLE(val)              (((val) & TAS6754_CBC_CTRL_WARN_DISABLE_MASK) >> TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_SET_FAULT_DISABLE(val)             (((val) & 0x01) << TAS6754_CBC_CTRL_FAULT_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_SET_WARN_DISABLE(val)              (((val) & 0x01) << TAS6754_CBC_CTRL_WARN_DISABLE_SHIFT)
#define TAS6754_CBC_CTRL_RESERVED_MASK                      (TAS6754_CBC_CTRL_RESERVED_7_4_MASK | \
                                                            TAS6754_CBC_CTRL_RESERVED_3_MASK | \
                                                            TAS6754_CBC_CTRL_RESERVED_2_MASK)
#define TAS6754_CBC_CTRL_FAULT_DETECTION_DISABLED           (0x00)              /* Disable CBC fault detection */
#define TAS6754_CBC_CTRL_FAULT_DETECTION_ENABLED            (0x01)              /* Enable CBC fault detection */
#define TAS6754_CBC_CTRL_WARN_DETECTION_DISABLED            (0x00)              /* Disable CBC warning detection */
#define TAS6754_CBC_CTRL_WARN_DETECTION_ENABLED             (0x01)              /* Enable CBC warning detection */
#define TAS6754_CBC_CTRL_ALL_DETECTION_DISABLED             (0x00)              /* Both fault and warning detection disabled */
#define TAS6754_CBC_CTRL_ALL_DETECTION_ENABLED              (0x03)              /* Both fault and warning detection enabled */
#define TAS6754_CBC_CTRL_ONLY_FAULT_ENABLED                 (0x02)              /* Only fault detection enabled */
#define TAS6754_CBC_CTRL_ONLY_WARN_ENABLED                  (0x01)              /* Only warning detection enabled */
#define TAS6754_CBC_CTRL_WRITE_MASK                         (0x07)              /* Mask for writing (preserve reserved bits) */
/************************************************************************
* 					TAS6754_CURRENT_LIMIT_CTRL (0x55)
************************************************************************/
#define TAS6754_CURRENT_LIMIT_RESERVED_7_MASK               BIT(7)              /* Bit 7: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_6_5_MASK             GENMASK(6, 5)       /* Bits 6-5: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_4_3_MASK             GENMASK(4, 3)       /* Bits 4-3: Reserved */
#define TAS6754_CURRENT_LIMIT_RESERVED_2_MASK               BIT(2)              /* Bit 2: Reserved */
#define TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_MASK             GENMASK(1, 0)       /* Bits 1-0: CBC/OC LEVEL */
#define TAS6754_CURRENT_LIMIT_RESERVED_7_SHIFT              (7)
#define TAS6754_CURRENT_LIMIT_RESERVED_6_5_SHIFT            (5)
#define TAS6754_CURRENT_LIMIT_RESERVED_4_3_SHIFT            (3)
#define TAS6754_CURRENT_LIMIT_RESERVED_2_SHIFT              (2)
#define TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT            (0)
#define TAS6754_CURRENT_LIMIT_GET_CBC_OC_LEVEL(val)         (((val) & TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_MASK) >> TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT)
#define TAS6754_CURRENT_LIMIT_SET_CBC_OC_LEVEL(val)         (((val) & 0x03) << TAS6754_CURRENT_LIMIT_CBC_OC_LEVEL_SHIFT)
#define TAS6754_CURRENT_LIMIT_RESERVED_MASK                 (TAS6754_CURRENT_LIMIT_RESERVED_7_MASK | \
                                                            TAS6754_CURRENT_LIMIT_RESERVED_6_5_MASK | \
                                                            TAS6754_CURRENT_LIMIT_RESERVED_4_3_MASK | \
                                                            TAS6754_CURRENT_LIMIT_RESERVED_2_MASK)
#define TAS6754_CURRENT_LIMIT_LEVEL_4                       (0x00)              /* Level 4 (lowest current limit) */
#define TAS6754_CURRENT_LIMIT_LEVEL_3                       (0x01)              /* Level 3 */
#define TAS6754_CURRENT_LIMIT_LEVEL_2                       (0x02)              /* Level 2 */
#define TAS6754_CURRENT_LIMIT_LEVEL_1                       (0x03)              /* Level 1 (highest current limit) */
/************************************************************************
* 					TAS6754_ISENSE_CAL (0x5B)
************************************************************************/
#define TAS6754_ISENSE_CAL_RESERVED_7_4_MASK                GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_ISENSE_CAL_CALIBRATION_MASK                 BIT(3)              /* Bit 3: ISENSE CALIBRATION */
#define TAS6754_ISENSE_CAL_RESERVED_2_0_MASK                GENMASK(2, 0)       /* Bits 2-0: Reserved */
#define TAS6754_ISENSE_CAL_RESERVED_7_4_SHIFT               (4)
#define TAS6754_ISENSE_CAL_CALIBRATION_SHIFT                (3)
#define TAS6754_ISENSE_CAL_RESERVED_2_0_SHIFT               (0)
#define TAS6754_ISENSE_CAL_GET_CALIBRATION(val)             (((val) & TAS6754_ISENSE_CAL_CALIBRATION_MASK) >> TAS6754_ISENSE_CAL_CALIBRATION_SHIFT)
#define TAS6754_ISENSE_CAL_SET_CALIBRATION(val)             (((val) & 0x01) << TAS6754_ISENSE_CAL_CALIBRATION_SHIFT)
#define TAS6754_ISENSE_CAL_RESERVED_MASK                    (TAS6754_ISENSE_CAL_RESERVED_7_4_MASK | \
                                                            TAS6754_ISENSE_CAL_RESERVED_2_0_MASK)
#define TAS6754_ISENSE_CAL_DISABLED                         (0x00)              /* Disable Isense Offset calibration logic */
#define TAS6754_ISENSE_CAL_ENABLED                          (0x01)              /* Enable Isense Offset calibration logic */
/************************************************************************
* 					TAS6754_PWM_PHASE_CTRL (0x60)
************************************************************************/
#define TAS6754_PWM_PHASE_MANUAL_MODE_ENABLE_MASK           BIT(7)              /* Bit 7: PWM PHASE MANUAL MODE ENABLE */
#define TAS6754_PWM_PHASE_SELECT_MASK                       GENMASK(6, 4)       /* Bits 6-4: PWM PHASE SELECT */
#define TAS6754_PWM_PHASE_RESERVED_MASK                     GENMASK(3, 2)       /* Bits 3-2: Reserved */
#define TAS6754_PWM_PHASE_SYNC_SELECT_MASK                  BIT(1)              /* Bit 1: PWM PHASE SYNC SELECT */
#define TAS6754_PWM_PHASE_SYNC_ENABLE_MASK                  BIT(0)              /* Bit 0: PWM PHASE SYNC ENABLE */
#define TAS6754_PWM_PHASE_MANUAL_MODE_DISABLE               (0x00 << 7)         /* 0: Disable manual phase mode */
#define TAS6754_PWM_PHASE_MANUAL_MODE_ENABLE                (0x01 << 7)         /* 1: Enable manual phase mode */
#define TAS6754_PWM_PHASE_SELECT_MODE_0                     (0x00 << 4)         /* 000: Ch1:0°, Ch2:180°, Ch3:90°, Ch4:270° */
#define TAS6754_PWM_PHASE_SELECT_MODE_1                     (0x01 << 4)         /* 001: Ch1:0°, Ch2:45°, Ch3:90°, Ch4:135° */
#define TAS6754_PWM_PHASE_SYNC_SELECT_GPIO                  (0x00 << 1)         /* 0: GPIO sync */
#define TAS6754_PWM_PHASE_SYNC_SELECT_SCLK                  (0x01 << 1)         /* 1: SCLK sync */
#define TAS6754_PWM_PHASE_SYNC_DISABLE                      (0x00 << 0)         /* 0: Disable ramp phase sync */
#define TAS6754_PWM_PHASE_SYNC_ENABLE                       (0x01 << 0)         /* 1: Enable ramp phase sync */
/************************************************************************
* 					TAS6754_SS_CTRL (0x61)
************************************************************************/
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV_MASK               GENMASK(7, 6)       /* Bits 7-6: GPO RAMP CLK DIV */
#define TAS6754_SS_CTRL_RESERVED_5_MASK                     BIT(5)              /* Bit 5: Reserved */
#define TAS6754_SS_CTRL_RESERVED_4_MASK                     BIT(4)              /* Bit 4: Reserved */
#define TAS6754_SS_CTRL_RESERVED_3_MASK                     BIT(3)              /* Bit 3: Reserved */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_SS_MASK         BIT(2)              /* Bit 2: RDM PERIOD TRIANGLE SS ENABLE */
#define TAS6754_SS_CTRL_RANDOM_SS_MASK                      BIT(1)              /* Bit 1: RANDOM SS ENABLE */
#define TAS6754_SS_CTRL_TRIANGLE_SS_MASK                    BIT(0)              /* Bit 0: TRIANGLE SS ENABLE */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV1                   (0x00 << 6)         /* 0: div1 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV2                   (0x01 << 6)         /* 1: div2 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV4                   (0x02 << 6)         /* 2: div4 */
#define TAS6754_SS_CTRL_GPO_RAMP_CLK_DIV8                   (0x03 << 6)         /* 3: div8 */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_NORMAL          (0x00 << 2)         /* 0: Normal triangle spread spectrum */
#define TAS6754_SS_CTRL_RDM_PERIOD_TRIANGLE_RANDOM          (0x01 << 2)         /* 1: Triangle spread spectrum with random period */
#define TAS6754_SS_CTRL_RANDOM_SS_DISABLE                   (0x00 << 1)         /* 0: Disable random spread spectrum */
#define TAS6754_SS_CTRL_RANDOM_SS_ENABLE                    (0x01 << 1)         /* 1: Enable random spread spectrum */
#define TAS6754_SS_CTRL_TRIANGLE_SS_DISABLE                 (0x00 << 0)         /* 0: Disable triangle spread spectrum */
#define TAS6754_SS_CTRL_TRIANGLE_SS_ENABLE                  (0x01 << 0)         /* 1: Enable triangle spread spectrum */
#define TAS6754_SS_CTRL_RESERVED_MASK                       (TAS6754_SS_CTRL_RESERVED_5_MASK | \
                                                            TAS6754_SS_CTRL_RESERVED_4_MASK | \
                                                            TAS6754_SS_CTRL_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_SS_RANGE_CTRL (0x62)
************************************************************************/
#define TAS6754_SS_RANGE_RESERVED_7_MASK                    BIT(7)              /* Bit 7: Reserved */
#define TAS6754_SS_RANGE_RANDOM_SS_RANGE_MASK               GENMASK(6, 4)       /* Bits 6-4: RANDOM SS RANGE */
#define TAS6754_SS_RANGE_RDM_DWELL_CTRL_MASK                GENMASK(3, 2)       /* Bits 3-2: SS RDM DWELL CTRL */
#define TAS6754_SS_RANGE_TRIANGLE_SS_RANGE_MASK             GENMASK(1, 0)       /* Bits 1-0: TRIANGLE SS RANGE */
#define TAS6754_SS_RANGE_RANDOM_0_83_PERCENT                (0x00 << 4)         /* 000: ±0.83% */
#define TAS6754_SS_RANGE_RANDOM_2_50_PERCENT                (0x01 << 4)         /* 001: ±2.50% */
#define TAS6754_SS_RANGE_RANDOM_5_83_PERCENT                (0x02 << 4)         /* 010: ±5.83% */
#define TAS6754_SS_RANGE_RANDOM_12_50_PERCENT               (0x03 << 4)         /* 011: ±12.50% */
#define TAS6754_SS_RANGE_RANDOM_25_83_PERCENT               (0x04 << 4)         /* 100: ±25.83% */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_2                   (0x00 << 2)         /* 00: Random triangle SS period from 1/FSS to 2/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_4                   (0x01 << 2)         /* 01: Random triangle SS period from 1/FSS to 4/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_8                   (0x02 << 2)         /* 10: Random triangle SS period from 1/FSS to 8/FSS */
#define TAS6754_SS_RANGE_RDM_DWELL_1_TO_15                  (0x03 << 2)         /* 11: Random triangle SS period from 1/FSS to 15/FSS */
#define TAS6754_SS_RANGE_TRIANGLE_6_5_PERCENT               (0x00 << 0)         /* 00: ±6.5% */
#define TAS6754_SS_RANGE_TRIANGLE_13_5_PERCENT              (0x01 << 0)         /* 01: ±13.5% */
#define TAS6754_SS_RANGE_TRIANGLE_5_PERCENT                 (0x02 << 0)         /* 10: ±5% */
#define TAS6754_SS_RANGE_TRIANGLE_10_PERCENT                (0x03 << 0)         /* 11: ±10% */
/************************************************************************
* 					TAS6754_SS_DWELL_CTRL (0x66)
************************************************************************/
#define TAS6754_SS_DWELL_MIN_MASK                           GENMASK(7, 4)       /* Bits 7-4: DWELL min control */
#define TAS6754_SS_DWELL_MAX_MASK                           GENMASK(3, 0)       /* Bits 3-0: DWELL max control */
#define TAS6754_SS_DWELL_MIN_VAL(val)                       ((val & 0x0F) << 4) /* Set DWELL min value (1-15, 0 unavailable) */
#define TAS6754_SS_DWELL_MAX_VAL(val)                       (val & 0x0F)        /* Set DWELL max value (1-15, 0 unavailable) */
#define TAS6754_SS_DWELL_CTRL_VAL(min, max)                 ((min << 4) | max)  /* Set both min and max values */
/************************************************************************
* 					TAS6754_RAMP_PHASE_CTRL_GPO (0x68)
************************************************************************/
#define TAS6754_RAMP_PHASE_GPO_SYNC_MASK                    (0xFF)              /* Bits 7-0: RAMP PHASE GPO SYNC */
#define TAS6754_RAMP_PHASE_GPO_0_DEG                        (0x00)              /* 0x00: 0 degrees */
#define TAS6754_RAMP_PHASE_GPO_45_DEG                       (0x20)              /* 0x20: 45 degrees */
#define TAS6754_RAMP_PHASE_GPO_90_DEG                       (0x40)              /* 0x40: 90 degrees */
#define TAS6754_RAMP_PHASE_GPO_135_DEG                      (0x60)              /* 0x60: 135 degrees */
#define TAS6754_RAMP_PHASE_GPO_180_DEG                      (0x80)              /* 0x80: 180 degrees */
#define TAS6754_RAMP_PHASE_GPO_225_DEG                      (0xA0)              /* 0xA0: 225 degrees */
#define TAS6754_RAMP_PHASE_GPO_270_DEG                      (0xC0)              /* 0xC0: 270 degrees */
#define TAS6754_RAMP_PHASE_GPO_315_DEG                      (0xE0)              /* 0xE0: 315 degrees */
#define TAS6754_RAMP_PHASE_GPO_DEG(deg)                     ((uint8_t)((deg * 256) / 360))
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH1 (0x69)
************************************************************************/
#define TAS6754_PWM_PHASE_M_CTRL_CH1_MASK                   (0xFF)              /* Bits 7-0: CH1 PWM PHASE MANUAL CTRL */
#define TAS6754_PWM_PHASE_CH1_0_DEG                         (0x00)              /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH1_45_DEG                        (0x20)              /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH1_90_DEG                        (0x40)              /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH1_135_DEG                       (0x60)              /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH1_180_DEG                       (0x80)              /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH1_225_DEG                       (0xA0)              /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH1_270_DEG                       (0xC0)              /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH1_315_DEG                       (0xE0)              /* 0xE0: 315 degrees */
#define TAS6754_PWM_PHASE_CH1_DEG(deg)                      ((uint8_t)((deg * 256) / 360))
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH2 (0x6A)
************************************************************************/
#define TAS6754_PWM_PHASE_M_CTRL_CH2_MASK                   (0xFF)              /* Bits 7-0: CH2 PWM PHASE MANUAL CTRL */
#define TAS6754_PWM_PHASE_CH2_0_DEG                         (0x00)              /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH2_45_DEG                        (0x20)              /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH2_90_DEG                        (0x40)              /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH2_135_DEG                       (0x60)              /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH2_180_DEG                       (0x80)              /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH2_225_DEG                       (0xA0)              /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH2_270_DEG                       (0xC0)              /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH2_315_DEG                       (0xE0)              /* 0xE0: 315 degrees */
#define TAS6754_PWM_PHASE_CH2_DEG(deg)                      ((uint8_t)((deg * 256) / 360))
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH3 (0x6B)
************************************************************************/
#define TAS6754_PWM_PHASE_M_CTRL_CH3_MASK                   (0xFF)              /* Bits 7-0: CH3 PWM PHASE MANUAL CTRL */
#define TAS6754_PWM_PHASE_CH3_0_DEG                         (0x00)              /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH3_45_DEG                        (0x20)              /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH3_90_DEG                        (0x40)              /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH3_135_DEG                       (0x60)              /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH3_180_DEG                       (0x80)              /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH3_225_DEG                       (0xA0)              /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH3_270_DEG                       (0xC0)              /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH3_315_DEG                       (0xE0)              /* 0xE0: 315 degrees */
#define TAS6754_PWM_PHASE_CH3_DEG(deg)                      ((uint8_t)((deg * 256) / 360))
/************************************************************************
* 					TAS6754_PWM_PHASE_M_CTRL_CH4 (0x6C)
************************************************************************/
#define TAS6754_PWM_PHASE_M_CTRL_CH4_MASK                   (0xFF)              /* Bits 7-0: CH4 PWM PHASE MANUAL CTRL */
#define TAS6754_PWM_PHASE_CH4_0_DEG                         (0x00)              /* 0x00: 0 degrees */
#define TAS6754_PWM_PHASE_CH4_45_DEG                        (0x20)              /* 0x20: 45 degrees */
#define TAS6754_PWM_PHASE_CH4_90_DEG                        (0x40)              /* 0x40: 90 degrees */
#define TAS6754_PWM_PHASE_CH4_135_DEG                       (0x60)              /* 0x60: 135 degrees */
#define TAS6754_PWM_PHASE_CH4_180_DEG                       (0x80)              /* 0x80: 180 degrees */
#define TAS6754_PWM_PHASE_CH4_225_DEG                       (0xA0)              /* 0xA0: 225 degrees */
#define TAS6754_PWM_PHASE_CH4_270_DEG                       (0xC0)              /* 0xC0: 270 degrees */
#define TAS6754_PWM_PHASE_CH4_315_DEG                       (0xE0)              /* 0xE0: 315 degrees */
#define TAS6754_PWM_PHASE_CH4_DEG(deg)                      ((uint8_t)((deg * 256) / 360))
/************************************************************************
* 					TAS6754_AUTO_MUTE_STATUS (0x71)
************************************************************************/
#define TAS6754_AUTO_MUTE_STATUS_RESERVED_MASK              GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_AUTO_MUTE_STATUS_CH1_MASK                   BIT(3)              /* Bit 3: CH1 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH2_MASK                   BIT(2)              /* Bit 2: CH2 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH3_MASK                   BIT(1)              /* Bit 1: CH3 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH4_MASK                   BIT(0)              /* Bit 0: CH4 AM STATUS */
#define TAS6754_AUTO_MUTE_STATUS_CH1_NOT_MUTED              (0x00 << 3)         /* 0: Channel 1 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH1_MUTED                  (0x01 << 3)         /* 1: Channel 1 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH2_NOT_MUTED              (0x00 << 2)         /* 0: Channel 2 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH2_MUTED                  (0x01 << 2)         /* 1: Channel 2 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH3_NOT_MUTED              (0x00 << 1)         /* 0: Channel 3 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH3_MUTED                  (0x01 << 1)         /* 1: Channel 3 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH4_NOT_MUTED              (0x00 << 0)         /* 0: Channel 4 not auto muted */
#define TAS6754_AUTO_MUTE_STATUS_CH4_MUTED                  (0x01 << 0)         /* 1: Channel 4 auto muted */
#define TAS6754_AUTO_MUTE_STATUS_ALL_CHANNELS_MASK          GENMASK(3, 0)       /* Mask for all channel status bits */
/************************************************************************
* 					TAS6754_STATE_REPORT_CH1_CH2 (0x72)
************************************************************************/
#define TAS6754_STATE_REPORT_CH1_MASK                       GENMASK(7, 4)       /* Bits 7-4: CH1 STATUS */
#define TAS6754_STATE_REPORT_CH2_MASK                       GENMASK(3, 0)       /* Bits 3-0: CH2 STATUS */
#define TAS6754_STATE_CH1_DEEPSLEEP                         (0x00 << 4)         /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH1_LOAD_DIAG                         (0x01 << 4)         /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH1_SLEEP                             (0x02 << 4)         /* 0010: SLEEP */
#define TAS6754_STATE_CH1_HIZ                               (0x03 << 4)         /* 0011: HI-Z */
#define TAS6754_STATE_CH1_PLAY                              (0x04 << 4)         /* 0100: PLAY */
#define TAS6754_STATE_CH1_FAULT                             (0x05 << 4)         /* 0101: FAULT */
#define TAS6754_STATE_CH1_AUTOREC                           (0x06 << 4)         /* 0110: AUTOREC */
#define TAS6754_STATE_CH2_DEEPSLEEP                         (0x00 << 0)         /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH2_LOAD_DIAG                         (0x01 << 0)         /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH2_SLEEP                             (0x02 << 0)         /* 0010: SLEEP */
#define TAS6754_STATE_CH2_HIZ                               (0x03 << 0)         /* 0011: HI-Z */
#define TAS6754_STATE_CH2_PLAY                              (0x04 << 0)         /* 0100: PLAY */
#define TAS6754_STATE_CH2_FAULT                             (0x05 << 0)         /* 0101: FAULT */
#define TAS6754_STATE_CH2_AUTOREC                           (0x06 << 0)         /* 0110: AUTOREC */
#define TAS6754_GET_CH1_STATE(reg_val)                      ((reg_val & TAS6754_STATE_REPORT_CH1_MASK) >> 4)
#define TAS6754_GET_CH2_STATE(reg_val)                      (reg_val & TAS6754_STATE_REPORT_CH2_MASK)
/************************************************************************
* 					TAS6754_STATE_REPORT_CH3_CH4 (0x73)
************************************************************************/
#define TAS6754_STATE_REPORT_CH3_MASK                       GENMASK(7, 4)       /* Bits 7-4: CH3 STATUS */
#define TAS6754_STATE_REPORT_CH4_MASK                       GENMASK(3, 0)       /* Bits 3-0: CH4 STATUS */
#define TAS6754_STATE_CH3_DEEPSLEEP                         (0x00 << 4)         /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH3_LOAD_DIAG                         (0x01 << 4)         /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH3_SLEEP                             (0x02 << 4)         /* 0010: SLEEP */
#define TAS6754_STATE_CH3_HIZ                               (0x03 << 4)         /* 0011: HI-Z */
#define TAS6754_STATE_CH3_PLAY                              (0x04 << 4)         /* 0100: PLAY */
#define TAS6754_STATE_CH3_FAULT                             (0x05 << 4)         /* 0101: FAULT */
#define TAS6754_STATE_CH3_AUTOREC                           (0x06 << 4)         /* 0110: AUTOREC */
#define TAS6754_STATE_CH4_DEEPSLEEP                         (0x00 << 0)         /* 0000: DEEPSLEEP */
#define TAS6754_STATE_CH4_LOAD_DIAG                         (0x01 << 0)         /* 0001: LOAD DIAG */
#define TAS6754_STATE_CH4_SLEEP                             (0x02 << 0)         /* 0010: SLEEP */
#define TAS6754_STATE_CH4_HIZ                               (0x03 << 0)         /* 0011: HI-Z */
#define TAS6754_STATE_CH4_PLAY                              (0x04 << 0)         /* 0100: PLAY */
#define TAS6754_STATE_CH4_FAULT                             (0x05 << 0)         /* 0101: FAULT */
#define TAS6754_STATE_CH4_AUTOREC                           (0x06 << 0)         /* 0110: AUTOREC */
#define TAS6754_GET_CH3_STATE(reg_val)                      ((reg_val & TAS6754_STATE_REPORT_CH3_MASK) >> 4)
#define TAS6754_GET_CH4_STATE(reg_val)                      (reg_val & TAS6754_STATE_REPORT_CH4_MASK)
/************************************************************************
* 					TAS6754_PVDD_SENSE (0x74)
************************************************************************/
#define TAS6754_PVDD_SENSE_MASK                             (0xFF)              /* Bits 7-0: PVDD SENSE */
#define TAS6754_PVDD_VOLTAGE_SCALE                          (0.19)              /* Voltage scale factor: 0.19V per bit */
#define TAS6754_PVDD_VOLTAGE_TO_REG(volt)                   ((uint8_t)((volt) / TAS6754_PVDD_VOLTAGE_SCALE))
#define TAS6754_REG_TO_PVDD_VOLTAGE(reg_val)                ((float)(reg_val) * TAS6754_PVDD_VOLTAGE_SCALE)
#define TAS6754_PVDD_SENSE_0V                               (0x00)              /* 0000 0000: 0V */
#define TAS6754_PVDD_SENSE_5V                               (0x1A)              /* 0001 1010: ~5V (actually 4.94V) */
#define TAS6754_PVDD_SENSE_12V                              (0x40)              /* 0100 0000: ~12V (actually 12.16V) */
#define TAS6754_PVDD_SENSE_14_44V                           (0x4C)              /* 0100 1100: 14.44V */
#define TAS6754_PVDD_SENSE_24V                              (0x7F)              /* 0111 1111: ~24V (actually 24.13V) */
#define TAS6754_PVDD_SENSE_48V                              (0xFF)              /* 1111 1111: ~48V (actually 48.45V) */
/************************************************************************
* 					TAS6754_TEMP_GLOBAL (0x75)
************************************************************************/
#define TAS6754_TEMP_GLOBAL_MASK                            (0xFF)              /* Bits 7-0: GLOBAL TEMP SENSOR */
#define TAS6754_TEMP_KELVIN_SCALE                           (2.19)              /* Temperature scale factor: 2.19K per bit */
#define TAS6754_KELVIN_TO_CELSIUS_OFFSET                    (273.15)            /* Offset to convert Kelvin to Celsius */
#define TAS6754_REG_TO_TEMP_KELVIN(reg_val)                 ((float)(reg_val) * TAS6754_TEMP_KELVIN_SCALE)
#define TAS6754_REG_TO_TEMP_CELSIUS(reg_val)                (TAS6754_REG_TO_TEMP_KELVIN(reg_val) - TAS6754_KELVIN_TO_CELSIUS_OFFSET)
#define TAS6754_TEMP_CELSIUS_TO_REG(celsius)                ((uint8_t)(((celsius) + TAS6754_KELVIN_TO_CELSIUS_OFFSET) / TAS6754_TEMP_KELVIN_SCALE))
#define TAS6754_TEMP_0C                                     (0x7D)              /* ~0°C (actually 0.02°C) */
#define TAS6754_TEMP_25C                                    (0x8A)              /* ~25°C (actually 25.06°C) */
#define TAS6754_TEMP_50C                                    (0x96)              /* ~50°C (actually 50.09°C) */
#define TAS6754_TEMP_75C                                    (0xA3)              /* ~75°C (actually 75.12°C) */
#define TAS6754_TEMP_100C                                   (0xAF)              /* ~100°C (actually 100.16°C) */
#define TAS6754_TEMP_125C                                   (0xBC)              /* ~125°C (actually 125.19°C) */
/************************************************************************
* 					TAS6754_FS_MON (0x76)
************************************************************************/
#define TAS6754_FS_MON_RESERVED_MASK                        GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_FS_MON_SCLK_RATIO_MSB_MASK                  GENMASK(5, 4)       /* Bits 5-4: SCLK RATIO MSB */
#define TAS6754_FS_MON_DETECTED_SAMPLE_RATE_MASK            GENMASK(3, 0)       /* Bits 3-0: DETECTED SAMPLE RATE */
#define TAS6754_FS_MON_SCLK_RATIO_MSB_SHIFT                 (4)                 /* Shift value for MSB bits */
#define TAS6754_FS_MON_SAMPLE_RATE_ERROR                    (0x00 << 0)         /* 0000: FS Error */
#define TAS6754_FS_MON_SAMPLE_RATE_48KHZ                    (0x09 << 0)         /* 1001: 48KHz */
#define TAS6754_FS_MON_SAMPLE_RATE_96KHZ                    (0x0B << 0)         /* 1011: 96KHz */
#define TAS6754_FS_MON_SAMPLE_RATE_192KHZ                   (0x0D << 0)         /* 1101: 192KHz */
#define TAS6754_GET_DETECTED_SAMPLE_RATE(reg_val)           (reg_val & TAS6754_FS_MON_DETECTED_SAMPLE_RATE_MASK)
#define TAS6754_GET_SCLK_RATIO_MSB(reg_val)                 ((reg_val & TAS6754_FS_MON_SCLK_RATIO_MSB_MASK) >> TAS6754_FS_MON_SCLK_RATIO_MSB_SHIFT)
/************************************************************************
* 					TAS6754_SCLK_MON (0x77)
************************************************************************/
#define TAS6754_SCLK_MON_RATIO_LSB_MASK                     (0xFF)              /* Bits 7-0: SCLK RATIO LSB */
#define TAS6754_SCLK_RATIO_32FS_LSB                         (0x20)              /* LSB part of 32Fs ratio */
#define TAS6754_SCLK_RATIO_64FS_LSB                         (0x40)              /* LSB part of 64Fs ratio */
#define TAS6754_SCLK_RATIO_128FS_LSB                        (0x80)              /* LSB part of 128Fs ratio */
#define TAS6754_SCLK_RATIO_256FS_LSB                        (0x00)              /* LSB part of 256Fs ratio (with MSB=01) */
#define TAS6754_SCLK_RATIO_512FS_LSB                        (0x00)              /* LSB part of 512Fs ratio (with MSB=10) */
#define TAS6754_GET_SCLK_RATIO(msb_reg_val, lsb_reg_val) \
    ((TAS6754_GET_SCLK_RATIO_MSB(msb_reg_val) << 8) | (lsb_reg_val))
#define TAS6754_SCLK_RATIO_32FS                             (0x020)             /* 00 0010 0000: 32Fs */
#define TAS6754_SCLK_RATIO_64FS                             (0x040)             /* 00 0100 0000: 64Fs */
#define TAS6754_SCLK_RATIO_128FS                            (0x080)             /* 00 1000 0000: 128Fs */
#define TAS6754_SCLK_RATIO_256FS                            (0x100)             /* 01 0000 0000: 256Fs */
#define TAS6754_SCLK_RATIO_512FS                            (0x200)             /* 10 0000 0000: 512Fs */
/************************************************************************
* 					TAS6754_REPORT_ROUTING_1 (0x7C)
************************************************************************/
#define TAS6754_REPORT_ROUTING_1_RESERVED_7_MASK            BIT(7)              /* Bit 7: Reserved */
#define TAS6754_REPORT_ROUTING_1_OUTM_STORED_MASK           BIT(6)              /* Bit 6: OUTM STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_CP_UV_STORED_MASK          BIT(5)              /* Bit 5: CP UV STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_CP_STORED_MASK             BIT(4)              /* Bit 4: CP STORED GPIO */
#define TAS6754_REPORT_ROUTING_1_RESERVED_3_MASK            BIT(3)              /* Bit 3: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_2_MASK            BIT(2)              /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_1_MASK            BIT(1)              /* Bit 1: Reserved */
#define TAS6754_REPORT_ROUTING_1_RESERVED_0_MASK            BIT(0)              /* Bit 0: Reserved */
#define TAS6754_REPORT_ROUTING_1_OUTM_NOT_REPORTED          (0x00 << 6)         /* 0: Latched OUTM soft short fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_OUTM_REPORTED              (0x01 << 6)         /* 1: Latched OUTM soft short fault is reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_UV_NOT_REPORTED         (0x00 << 5)         /* 0: Latched charge pump UVLO fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_UV_REPORTED             (0x01 << 5)         /* 1: Latched charge pump UVLO fault is reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_NOT_REPORTED            (0x00 << 4)         /* 0: Latched charge pump fault is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_CP_REPORTED                (0x01 << 4)         /* 1: Latched charge pump fault is reported to FAULT */
#define TAS6754_REPORT_ROUTING_1_RESERVED_MASK              (TAS6754_REPORT_ROUTING_1_RESERVED_7_MASK | \
                                                            TAS6754_REPORT_ROUTING_1_RESERVED_3_MASK | \
                                                            TAS6754_REPORT_ROUTING_1_RESERVED_2_MASK | \
                                                            TAS6754_REPORT_ROUTING_1_RESERVED_1_MASK | \
                                                            TAS6754_REPORT_ROUTING_1_RESERVED_0_MASK)
/************************************************************************
* 					TAS6754_POWER_FAULT_STATUS_1 (0x7D)
************************************************************************/
#define TAS6754_POWER_FAULT_1_RESERVED_MASK                 GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_POWER_FAULT_1_CH4_SOFT_SHORT_MASK           BIT(5)              /* Bit 5: CH4 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH3_SOFT_SHORT_MASK           BIT(4)              /* Bit 4: CH3 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH2_SOFT_SHORT_MASK           BIT(3)              /* Bit 3: CH2 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CH1_SOFT_SHORT_MASK           BIT(2)              /* Bit 2: CH1 SOFT SHORT STORED */
#define TAS6754_POWER_FAULT_1_CP_UVLO_STORED_MASK           BIT(1)              /* Bit 1: CP UVLO FAULT STORED */
#define TAS6754_POWER_FAULT_1_CP_UVLO_MASK                  BIT(0)              /* Bit 0: CP UVLO FAULT */
#define TAS6754_POWER_FAULT_1_CH4_NO_SHORT                  (0x00 << 5)         /* 0: No Channel 4 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH4_SHORT_STORED              (0x01 << 5)         /* 1: Channel 4 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH3_NO_SHORT                  (0x00 << 4)         /* 0: No Channel 3 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH3_SHORT_STORED              (0x01 << 4)         /* 1: Channel 3 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH2_NO_SHORT                  (0x00 << 3)         /* 0: No Channel 2 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH2_SHORT_STORED              (0x01 << 3)         /* 1: Channel 2 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH1_NO_SHORT                  (0x00 << 2)         /* 0: No Channel 1 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CH1_SHORT_STORED              (0x01 << 2)         /* 1: Channel 1 OUTM soft short fault event stored */
#define TAS6754_POWER_FAULT_1_CP_UVLO_NO_STORED             (0x00 << 1)         /* 0: No charge pump UVLO fault event stored */
#define TAS6754_POWER_FAULT_1_CP_UVLO_STORED                (0x01 << 1)         /* 1: Charge pump UVLO fault event stored */
#define TAS6754_POWER_FAULT_1_CP_UVLO_NO_FAULT              (0x00 << 0)         /* 0: No charge pump UVLO fault */
#define TAS6754_POWER_FAULT_1_CP_UVLO_FAULT                 (0x01 << 0)         /* 1: Charge pump UVLO fault happens */
#define TAS6754_POWER_FAULT_1_ALL_CH_SHORT_MASK             (TAS6754_POWER_FAULT_1_CH4_SOFT_SHORT_MASK | \
                                                            TAS6754_POWER_FAULT_1_CH3_SOFT_SHORT_MASK | \
                                                            TAS6754_POWER_FAULT_1_CH2_SOFT_SHORT_MASK | \
                                                            TAS6754_POWER_FAULT_1_CH1_SOFT_SHORT_MASK)
/************************************************************************
* 					TAS6754_POWER_FAULT_STATUS_2 (0x80)
************************************************************************/
#define TAS6754_POWER_FAULT_2_GLOBAL_WARNING_MASK           BIT(7)              /* Bit 7: GLOBAL WARNING STATUS */
#define TAS6754_POWER_FAULT_2_GLOBAL_FAULT_MASK             BIT(6)              /* Bit 6: GLOBAL FAULT STATUS */
#define TAS6754_POWER_FAULT_2_RESERVED_MASK                 BIT(5)              /* Bit 5: Reserved */
#define TAS6754_POWER_FAULT_2_DVDD_UV_MASK                  BIT(4)              /* Bit 4: DVDD UV STATUS */
#define TAS6754_POWER_FAULT_2_PVDD_OV_MASK                  BIT(3)              /* Bit 3: PVDD OV STATUS */
#define TAS6754_POWER_FAULT_2_VBAT_OV_MASK                  BIT(2)              /* Bit 2: VBAT OV STATUS */
#define TAS6754_POWER_FAULT_2_PVDD_UV_MASK                  BIT(1)              /* Bit 1: PVDD UV STATUS */
#define TAS6754_POWER_FAULT_2_VBAT_UV_MASK                  BIT(0)              /* Bit 0: VBAT UV STATUS */
#define TAS6754_POWER_FAULT_2_NO_WARNING                    (0x00 << 7)         /* 0: No warning */
#define TAS6754_POWER_FAULT_2_WARNING_ACTIVE                (0x01 << 7)         /* 1: Warning active in device */
#define TAS6754_POWER_FAULT_2_NO_FAULT                      (0x00 << 6)         /* 0: No fault */
#define TAS6754_POWER_FAULT_2_FAULT_ACTIVE                  (0x01 << 6)         /* 1: Fault active in device */
#define TAS6754_POWER_FAULT_2_DVDD_UV_NORMAL                (0x00 << 4)         /* 0: DVDD supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_DVDD_UV_FAULT                 (0x01 << 4)         /* 1: DVDD supply voltage is below UV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_OV_NORMAL                (0x00 << 3)         /* 0: PVDD supply voltage is below OV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_OV_FAULT                 (0x01 << 3)         /* 1: PVDD supply voltage is above OV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_OV_NORMAL                (0x00 << 2)         /* 0: VBAT supply voltage is below OV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_OV_FAULT                 (0x01 << 2)         /* 1: VBAT supply voltage is above OV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_UV_NORMAL                (0x00 << 1)         /* 0: PVDD supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_PVDD_UV_FAULT                 (0x01 << 1)         /* 1: PVDD supply voltage is below UV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_UV_NORMAL                (0x00 << 0)         /* 0: VBAT supply voltage is above UV threshold */
#define TAS6754_POWER_FAULT_2_VBAT_UV_FAULT                 (0x01 << 0)         /* 1: VBAT supply voltage is below UV threshold */
#define TAS6754_POWER_FAULT_2_ALL_SUPPLY_MASK               (TAS6754_POWER_FAULT_2_DVDD_UV_MASK | \
                                                            TAS6754_POWER_FAULT_2_PVDD_OV_MASK | \
                                                            TAS6754_POWER_FAULT_2_VBAT_OV_MASK | \
                                                            TAS6754_POWER_FAULT_2_PVDD_UV_MASK | \
                                                            TAS6754_POWER_FAULT_2_VBAT_UV_MASK)
/************************************************************************
* 					TAS6754_OT_FAULT (0x81)
************************************************************************/
#define TAS6754_OT_FAULT_GLOBAL_WARNING_MASK                BIT(7)              /* Bit 7: GLOBAL WARNING */
#define TAS6754_OT_FAULT_GLOBAL_FAULT_MASK                  BIT(6)              /* Bit 6: GLOBAL FAULT */
#define TAS6754_OT_FAULT_CP_FAULT_MASK                      BIT(5)              /* Bit 5: CP FAULT STATUS */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_MASK                   BIT(4)              /* Bit 4: GLOBAL OTSD STATUS */
#define TAS6754_OT_FAULT_CH1_OTSD_MASK                      BIT(3)              /* Bit 3: CH1 OTSD STATUS */
#define TAS6754_OT_FAULT_CH2_OTSD_MASK                      BIT(2)              /* Bit 2: CH2 OTSD STATUS */
#define TAS6754_OT_FAULT_CH3_OTSD_MASK                      BIT(1)              /* Bit 1: CH3 OTSD STATUS */
#define TAS6754_OT_FAULT_CH4_OTSD_MASK                      BIT(0)              /* Bit 0: CH4 OTSD STATUS */
#define TAS6754_OT_FAULT_NO_WARNING                         (0x00 << 7)         /* 0: No warning */
#define TAS6754_OT_FAULT_WARNING_TRIGGERED                  (0x01 << 7)         /* 1: Any warning triggered */
#define TAS6754_OT_FAULT_NO_FAULT                           (0x00 << 6)         /* 0: No fault */
#define TAS6754_OT_FAULT_FAULT_TRIGGERED                    (0x01 << 6)         /* 1: Any fault triggered */
#define TAS6754_OT_FAULT_CP_NORMAL                          (0x00 << 5)         /* 0: No charge pump fault detected */
#define TAS6754_OT_FAULT_CP_FAULT                           (0x01 << 5)         /* 1: Charge pump fault detected */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_NORMAL                 (0x00 << 4)         /* 0: Global die temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_GLOBAL_OTSD_FAULT                  (0x01 << 4)         /* 1: Global die temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH1_OTSD_NORMAL                    (0x00 << 3)         /* 0: Channel 1 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH1_OTSD_FAULT                     (0x01 << 3)         /* 1: Channel 1 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH2_OTSD_NORMAL                    (0x00 << 2)         /* 0: Channel 2 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH2_OTSD_FAULT                     (0x01 << 2)         /* 1: Channel 2 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH3_OTSD_NORMAL                    (0x00 << 1)         /* 0: Channel 3 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH3_OTSD_FAULT                     (0x01 << 1)         /* 1: Channel 3 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_CH4_OTSD_NORMAL                    (0x00 << 0)         /* 0: Channel 4 temperature is below OTSD threshold */
#define TAS6754_OT_FAULT_CH4_OTSD_FAULT                     (0x01 << 0)         /* 1: Channel 4 temperature is above OTSD threshold */
#define TAS6754_OT_FAULT_ALL_CH_OTSD_MASK                   (TAS6754_OT_FAULT_CH1_OTSD_MASK | \
                                                            TAS6754_OT_FAULT_CH2_OTSD_MASK | \
                                                            TAS6754_OT_FAULT_CH3_OTSD_MASK | \
                                                            TAS6754_OT_FAULT_CH4_OTSD_MASK)
/************************************************************************
* 					TAS6754_OTW_STATUS (0x82)
************************************************************************/
#define TAS6754_OTW_STATUS_RESERVED_MASK                    GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_OTW_STATUS_GLOBAL_OTW_MASK                  BIT(4)              /* Bit 4: GLOBAL OTW STATUS */
#define TAS6754_OTW_STATUS_CH1_OTW_MASK                     BIT(3)              /* Bit 3: CH1 OTW STATUS */
#define TAS6754_OTW_STATUS_CH2_OTW_MASK                     BIT(2)              /* Bit 2: CH2 OTW STATUS */
#define TAS6754_OTW_STATUS_CH3_OTW_MASK                     BIT(1)              /* Bit 1: CH3 OTW STATUS */
#define TAS6754_OTW_STATUS_CH4_OTW_MASK                     BIT(0)              /* Bit 0: CH4 OTW STATUS */
#define TAS6754_OTW_STATUS_GLOBAL_NORMAL                    (0x00 << 4)         /* 0: Global die temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_GLOBAL_WARNING                   (0x01 << 4)         /* 1: Global die temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH1_NORMAL                       (0x00 << 3)         /* 0: Channel 1 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH1_WARNING                      (0x01 << 3)         /* 1: Channel 1 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH2_NORMAL                       (0x00 << 2)         /* 0: Channel 2 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH2_WARNING                      (0x01 << 2)         /* 1: Channel 2 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH3_NORMAL                       (0x00 << 1)         /* 0: Channel 3 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH3_WARNING                      (0x01 << 1)         /* 1: Channel 3 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_CH4_NORMAL                       (0x00 << 0)         /* 0: Channel 4 temperature is below OTW threshold */
#define TAS6754_OTW_STATUS_CH4_WARNING                      (0x01 << 0)         /* 1: Channel 4 temperature is above OTW threshold */
#define TAS6754_OTW_STATUS_ALL_CH_MASK                      (TAS6754_OTW_STATUS_CH1_OTW_MASK | \
                                                            TAS6754_OTW_STATUS_CH2_OTW_MASK | \
                                                            TAS6754_OTW_STATUS_CH3_OTW_MASK | \
                                                            TAS6754_OTW_STATUS_CH4_OTW_MASK)
/************************************************************************
* 					TAS6754_CLIP_WARN_STATUS (0x83)
************************************************************************/
#define TAS6754_CLIP_WARN_STATUS_RESERVED_MASK              GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_CLIP_WARN_STATUS_CH1_MASK                   BIT(3)              /* Bit 3: CH1 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH2_MASK                   BIT(2)              /* Bit 2: CH2 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH3_MASK                   BIT(1)              /* Bit 1: CH3 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH4_MASK                   BIT(0)              /* Bit 0: CH4 CLIP STATUS */
#define TAS6754_CLIP_WARN_STATUS_CH1_NORMAL                 (0x00 << 3)         /* 0: Channel 1 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH1_CLIPPING               (0x01 << 3)         /* 1: Channel 1 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH2_NORMAL                 (0x00 << 2)         /* 0: Channel 2 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH2_CLIPPING               (0x01 << 2)         /* 1: Channel 2 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH3_NORMAL                 (0x00 << 1)         /* 0: Channel 3 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH3_CLIPPING               (0x01 << 1)         /* 1: Channel 3 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_CH4_NORMAL                 (0x00 << 0)         /* 0: Channel 4 clipping is not present or below threshold */
#define TAS6754_CLIP_WARN_STATUS_CH4_CLIPPING               (0x01 << 0)         /* 1: Channel 4 clipping is above clip detect threshold */
#define TAS6754_CLIP_WARN_STATUS_ALL_CH_MASK                (TAS6754_CLIP_WARN_STATUS_CH1_MASK | \
                                                            TAS6754_CLIP_WARN_STATUS_CH2_MASK | \
                                                            TAS6754_CLIP_WARN_STATUS_CH3_MASK | \
                                                            TAS6754_CLIP_WARN_STATUS_CH4_MASK)
/************************************************************************
* 					TAS6754_CBC_WARNING_STATUS (0x85)
************************************************************************/
#define TAS6754_CBC_WARNING_CH1_MASK                        BIT(7)              /* Bit 7: CH1 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH2_MASK                        BIT(6)              /* Bit 6: CH2 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH3_MASK                        BIT(5)              /* Bit 5: CH3 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_CH4_MASK                        BIT(4)              /* Bit 4: CH4 CBC WARN STATUS */
#define TAS6754_CBC_WARNING_RESERVED_MASK                   GENMASK(3, 0)       /* Bits 3-0: Reserved */
#define TAS6754_CBC_WARNING_CH1_NORMAL                      (0x00 << 7)         /* 0: Channel 1 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH1_PRESENT                     (0x01 << 7)         /* 1: Channel 1 CBC warning is present */
#define TAS6754_CBC_WARNING_CH2_NORMAL                      (0x00 << 6)         /* 0: Channel 2 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH2_PRESENT                     (0x01 << 6)         /* 1: Channel 2 CBC warning is present */
#define TAS6754_CBC_WARNING_CH3_NORMAL                      (0x00 << 5)         /* 0: Channel 3 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH3_PRESENT                     (0x01 << 5)         /* 1: Channel 3 CBC warning is present */
#define TAS6754_CBC_WARNING_CH4_NORMAL                      (0x00 << 4)         /* 0: Channel 4 CBC warning is not present */
#define TAS6754_CBC_WARNING_CH4_PRESENT                     (0x01 << 4)         /* 1: Channel 4 CBC warning is present */
#define TAS6754_CBC_WARNING_ALL_CH_MASK                     (TAS6754_CBC_WARNING_CH1_MASK | \
                                                            TAS6754_CBC_WARNING_CH2_MASK | \
                                                            TAS6754_CBC_WARNING_CH3_MASK | \
                                                            TAS6754_CBC_WARNING_CH4_MASK)
/************************************************************************
* 					TAS6754_POWER_FAULT_LATCHED (0x86)
************************************************************************/
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK           BIT(7)              /* Bit 7: DVDD POR STORED */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_6_MASK         BIT(6)              /* Bit 6: Reserved */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_5_MASK         BIT(5)              /* Bit 5: Reserved */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK            BIT(4)              /* Bit 4: DVDD UV STORED */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK            BIT(3)              /* Bit 3: PVDD OV STORED */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK            BIT(2)              /* Bit 2: VBAT OV STORED */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK            BIT(1)              /* Bit 1: PVDD UV STORED */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK            BIT(0)              /* Bit 0: VBAT UV STORED */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_NONE           (0x00 << 7)         /* 0: No DVDD power on reset event stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_POR_STORED         (0x01 << 7)         /* 1: DVDD power on reset event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_NONE            (0x00 << 4)         /* 0: No DVDD under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_DVDD_UV_STORED          (0x01 << 4)         /* 1: DVDD under voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_NONE            (0x00 << 3)         /* 0: No PVDD over voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_OV_STORED          (0x01 << 3)         /* 1: PVDD over voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_NONE            (0x00 << 2)         /* 0: No VBAT over voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_OV_STORED          (0x01 << 2)         /* 1: VBAT over voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_NONE            (0x00 << 1)         /* 0: No PVDD under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_PVDD_UV_STORED          (0x01 << 1)         /* 1: PVDD under voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_NONE            (0x00 << 0)         /* 0: No VBAT under voltage event stored */
#define TAS6754_POWER_FAULT_LATCHED_VBAT_UV_STORED          (0x01 << 0)         /* 1: VBAT under voltage event detected and stored */
#define TAS6754_POWER_FAULT_LATCHED_RESERVED_MASK           (TAS6754_POWER_FAULT_LATCHED_RESERVED_6_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_RESERVED_5_MASK)
#define TAS6754_POWER_FAULT_LATCHED_ALL_MASK                (TAS6754_POWER_FAULT_LATCHED_DVDD_POR_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_DVDD_UV_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_PVDD_OV_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_VBAT_OV_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_PVDD_UV_MASK | \
                                                            TAS6754_POWER_FAULT_LATCHED_VBAT_UV_MASK)
/************************************************************************
* 					TAS6754_OTSD_LATCHED (0x87)
************************************************************************/
#define TAS6754_OTSD_LATCHED_RESERVED_7_6_MASK              GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_OTSD_LATCHED_RESERVED_5_MASK                BIT(5)              /* Bit 5: Reserved */
#define TAS6754_OTSD_LATCHED_GLOBAL_MASK                    BIT(4)              /* Bit 4: GLOBAL OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH1_MASK                       BIT(3)              /* Bit 3: CH1 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH2_MASK                       BIT(2)              /* Bit 2: CH2 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH3_MASK                       BIT(1)              /* Bit 1: CH3 OTSD STORED */
#define TAS6754_OTSD_LATCHED_CH4_MASK                       BIT(0)              /* Bit 0: CH4 OTSD STORED */
#define TAS6754_OTSD_LATCHED_GLOBAL_NONE                    (0x00 << 4)         /* 0: No global over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_GLOBAL_STORED                  (0x01 << 4)         /* 1: Global over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH1_NONE                       (0x00 << 3)         /* 0: No Channel 1 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH1_STORED                     (0x01 << 3)         /* 1: Channel 1 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH2_NONE                       (0x00 << 2)         /* 0: No Channel 2 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH2_STORED                     (0x01 << 2)         /* 1: Channel 2 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH3_NONE                       (0x00 << 1)         /* 0: No Channel 3 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH3_STORED                     (0x01 << 1)         /* 1: Channel 3 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_CH4_NONE                       (0x00 << 0)         /* 0: No Channel 4 over temperature shutdown event stored */
#define TAS6754_OTSD_LATCHED_CH4_STORED                     (0x01 << 0)         /* 1: Channel 4 over temperature shutdown event detected and stored */
#define TAS6754_OTSD_LATCHED_RESERVED_MASK                  (TAS6754_OTSD_LATCHED_RESERVED_7_6_MASK | \
                                                            TAS6754_OTSD_LATCHED_RESERVED_5_MASK)
#define TAS6754_OTSD_LATCHED_ALL_CH_MASK                    (TAS6754_OTSD_LATCHED_CH1_MASK | \
                                                            TAS6754_OTSD_LATCHED_CH2_MASK | \
                                                            TAS6754_OTSD_LATCHED_CH3_MASK | \
                                                            TAS6754_OTSD_LATCHED_CH4_MASK)
/************************************************************************
* 					TAS6754_OTW_LATCHED (0x88)
************************************************************************/
#define TAS6754_OTW_LATCHED_RESERVED_MASK                   GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_OTW_LATCHED_GLOBAL_MASK                     BIT(4)              /* Bit 4: GLOBAL OTW STORED */
#define TAS6754_OTW_LATCHED_CH1_MASK                        BIT(3)              /* Bit 3: CH1 OTW STORED */
#define TAS6754_OTW_LATCHED_CH2_MASK                        BIT(2)              /* Bit 2: CH2 OTW STORED */
#define TAS6754_OTW_LATCHED_CH3_MASK                        BIT(1)              /* Bit 1: CH3 OTW STORED */
#define TAS6754_OTW_LATCHED_CH4_MASK                        BIT(0)              /* Bit 0: CH4 OTW STORED */
#define TAS6754_OTW_LATCHED_GLOBAL_NONE                     (0x00 << 4)         /* 0: No global over temperature warning event stored */
#define TAS6754_OTW_LATCHED_GLOBAL_STORED                   (0x01 << 4)         /* 1: Global over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH1_NONE                        (0x00 << 3)         /* 0: No Channel 1 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH1_STORED                      (0x01 << 3)         /* 1: Channel 1 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH2_NONE                        (0x00 << 2)         /* 0: No Channel 2 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH2_STORED                      (0x01 << 2)         /* 1: Channel 2 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH3_NONE                        (0x00 << 1)         /* 0: No Channel 3 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH3_STORED                      (0x01 << 1)         /* 1: Channel 3 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_CH4_NONE                        (0x00 << 0)         /* 0: No Channel 4 over temperature warning event stored */
#define TAS6754_OTW_LATCHED_CH4_STORED                      (0x01 << 0)         /* 1: Channel 4 over temperature warning event detected and stored */
#define TAS6754_OTW_LATCHED_ALL_CH_MASK                     (TAS6754_OTW_LATCHED_CH1_MASK | \
                                                            TAS6754_OTW_LATCHED_CH2_MASK | \
                                                            TAS6754_OTW_LATCHED_CH3_MASK | \
                                                            TAS6754_OTW_LATCHED_CH4_MASK)
/************************************************************************
* 					TAS6754_CLIP_WARN_LATCHED (0x89)
************************************************************************/
#define TAS6754_CLIP_WARN_LATCHED_RESERVED_MASK             GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_CLIP_WARN_LATCHED_CH1_MASK                  BIT(3)              /* Bit 3: CH1 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH2_MASK                  BIT(2)              /* Bit 2: CH2 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH3_MASK                  BIT(1)              /* Bit 1: CH3 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH4_MASK                  BIT(0)              /* Bit 0: CH4 CLIP STORED */
#define TAS6754_CLIP_WARN_LATCHED_CH1_NONE                  (0x00 << 3)         /* 0: No Channel 1 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH1_STORED                (0x01 << 3)         /* 1: Channel 1 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH2_NONE                  (0x00 << 2)         /* 0: No Channel 2 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH2_STORED                (0x01 << 2)         /* 1: Channel 2 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH3_NONE                  (0x00 << 1)         /* 0: No Channel 3 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH3_STORED                (0x01 << 1)         /* 1: Channel 3 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_CH4_NONE                  (0x00 << 0)         /* 0: No Channel 4 clipping event stored */
#define TAS6754_CLIP_WARN_LATCHED_CH4_STORED                (0x01 << 0)         /* 1: Channel 4 clipping event detected and stored */
#define TAS6754_CLIP_WARN_LATCHED_ALL_CH_MASK               (TAS6754_CLIP_WARN_LATCHED_CH1_MASK | \
                                                            TAS6754_CLIP_WARN_LATCHED_CH2_MASK | \
                                                            TAS6754_CLIP_WARN_LATCHED_CH3_MASK | \
                                                            TAS6754_CLIP_WARN_LATCHED_CH4_MASK)
/************************************************************************
* 					TAS6754_CLK_FAULT_LATCHED (0x8A)
************************************************************************/
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_7_2_MASK         GENMASK(7, 2)       /* Bits 7-2: Reserved */
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_1_MASK           BIT(1)              /* Bit 1: Reserved */
#define TAS6754_CLK_FAULT_LATCHED_CLOCK_FAULT_MASK          BIT(0)              /* Bit 0: CLOCK FAULT STORED */
#define TAS6754_CLK_FAULT_LATCHED_NONE                      (0x00 << 0)         /* 0: No Clock Error event stored */
#define TAS6754_CLK_FAULT_LATCHED_STORED                    (0x01 << 0)         /* 1: Clock Error event stored */
#define TAS6754_CLK_FAULT_LATCHED_RESERVED_MASK             (TAS6754_CLK_FAULT_LATCHED_RESERVED_7_2_MASK | \
                                                            TAS6754_CLK_FAULT_LATCHED_RESERVED_1_MASK)
/************************************************************************
* 					TAS6754_RTLDG_OL_SL_FAULT_LATCHED (0x8B)
************************************************************************/
#define TAS6754_RTLDG_SL_CH1_MASK                           BIT(7)              /* Bit 7: CH1 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH2_MASK                           BIT(6)              /* Bit 6: CH2 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH3_MASK                           BIT(5)              /* Bit 5: CH3 RTLDG SL STORED */
#define TAS6754_RTLDG_SL_CH4_MASK                           BIT(4)              /* Bit 4: CH4 RTLDG SL STORED */
#define TAS6754_RTLDG_OL_CH1_MASK                           BIT(3)              /* Bit 3: CH1 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH2_MASK                           BIT(2)              /* Bit 2: CH2 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH3_MASK                           BIT(1)              /* Bit 1: CH3 RTLDG OL STORED */
#define TAS6754_RTLDG_OL_CH4_MASK                           BIT(0)              /* Bit 0: CH4 RTLDG OL STORED */
#define TAS6754_RTLDG_SL_CH1_NONE                           (0x00 << 7)         /* 0: No shorted load condition on Channel 1 */
#define TAS6754_RTLDG_SL_CH1_STORED                         (0x01 << 7)         /* 1: Shorted load condition on Channel 1 */
#define TAS6754_RTLDG_SL_CH2_NONE                           (0x00 << 6)         /* 0: No shorted load condition on Channel 2 */
#define TAS6754_RTLDG_SL_CH2_STORED                         (0x01 << 6)         /* 1: Shorted load condition on Channel 2 */
#define TAS6754_RTLDG_SL_CH3_NONE                           (0x00 << 5)         /* 0: No shorted load condition on Channel 3 */
#define TAS6754_RTLDG_SL_CH3_STORED                         (0x01 << 5)         /* 1: Shorted load condition on Channel 3 */
#define TAS6754_RTLDG_SL_CH4_NONE                           (0x00 << 4)         /* 0: No shorted load condition on Channel 4 */
#define TAS6754_RTLDG_SL_CH4_STORED                         (0x01 << 4)         /* 1: Shorted load condition on Channel 4 */
#define TAS6754_RTLDG_OL_CH1_NONE                           (0x00 << 3)         /* 0: No open load condition on Channel 1 */
#define TAS6754_RTLDG_OL_CH1_STORED                         (0x01 << 3)         /* 1: Open load condition on Channel 1 */
#define TAS6754_RTLDG_OL_CH2_NONE                           (0x00 << 2)         /* 0: No open load condition on Channel 2 */
#define TAS6754_RTLDG_OL_CH2_STORED                         (0x01 << 2)         /* 1: Open load condition on Channel 2 */
#define TAS6754_RTLDG_OL_CH3_NONE                           (0x00 << 1)         /* 0: No open load condition on Channel 3 */
#define TAS6754_RTLDG_OL_CH3_STORED                         (0x01 << 1)         /* 1: Open load condition on Channel 3 */
#define TAS6754_RTLDG_OL_CH4_NONE                           (0x00 << 0)         /* 0: No open load condition on Channel 4 */
#define TAS6754_RTLDG_OL_CH4_STORED                         (0x01 << 0)         /* 1: Open load condition on Channel 4 */
#define TAS6754_RTLDG_SL_ALL_CH_MASK                        (TAS6754_RTLDG_SL_CH1_MASK | \
                                                            TAS6754_RTLDG_SL_CH2_MASK | \
                                                            TAS6754_RTLDG_SL_CH3_MASK | \
                                                            TAS6754_RTLDG_SL_CH4_MASK)
#define TAS6754_RTLDG_OL_ALL_CH_MASK                        (TAS6754_RTLDG_OL_CH1_MASK | \
                                                            TAS6754_RTLDG_OL_CH2_MASK | \
                                                            TAS6754_RTLDG_OL_CH3_MASK | \
                                                            TAS6754_RTLDG_OL_CH4_MASK)
/************************************************************************
* 					TAS6754_CBC_FAULT_WARN_LATCHED (0x8D)
************************************************************************/
#define TAS6754_CBC_WARN_CH1_MASK                           BIT(7)              /* Bit 7: CH1 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH2_MASK                           BIT(6)              /* Bit 6: CH2 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH3_MASK                           BIT(5)              /* Bit 5: CH3 CBC WARN STORED */
#define TAS6754_CBC_WARN_CH4_MASK                           BIT(4)              /* Bit 4: CH4 CBC WARN STORED */
#define TAS6754_CBC_FAULT_CH1_MASK                          BIT(3)              /* Bit 3: CH1 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH2_MASK                          BIT(2)              /* Bit 2: CH2 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH3_MASK                          BIT(1)              /* Bit 1: CH3 CBC FAULT STORED */
#define TAS6754_CBC_FAULT_CH4_MASK                          BIT(0)              /* Bit 0: CH4 CBC FAULT STORED */
#define TAS6754_CBC_WARN_CH1_NONE                           (0x00 << 7)         /* 0: No Channel 1 load current warning event stored */
#define TAS6754_CBC_WARN_CH1_STORED                         (0x01 << 7)         /* 1: Channel 1 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH2_NONE                           (0x00 << 6)         /* 0: No Channel 2 load current warning event stored */
#define TAS6754_CBC_WARN_CH2_STORED                         (0x01 << 6)         /* 1: Channel 2 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH3_NONE                           (0x00 << 5)         /* 0: No Channel 3 load current warning event stored */
#define TAS6754_CBC_WARN_CH3_STORED                         (0x01 << 5)         /* 1: Channel 3 load current warning event detected and stored */
#define TAS6754_CBC_WARN_CH4_NONE                           (0x00 << 4)         /* 0: No Channel 4 load current warning event stored */
#define TAS6754_CBC_WARN_CH4_STORED                         (0x01 << 4)         /* 1: Channel 4 load current warning event detected and stored */
#define TAS6754_CBC_FAULT_CH1_NONE                          (0x00 << 3)         /* 0: No Channel 1 load current fault event stored */
#define TAS6754_CBC_FAULT_CH1_STORED                        (0x01 << 3)         /* 1: Channel 1 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH2_NONE                          (0x00 << 2)         /* 0: No Channel 2 load current fault event stored */
#define TAS6754_CBC_FAULT_CH2_STORED                        (0x01 << 2)         /* 1: Channel 2 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH3_NONE                          (0x00 << 1)         /* 0: No Channel 3 load current fault event stored */
#define TAS6754_CBC_FAULT_CH3_STORED                        (0x01 << 1)         /* 1: Channel 3 load current fault event detected and stored */
#define TAS6754_CBC_FAULT_CH4_NONE                          (0x00 << 0)         /* 0: No Channel 4 load current fault event stored */
#define TAS6754_CBC_FAULT_CH4_STORED                        (0x01 << 0)         /* 1: Channel 4 load current fault event detected and stored */
#define TAS6754_CBC_WARN_ALL_CH_MASK                        (TAS6754_CBC_WARN_CH1_MASK | \
                                                            TAS6754_CBC_WARN_CH2_MASK | \
                                                            TAS6754_CBC_WARN_CH3_MASK | \
                                                            TAS6754_CBC_WARN_CH4_MASK)
#define TAS6754_CBC_FAULT_ALL_CH_MASK                       (TAS6754_CBC_FAULT_CH1_MASK | \
                                                            TAS6754_CBC_FAULT_CH2_MASK | \
                                                            TAS6754_CBC_FAULT_CH3_MASK | \
                                                            TAS6754_CBC_FAULT_CH4_MASK)
/************************************************************************
* 					TAS6754_OC_DC_FAULT_LATCHED (0x8E)
************************************************************************/
#define TAS6754_OC_FAULT_CH1_MASK                           BIT(7)              /* Bit 7: CH1 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH2_MASK                           BIT(6)              /* Bit 6: CH2 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH3_MASK                           BIT(5)              /* Bit 5: CH3 OC FAULT STORED */
#define TAS6754_OC_FAULT_CH4_MASK                           BIT(4)              /* Bit 4: CH4 OC FAULT STORED */
#define TAS6754_DC_FAULT_CH1_MASK                           BIT(3)              /* Bit 3: CH1 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH2_MASK                           BIT(2)              /* Bit 2: CH2 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH3_MASK                           BIT(1)              /* Bit 1: CH3 DC FAULT STORED */
#define TAS6754_DC_FAULT_CH4_MASK                           BIT(0)              /* Bit 0: CH4 DC FAULT STORED */
#define TAS6754_OC_FAULT_CH1_NONE                           (0x00 << 7)         /* 0: No Channel 1 over current fault event stored */
#define TAS6754_OC_FAULT_CH1_STORED                         (0x01 << 7)         /* 1: Channel 1 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH2_NONE                           (0x00 << 6)         /* 0: No Channel 2 over current fault event stored */
#define TAS6754_OC_FAULT_CH2_STORED                         (0x01 << 6)         /* 1: Channel 2 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH3_NONE                           (0x00 << 5)         /* 0: No Channel 3 over current fault event stored */
#define TAS6754_OC_FAULT_CH3_STORED                         (0x01 << 5)         /* 1: Channel 3 over current fault event detected and stored */
#define TAS6754_OC_FAULT_CH4_NONE                           (0x00 << 4)         /* 0: No Channel 4 over current fault event stored */
#define TAS6754_OC_FAULT_CH4_STORED                         (0x01 << 4)         /* 1: Channel 4 over current fault event detected and stored */
#define TAS6754_DC_FAULT_CH1_NONE                           (0x00 << 3)         /* 0: No Channel 1 DC fault event stored */
#define TAS6754_DC_FAULT_CH1_STORED                         (0x01 << 3)         /* 1: Channel 1 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH2_NONE                           (0x00 << 2)         /* 0: No Channel 2 DC fault event stored */
#define TAS6754_DC_FAULT_CH2_STORED                         (0x01 << 2)         /* 1: Channel 2 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH3_NONE                           (0x00 << 1)         /* 0: No Channel 3 DC fault event stored */
#define TAS6754_DC_FAULT_CH3_STORED                         (0x01 << 1)         /* 1: Channel 3 DC fault event detected and stored */
#define TAS6754_DC_FAULT_CH4_NONE                           (0x00 << 0)         /* 0: No Channel 4 DC fault event stored */
#define TAS6754_DC_FAULT_CH4_STORED                         (0x01 << 0)         /* 1: Channel 4 DC fault event detected and stored */
#define TAS6754_OC_FAULT_ALL_CH_MASK                        (TAS6754_OC_FAULT_CH1_MASK | \
                                                            TAS6754_OC_FAULT_CH2_MASK | \
                                                            TAS6754_OC_FAULT_CH3_MASK | \
                                                            TAS6754_OC_FAULT_CH4_MASK)
#define TAS6754_DC_FAULT_ALL_CH_MASK                        (TAS6754_DC_FAULT_CH1_MASK | \
                                                            TAS6754_DC_FAULT_CH2_MASK | \
                                                            TAS6754_DC_FAULT_CH3_MASK | \
                                                            TAS6754_DC_FAULT_CH4_MASK)
/************************************************************************
* 					TAS6754_OTSD_RECOVERY_EN (0x8F)
************************************************************************/
#define TAS6754_OTSD_RECOVERY_RESERVED_7_3_MASK             GENMASK(7, 3)       /* Bits 7-3: Reserved */
#define TAS6754_OTSD_RECOVERY_RESERVED_2_MASK               BIT(2)              /* Bit 2: Reserved */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_EN_MASK              BIT(1)              /* Bit 1: OTSD AUTO REC ENABLE */
#define TAS6754_OTSD_RECOVERY_RESERVED_0_MASK               BIT(0)              /* Bit 0: Reserved */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_DISABLE              (0x00 << 1)         /* 0: Disable Overtemperature Shutdown Auto-recovery */
#define TAS6754_OTSD_RECOVERY_AUTO_REC_ENABLE               (0x01 << 1)         /* 1: Enable Overtemperature Shutdown Auto-recovery */
#define TAS6754_OTSD_RECOVERY_RESERVED_MASK                 (TAS6754_OTSD_RECOVERY_RESERVED_7_3_MASK | \
                                                            TAS6754_OTSD_RECOVERY_RESERVED_2_MASK | \
                                                            TAS6754_OTSD_RECOVERY_RESERVED_0_MASK)
/************************************************************************
* 					TAS6754_REPORT_ROUTING_2 (0x90)
************************************************************************/
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_MASK             BIT(7)              /* Bit 7: CBC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_RESERVED_6_MASK            BIT(6)              /* Bit 6: Reserved */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_MASK            BIT(5)              /* Bit 5: OTSD LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_MASK           BIT(4)              /* Bit 4: POWER LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_MASK                BIT(3)              /* Bit 3: DC LDG FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_RESERVED_2_MASK            BIT(2)              /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_2_OTSD_MASK                  BIT(1)              /* Bit 1: OTSD FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_POWER_MASK                 BIT(0)              /* Bit 0: POWER FAULT GPIO */
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_DISABLE          (0x00 << 7)         /* 0: Latching Overcurrent Limiting events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_CBC_LATCH_ENABLE           (0x01 << 7)         /* 1: Latching Overcurrent Limiting events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_DISABLE         (0x00 << 5)         /* 0: Latching Overtemperature Shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_LATCH_ENABLE          (0x01 << 5)         /* 1: Latching Overtemperature Shutdown events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_DISABLE        (0x00 << 4)         /* 0: Latching Power Fault events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_LATCH_ENABLE         (0x01 << 4)         /* 1: Latching Power Fault events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_DISABLE             (0x00 << 3)         /* 0: Non-Latched DC Load Diagnostic events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_DC_LDG_ENABLE              (0x01 << 3)         /* 1: Non-Latched DC Load Diagnostics events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_DISABLE               (0x00 << 1)         /* 0: Non-Latched Overtemperature Shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_OTSD_ENABLE                (0x01 << 1)         /* 1: Non-Latched Overtemperature Shutdown events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_DISABLE              (0x00 << 0)         /* 0: Non-Latching Power Fault events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_POWER_ENABLE               (0x01 << 0)         /* 1: Non-Latching Power Fault events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_2_RESERVED_MASK              (TAS6754_REPORT_ROUTING_2_RESERVED_6_MASK | \
                                                            TAS6754_REPORT_ROUTING_2_RESERVED_2_MASK)
/************************************************************************
* 					TAS6754_REPORT_ROUTING_3 (0x91)
************************************************************************/
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_MASK             BIT(7)              /* Bit 7: CBC LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_RESERVED_6_MASK            BIT(6)              /* Bit 6: Reserved */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_MASK            BIT(5)              /* Bit 5: OTSD LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_MASK           BIT(4)              /* Bit 4: POWER LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_MASK                BIT(3)              /* Bit 3: DC LDG WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_RESERVED_2_MASK            BIT(2)              /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_3_OTSD_MASK                  BIT(1)              /* Bit 1: OTSD WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_POWER_MASK                 BIT(0)              /* Bit 0: POWER WARN GPIO */
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_DISABLE          (0x00 << 7)         /* 0: Latching Overcurrent Limiting events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_CBC_LATCH_ENABLE           (0x01 << 7)         /* 1: Latching Overcurrent Limiting events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_DISABLE         (0x00 << 5)         /* 0: Latching Overtemperature Shutdown events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_LATCH_ENABLE          (0x01 << 5)         /* 1: Latching Overtemperature Shutdown events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_DISABLE        (0x00 << 4)         /* 0: Latching Power Fault events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_LATCH_ENABLE         (0x01 << 4)         /* 1: Latching Power Fault events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_DISABLE             (0x00 << 3)         /* 0: Non-Latched DC Load Diagnostic events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_DC_LDG_ENABLE              (0x01 << 3)         /* 1: Non-Latched DC Load Diagnostics events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_DISABLE               (0x00 << 1)         /* 0: Non-Latched Overtemperature Shutdown events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_OTSD_ENABLE                (0x01 << 1)         /* 1: Non-Latched Overtemperature Shutdown events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_DISABLE              (0x00 << 0)         /* 0: Non-Latching Power Fault events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_3_POWER_ENABLE               (0x01 << 0)         /* 1: Non-Latching Power Fault events are routed to WARN */
#define TAS6754_REPORT_ROUTING_3_RESERVED_MASK              (TAS6754_REPORT_ROUTING_3_RESERVED_6_MASK | \
                                                            TAS6754_REPORT_ROUTING_3_RESERVED_2_MASK)
/************************************************************************
* 					TAS6754_REPORT_ROUTING_4 (0x92)
************************************************************************/
#define TAS6754_REPORT_ROUTING_4_RESERVED_7_MASK            BIT(7)              /* Bit 7: Reserved */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_WARN_MASK       BIT(6)              /* Bit 6: CLIP LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_WARN_MASK        BIT(5)              /* Bit 5: OTW LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_OTW_WARN_MASK              BIT(4)              /* Bit 4: OTW WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_MASK               BIT(3)              /* Bit 3: PROT SD FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_MASK              BIT(2)              /* Bit 2: OC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_MASK              BIT(1)              /* Bit 1: DC LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_MASK            BIT(0)              /* Bit 0: FAULT WARN GPIO */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_DISABLE         (0x00 << 6)         /* 0: Latching Clip Detect events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_CLIP_LATCH_ENABLE          (0x01 << 6)         /* 1: Latching Clip Detect events are routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_DISABLE          (0x00 << 5)         /* 0: Latching Overtemperature Warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_LATCH_ENABLE           (0x01 << 5)         /* 1: Latching Overtemperature Warning events are routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_DISABLE                (0x00 << 4)         /* 0: Non-latched Overtemperature Warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_4_OTW_ENABLE                 (0x01 << 4)         /* 1: Non-latched Overtemperature Warning events are routed to WARN */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_DISABLE            (0x00 << 3)         /* 0: If any channel enters the FAULT state it is not reported to FAULT */
#define TAS6754_REPORT_ROUTING_4_PROT_SD_ENABLE             (0x01 << 3)         /* 1: If any channel enters the FAULT state it is reported to FAULT */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_DISABLE           (0x00 << 2)         /* 0: Latching Overcurrent shutdown events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_OC_LATCH_ENABLE            (0x01 << 2)         /* 1: Latching Overcurrent shutdown events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_DISABLE           (0x00 << 1)         /* 0: Latching DC Detect events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_DC_LATCH_ENABLE            (0x01 << 1)         /* 1: Latching DC Detect events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_DISABLE         (0x00 << 0)         /* 0: WARN pin signals are not routed to the FAULT pin */
#define TAS6754_REPORT_ROUTING_4_FAULT_WARN_ENABLE          (0x01 << 0)         /* 1: WARN pin signals are routed to the FAULT pin */
/************************************************************************
* 					TAS6754_CLIP_DETECT_CTRL (0x93)
************************************************************************/
#define TAS6754_CLIP_DETECT_RESERVED_7_MASK                 BIT(7)              /* Bit 7: Reserved */
#define TAS6754_CLIP_DETECT_ENABLE_MASK                     BIT(6)              /* Bit 6: CLIP DETECT ENABLE */
#define TAS6754_CLIP_DETECT_RESERVED_5_4_MASK               GENMASK(5, 4)       /* Bits 5-4: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_3_MASK                 BIT(3)              /* Bit 3: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_2_MASK                 BIT(2)              /* Bit 2: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_1_MASK                 BIT(1)              /* Bit 1: Reserved */
#define TAS6754_CLIP_DETECT_RESERVED_0_MASK                 BIT(0)              /* Bit 0: Reserved */
#define TAS6754_CLIP_DETECT_DISABLE                         (0x00 << 6)         /* 0: Disable Clip detect */
#define TAS6754_CLIP_DETECT_ENABLE                          (0x01 << 6)         /* 1: Enable Clip detect */
#define TAS6754_CLIP_DETECT_RESERVED_MASK                   (TAS6754_CLIP_DETECT_RESERVED_7_MASK | \
                                                            TAS6754_CLIP_DETECT_RESERVED_5_4_MASK | \
                                                            TAS6754_CLIP_DETECT_RESERVED_3_MASK | \
                                                            TAS6754_CLIP_DETECT_RESERVED_2_MASK | \
                                                            TAS6754_CLIP_DETECT_RESERVED_1_MASK | \
                                                            TAS6754_CLIP_DETECT_RESERVED_0_MASK)
/************************************************************************
* 					TAS6754_REPORT_ROUTING_5 (0x94)
************************************************************************/
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_MASK             BIT(7)              /* Bit 7: CLK FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_FAULT_MASK       BIT(6)              /* Bit 6: CLK LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_MASK              BIT(5)              /* Bit 5: CBC WARN FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_RTLDG_LATCH_FAULT_MASK     BIT(4)              /* Bit 4: RTLDG LATCH FAULT GPIO */
#define TAS6754_REPORT_ROUTING_5_RESERVED_3_MASK            BIT(3)              /* Bit 3: Reserved */
#define TAS6754_REPORT_ROUTING_5_RESERVED_2_MASK            BIT(2)              /* Bit 2: Reserved */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_MASK             BIT(1)              /* Bit 1: CLIP WARN GPIO */
#define TAS6754_REPORT_ROUTING_5_RTLDG_LATCH_WARN_MASK      BIT(0)              /* Bit 0: RTLDG LATCH WARN GPIO */
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_DISABLE          (0x00 << 7)         /* 0: Non-Latched Clock error events are not routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CLK_FAULT_ENABLE           (0x01 << 7)         /* 1: Non-Latched Clock error events are routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_DISABLE          (0x00 << 6)         /* 0: Latched Clock error events are not routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CLK_LATCH_ENABLE           (0x01 << 6)         /* 1: Latched Clock error events are routed to FAULT and WARN */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_DISABLE           (0x00 << 5)         /* 0: Unlatched CBC warning events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_CBC_WARN_ENABLE            (0x01 << 5)         /* 1: Unlatched CBC warning events are routed to WARN */
#define TAS6754_REPORT_ROUTING_5_RTLDG_F_DISABLE            (0x00 << 4)         /* 0: Latched Real-time load diagnostic events are not routed to FAULT */
#define TAS6754_REPORT_ROUTING_5_RTLDG_F_ENABLE             (0x01 << 4)         /* 1: Latched Real-time load diagnostic events are routed to FAULT */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_DISABLE          (0x00 << 1)         /* 0: Non-latched Clip Detect events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_CLIP_WARN_ENABLE           (0x01 << 1)         /* 1: Non-latched Clip Detect events are routed to WARN */
#define TAS6754_REPORT_ROUTING_5_RTLDG_W_DISABLE            (0x00 << 0)         /* 0: Latched Real-time load diagnostic events are not routed to WARN */
#define TAS6754_REPORT_ROUTING_5_RTLDG_W_ENABLE             (0x01 << 0)         /* 1: Latched Real-time load diagnostic events are routed to WARN */
#define TAS6754_REPORT_ROUTING_5_RESERVED_MASK              (TAS6754_REPORT_ROUTING_5_RESERVED_3_MASK | \
                                                            TAS6754_REPORT_ROUTING_5_RESERVED_2_MASK)
/************************************************************************
* 					TAS6754_GPIO1_OUTPUT_SELECT (0x95)
************************************************************************/
#define TAS6754_GPIO1_OUTPUT_RESERVED_7_6_MASK              GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_GPIO1_OUTPUT_RESERVED_5_MASK                BIT(5)              /* Bit 5: Reserved */
#define TAS6754_GPIO1_OUTPUT_SELECT_MASK                    GENMASK(4, 0)       /* Bits 4-0: GPIO1 OUTPUT */
#define TAS6754_GPIO1_OUTPUT_LOW                            (0x00)              /* 0x00: LOW */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_ALL                  (0x02)              /* 0x02: Auto Mute All Channels */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH4                  (0x03)              /* 0x03: Auto Mute Channel 4 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH3                  (0x04)              /* 0x04: Auto Mute Channel 3 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH2                  (0x05)              /* 0x05: Auto Mute Channel 2 */
#define TAS6754_GPIO1_OUTPUT_AUTO_MUTE_CH1                  (0x06)              /* 0x06: Auto Mute Channel 1 */
#define TAS6754_GPIO1_OUTPUT_SDOUT2                         (0x08)              /* 0x08: SDOUT2 */
#define TAS6754_GPIO1_OUTPUT_SDOUT1                         (0x09)              /* 0x09: SDOUT1 */
#define TAS6754_GPIO1_OUTPUT_WARN                           (0x0A)              /* 0x0A: WARN */
#define TAS6754_GPIO1_OUTPUT_FAULT                          (0x0B)              /* 0x0B: FAULT */
#define TAS6754_GPIO1_OUTPUT_CLOCK_SYNC                     (0x0E)              /* 0x0E: Clock sync out to secondary devices */
#define TAS6754_GPIO1_OUTPUT_INVALID_CLOCK                  (0x0F)              /* 0x0F: Invalid Clock */
#define TAS6754_GPIO1_OUTPUT_HIGH                           (0x13)              /* 0x13: HIGH */
#define TAS6754_GPIO1_OUTPUT_RESERVED_MASK                  (TAS6754_GPIO1_OUTPUT_RESERVED_7_6_MASK | \
                                                            TAS6754_GPIO1_OUTPUT_RESERVED_5_MASK)
/************************************************************************
* 					TAS6754_GPIO2_OUTPUT_SELECT (0x96)
************************************************************************/
#define TAS6754_GPIO2_OUTPUT_RESERVED_7_6_MASK              GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_GPIO2_OUTPUT_RESERVED_5_MASK                BIT(5)              /* Bit 5: Reserved */
#define TAS6754_GPIO2_OUTPUT_SELECT_MASK                    GENMASK(4, 0)       /* Bits 4-0: GPIO2 OUTPUT */
#define TAS6754_GPIO2_OUTPUT_LOW                            (0x00)              /* 0x00: LOW */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_ALL                  (0x02)              /* 0x02: Auto Mute All Channels */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH4                  (0x03)              /* 0x03: Auto Mute Channel 4 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH3                  (0x04)              /* 0x04: Auto Mute Channel 3 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH2                  (0x05)              /* 0x05: Auto Mute Channel 2 */
#define TAS6754_GPIO2_OUTPUT_AUTO_MUTE_CH1                  (0x06)              /* 0x06: Auto Mute Channel 1 */
#define TAS6754_GPIO2_OUTPUT_SDOUT2                         (0x08)              /* 0x08: SDOUT2 */
#define TAS6754_GPIO2_OUTPUT_SDOUT1                         (0x09)              /* 0x09: SDOUT1 */
#define TAS6754_GPIO2_OUTPUT_WARN                           (0x0A)              /* 0x0A: WARN */
#define TAS6754_GPIO2_OUTPUT_FAULT                          (0x0B)              /* 0x0B: FAULT */
#define TAS6754_GPIO2_OUTPUT_CLOCK_SYNC                     (0x0E)              /* 0x0E: Clock sync out to secondary devices */
#define TAS6754_GPIO2_OUTPUT_INVALID_CLOCK                  (0x0F)              /* 0x0F: Invalid Clock */
#define TAS6754_GPIO2_OUTPUT_HIGH                           (0x13)              /* 0x13: HIGH */
#define TAS6754_GPIO2_OUTPUT_RESERVED_MASK                  (TAS6754_GPIO2_OUTPUT_RESERVED_7_6_MASK | \
                                                            TAS6754_GPIO2_OUTPUT_RESERVED_5_MASK)
/************************************************************************
* 					TAS6754_GPIO_INPUT_SLEEP_HIZ (0x9B)
************************************************************************/
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK                  BIT(7)              /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_MASK                  GENMASK(6, 4)       /* Bits 6-4: GPIO INPUT FOR DEEP SLEEP */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK                  BIT(3)              /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_HIZ_MASK                         GENMASK(2, 0)       /* Bits 2-0: GPIO INPUT FOR HI Z */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_NA                    (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_GPIO1                 (0x10)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_DEEP_SLEEP_GPIO2                 (0x20)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_HIZ_NA                           (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_HIZ_GPIO1                        (0x01)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_HIZ_GPIO2                        (0x02)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_RESERVED_MASK                    (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_GPIO_INPUT_PLAY_SLEEP (0x9C)
************************************************************************/
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK                  BIT(7)              /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_PLAY_MASK                        GENMASK(6, 4)       /* Bits 6-4: GPIO INPUT PLAY */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK                  BIT(3)              /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_SLEEP_MASK                       GENMASK(2, 0)       /* Bits 2-0: GPIO INPUT SLEEP */
#define TAS6754_GPIO_INPUT_PLAY_NA                          (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_PLAY_GPIO1                       (0x10)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_PLAY_GPIO2                       (0x20)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_SLEEP_NA                         (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_SLEEP_GPIO1                      (0x01)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SLEEP_GPIO2                      (0x02)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_PS_RESERVED_MASK                 (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_GPIO_INPUT_MUTE (0x9D)
************************************************************************/
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK                  BIT(7)              /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_6_4_MASK                GENMASK(6, 4)       /* Bits 6-4: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK                  BIT(3)              /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_MUTE_MASK                        GENMASK(2, 0)       /* Bits 2-0: GPIO INPUT MUTE */
#define TAS6754_GPIO_INPUT_MUTE_NA                          (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_MUTE_GPIO1                       (0x01)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_MUTE_GPIO2                       (0x02)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_MUTE_RESERVED_MASK               (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_6_4_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_GPIO_INPUT_SYNC (0x9E)
************************************************************************/
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK                  BIT(7)              /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_6_4_MASK                GENMASK(6, 4)       /* Bits 6-4: Reserved */
#define TAS6754_GPIO_INPUT_RESERVED_3_MASK                  BIT(3)              /* Bit 3: Reserved */
#define TAS6754_GPIO_INPUT_SYNC_MASK                        GENMASK(2, 0)       /* Bits 2-0: GPIO INPUT SYNC */
#define TAS6754_GPIO_INPUT_SYNC_NA                          (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_SYNC_GPIO1                       (0x01)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SYNC_GPIO2                       (0x02)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_SYNC_RESERVED_MASK               (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_6_4_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_GPIO_INPUT_SDIN2 (0x9F)
************************************************************************/
#define TAS6754_GPIO_INPUT_RESERVED_7_MASK                  BIT(7)              /* Bit 7: Reserved */
#define TAS6754_GPIO_INPUT_SDIN2_MASK                       GENMASK(6, 4)       /* Bits 6-4: GPIO INPUT SDIN2 */
#define TAS6754_GPIO_INPUT_RESERVED_3_0_MASK                GENMASK(3, 0)       /* Bits 3-0: Reserved */
#define TAS6754_GPIO_INPUT_SDIN2_NA                         (0x00)              /* 000: N/A */
#define TAS6754_GPIO_INPUT_SDIN2_GPIO1                      (0x10)              /* 001: GPIO1 */
#define TAS6754_GPIO_INPUT_SDIN2_GPIO2                      (0x20)              /* 010: GPIO2 */
#define TAS6754_GPIO_INPUT_SDIN2_RESERVED_MASK              (TAS6754_GPIO_INPUT_RESERVED_7_MASK | \
                                                            TAS6754_GPIO_INPUT_RESERVED_3_0_MASK)
/************************************************************************
* 					TAS6754_GPIO_CTRL (0xA0)
************************************************************************/
#define TAS6754_GPIO_CTRL_GPIO1_IO_SELECT_MASK              BIT(7)              /* Bit 7: GPIO1 IO SELECT */
#define TAS6754_GPIO_CTRL_GPIO2_IO_SELECT_MASK              BIT(6)              /* Bit 6: GPIO2 IO SELECT */
#define TAS6754_GPIO_CTRL_RESERVED_5_MASK                   BIT(5)              /* Bit 5: Reserved */
#define TAS6754_GPIO_CTRL_RESERVED_4_MASK                   BIT(4)              /* Bit 4: Reserved */
#define TAS6754_GPIO_CTRL_GPO1_MODE_MASK                    BIT(3)              /* Bit 3: GPO1 MODE */
#define TAS6754_GPIO_CTRL_GPO2_MODE_MASK                    BIT(2)              /* Bit 2: GPO2 MODE */
#define TAS6754_GPIO_CTRL_RESERVED_1_MASK                   BIT(1)              /* Bit 1: Reserved */
#define TAS6754_GPIO_CTRL_RESERVED_0_MASK                   BIT(0)              /* Bit 0: Reserved */
#define TAS6754_GPIO_CTRL_GPIO1_AS_INPUT                    (0x00 << 7)         /* 0: Set GPIO1 as input */
#define TAS6754_GPIO_CTRL_GPIO1_AS_OUTPUT                   (0x01 << 7)         /* 1: Set GPIO1 as output */
#define TAS6754_GPIO_CTRL_GPIO2_AS_INPUT                    (0x00 << 6)         /* 0: Set GPIO2 as input */
#define TAS6754_GPIO_CTRL_GPIO2_AS_OUTPUT                   (0x01 << 6)         /* 1: Set GPIO2 as output */
#define TAS6754_GPIO_CTRL_GPO1_OUTPUT_BUFFER                (0x00 << 3)         /* 0: Output Buffer mode */
#define TAS6754_GPIO_CTRL_GPO1_OPEN_DRAIN                   (0x01 << 3)         /* 1: Open drain mode */
#define TAS6754_GPIO_CTRL_GPO2_OUTPUT_BUFFER                (0x00 << 2)         /* 0: Output Buffer mode */
#define TAS6754_GPIO_CTRL_GPO2_OPEN_DRAIN                   (0x01 << 2)         /* 1: Open drain mode */
#define TAS6754_GPIO_CTRL_RESERVED_MASK                     (TAS6754_GPIO_CTRL_RESERVED_5_MASK | \
                                                            TAS6754_GPIO_CTRL_RESERVED_4_MASK | \
                                                            TAS6754_GPIO_CTRL_RESERVED_1_MASK | \
                                                            TAS6754_GPIO_CTRL_RESERVED_0_MASK)
/************************************************************************
* 					TAS6754_GPIO_INVERT (0xA1)
************************************************************************/
#define TAS6754_GPIO_INVERT_GPO1_INV_MASK                   BIT(7)              /* Bit 7: GPO1 INV */
#define TAS6754_GPIO_INVERT_GPO2_INV_MASK                   BIT(6)              /* Bit 6: GPO2 INV */
#define TAS6754_GPIO_INVERT_RESERVED_5_MASK                 BIT(5)              /* Bit 5: Reserved */
#define TAS6754_GPIO_INVERT_RESERVED_4_1_MASK               GENMASK(4, 1)       /* Bits 4-1: Reserved */
#define TAS6754_GPIO_INVERT_GPO_PU_DISABLE_MASK             BIT(0)              /* Bit 0: GPO PU DISABLE */
#define TAS6754_GPIO_INVERT_GPO1_NON_INVERTED               (0x00 << 7)         /* 0: GPIO1 Output signal is non-inverted */
#define TAS6754_GPIO_INVERT_GPO1_INVERTED                   (0x01 << 7)         /* 1: GPIO1 Output signal is inverted */
#define TAS6754_GPIO_INVERT_GPO2_NON_INVERTED               (0x00 << 6)         /* 0: GPIO2 Output signal is non-inverted */
#define TAS6754_GPIO_INVERT_GPO2_INVERTED                   (0x01 << 6)         /* 1: GPIO2 Output signal is inverted */
#define TAS6754_GPIO_INVERT_GPO_PU_ENABLE                   (0x00 << 0)         /* 0: Enable internal 110kΩ pull-up of GP outputs in open drain */
#define TAS6754_GPIO_INVERT_GPO_PU_DISABLE                  (0x01 << 0)         /* 1: Disable internal 110kΩ pull-up of GP outputs in open drain */
#define TAS6754_GPIO_INVERT_RESERVED_MASK                   (TAS6754_GPIO_INVERT_RESERVED_5_MASK | \
                                                            TAS6754_GPIO_INVERT_RESERVED_4_1_MASK)
/************************************************************************
* 					TAS6754_DC_LDG_CTRL (0xB0)
************************************************************************/
#define TAS6754_DC_LDG_ABORT_MASK                           BIT(7)              /* Bit 7: LDG ABORT */
#define TAS6754_DC_LDG_BUFFER_WAIT_TIME_MASK                GENMASK(6, 5)       /* Bits 6-5: LDG BUFFER WAIT TIME */
#define TAS6754_DC_LDG_RESERVED_MASK                        GENMASK(4, 3)       /* Bits 4-3: Reserved */
#define TAS6754_DC_LDG_WAIT_BYPASS_MASK                     BIT(2)              /* Bit 2: LDG WAIT BYPASS */
#define TAS6754_DC_LDG_SLOL_DISABLE_MASK                    BIT(1)              /* Bit 1: LDG SLOL DISABLE */
#define TAS6754_DC_LDG_BYPASS_MASK                          BIT(0)              /* Bit 0: LDG BYPASS */
#define TAS6754_DC_LDG_ABORT_NORMAL                         (0x00 << 7)         /* 0: Normal operation */
#define TAS6754_DC_LDG_ABORT_ABORT                          (0x01 << 7)         /* 1: Abort DC load diagnostic */
#define TAS6754_DC_LDG_BUFFER_WAIT_1MS                      (0x00 << 5)         /* 00: Buffer wait time 1ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_2MS                      (0x01 << 5)         /* 01: Buffer wait time 2ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_5MS                      (0x02 << 5)         /* 10: Buffer wait time 5ms */
#define TAS6754_DC_LDG_BUFFER_WAIT_10MS                     (0x03 << 5)         /* 11: Buffer wait time 10ms */
#define TAS6754_DC_LDG_WAIT_BYPASS_DISABLE                  (0x00 << 2)         /* 0: Enable the waiting loop at the end of shorted/open load detection */
#define TAS6754_DC_LDG_WAIT_BYPASS_ENABLE                   (0x01 << 2)         /* 1: Bypass the waiting loop at the end of shorted/open load detection */
#define TAS6754_DC_LDG_SLOL_ENABLE                          (0x00 << 1)         /* 0: Shorted load and open load detection are enabled */
#define TAS6754_DC_LDG_SLOL_DISABLE                         (0x01 << 1)         /* 1: Shorted load, open load and line out detection are disabled */
#define TAS6754_DC_LDG_BYPASS_DISABLE                       (0x00 << 0)         /* 0: Automatic DC diagnostic after a channel fault occurs in Hi-Z or PLAY state */
#define TAS6754_DC_LDG_BYPASS_ENABLE                        (0x01 << 0)         /* 1: DC diagnostic will not run automatically */
/************************************************************************
* 					TAS6754_DC_LDG_LO_CTRL (0xB1)
************************************************************************/
#define TAS6754_DC_LDG_LO_RESERVED_7_MASK                   BIT(7)              /* Bit 7: Reserved */
#define TAS6754_DC_LDG_LO_RESERVED_6_MASK                   BIT(6)              /* Bit 6: Reserved */
#define TAS6754_DC_LDG_LO_RESERVED_5_4_MASK                 GENMASK(5, 4)       /* Bits 5-4: Reserved */
#define TAS6754_DC_LDG_LO_CH1_ENABLE_MASK                   BIT(3)              /* Bit 3: CH1 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH2_ENABLE_MASK                   BIT(2)              /* Bit 2: CH2 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH3_ENABLE_MASK                   BIT(1)              /* Bit 1: CH3 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH4_ENABLE_MASK                   BIT(0)              /* Bit 0: CH4 LO LDG ENABLE */
#define TAS6754_DC_LDG_LO_CH1_DISABLE                       (0x00 << 3)         /* 0: Disable DC Load Diagnostics for line-out load on Channel 1 */
#define TAS6754_DC_LDG_LO_CH1_ENABLE                        (0x01 << 3)         /* 1: Enable DC Load Diagnostics for line-out load on Channel 1 */
#define TAS6754_DC_LDG_LO_CH2_DISABLE                       (0x00 << 2)         /* 0: Disable DC Load Diagnostics for line-out load on Channel 2 */
#define TAS6754_DC_LDG_LO_CH2_ENABLE                        (0x01 << 2)         /* 1: Enable DC Load Diagnostics for line-out load on Channel 2 */
#define TAS6754_DC_LDG_LO_CH3_DISABLE                       (0x00 << 1)         /* 0: Disable DC Load Diagnostics for line-out load on Channel 3 */
#define TAS6754_DC_LDG_LO_CH3_ENABLE                        (0x01 << 1)         /* 1: Enable DC Load Diagnostics for line-out load on Channel 3 */
#define TAS6754_DC_LDG_LO_CH4_DISABLE                       (0x00 << 0)         /* 0: Disable DC Load Diagnostics for line-out load on Channel 4 */
#define TAS6754_DC_LDG_LO_CH4_ENABLE                        (0x01 << 0)         /* 1: Enable DC Load Diagnostics for line-out load on Channel 4 */
#define TAS6754_DC_LDG_LO_ALL_CH_MASK                       (TAS6754_DC_LDG_LO_CH1_ENABLE_MASK | \
                                                            TAS6754_DC_LDG_LO_CH2_ENABLE_MASK | \
                                                            TAS6754_DC_LDG_LO_CH3_ENABLE_MASK | \
                                                            TAS6754_DC_LDG_LO_CH4_ENABLE_MASK)
#define TAS6754_DC_LDG_LO_RESERVED_MASK                     (TAS6754_DC_LDG_LO_RESERVED_7_MASK | \
                                                            TAS6754_DC_LDG_LO_RESERVED_6_MASK | \
                                                            TAS6754_DC_LDG_LO_RESERVED_5_4_MASK)
/************************************************************************
* 					TAS6754_DC_LDG_TIME_CTRL (0xB2)
************************************************************************/
#define TAS6754_DC_LDG_RAMP_SL_OL_MASK                      GENMASK(7, 6)       /* Bits 7-6: LDG RAMP SL OL */
#define TAS6754_DC_LDG_SETTLING_SL_OL_MASK                  GENMASK(5, 4)       /* Bits 5-4: LDG SETTLING SL OL */
#define TAS6754_DC_LDG_RAMP_S2PG_MASK                       GENMASK(3, 2)       /* Bits 3-2: LDG RAMP S2PG */
#define TAS6754_DC_LDG_SETTLING_S2PG_MASK                   GENMASK(1, 0)       /* Bits 1-0: LDG SETTLING S2PG */
#define TAS6754_DC_LDG_RAMP_SL_OL_15MS                      (0x00 << 6)         /* 00: 15 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_30MS                      (0x01 << 6)         /* 01: 30 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_10MS                      (0x02 << 6)         /* 10: 10 ms */
#define TAS6754_DC_LDG_RAMP_SL_OL_20MS                      (0x03 << 6)         /* 11: 20 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_10MS                  (0x00 << 4)         /* 00: 10 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_5MS                   (0x01 << 4)         /* 01: 5 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_20MS                  (0x02 << 4)         /* 10: 20 ms */
#define TAS6754_DC_LDG_SETTLING_SL_OL_15MS                  (0x03 << 4)         /* 11: 15 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_5MS                        (0x00 << 2)         /* 00: 5 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_2_5MS                      (0x01 << 2)         /* 01: 2.5 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_10MS                       (0x02 << 2)         /* 10: 10 ms */
#define TAS6754_DC_LDG_RAMP_S2PG_15MS                       (0x03 << 2)         /* 11: 15 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_10MS                   (0x00 << 0)         /* 00: 10 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_5MS                    (0x01 << 0)         /* 01: 5 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_20MS                   (0x02 << 0)         /* 10: 20 ms */
#define TAS6754_DC_LDG_SETTLING_S2PG_30MS                   (0x03 << 0)         /* 11: 30 ms */
/************************************************************************
* 					TAS6754_DC_LDG_SL_CH1_CH2_CTRL (0xB3)
************************************************************************/
#define TAS6754_DC_LDG_SL_CH1_MASK                          GENMASK(7, 4)       /* Bits 7-4: CH1 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH2_MASK                          GENMASK(3, 0)       /* Bits 3-0: CH2 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH1_0_5_OHM                       (0x00 << 4)         /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH1_1_0_OHM                       (0x01 << 4)         /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH1_1_5_OHM                       (0x02 << 4)         /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH1_2_0_OHM                       (0x03 << 4)         /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH1_2_5_OHM                       (0x04 << 4)         /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH1_3_0_OHM                       (0x05 << 4)         /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH1_3_5_OHM                       (0x06 << 4)         /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH1_4_0_OHM                       (0x07 << 4)         /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH1_4_5_OHM                       (0x08 << 4)         /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH1_5_0_OHM                       (0x09 << 4)         /* 1001: 5.0Ω */
#define TAS6754_DC_LDG_SL_CH2_0_5_OHM                       (0x00 << 0)         /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH2_1_0_OHM                       (0x01 << 0)         /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH2_1_5_OHM                       (0x02 << 0)         /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH2_2_0_OHM                       (0x03 << 0)         /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH2_2_5_OHM                       (0x04 << 0)         /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH2_3_0_OHM                       (0x05 << 0)         /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH2_3_5_OHM                       (0x06 << 0)         /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH2_4_0_OHM                       (0x07 << 0)         /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH2_4_5_OHM                       (0x08 << 0)         /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH2_5_0_OHM                       (0x09 << 0)         /* 1001: 5.0Ω */
#define TAS6754_DC_LDG_SL_CH1_SET(val)                      ((val & 0x0F) << 4)
#define TAS6754_DC_LDG_SL_CH2_SET(val)                      (val & 0x0F)
/************************************************************************
* 					TAS6754_DC_LDG_SL_CH3_CH4_CTRL (0xB4)
************************************************************************/
#define TAS6754_DC_LDG_SL_CH3_MASK                          GENMASK(7, 4)       /* Bits 7-4: CH3 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH4_MASK                          GENMASK(3, 0)       /* Bits 3-0: CH4 DC LDG SL */
#define TAS6754_DC_LDG_SL_CH3_0_5_OHM                       (0x00 << 4)         /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH3_1_0_OHM                       (0x01 << 4)         /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH3_1_5_OHM                       (0x02 << 4)         /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH3_2_0_OHM                       (0x03 << 4)         /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH3_2_5_OHM                       (0x04 << 4)         /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH3_3_0_OHM                       (0x05 << 4)         /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH3_3_5_OHM                       (0x06 << 4)         /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH3_4_0_OHM                       (0x07 << 4)         /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH3_4_5_OHM                       (0x08 << 4)         /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH3_5_0_OHM                       (0x09 << 4)         /* 1001: 5.0Ω */
#define TAS6754_DC_LDG_SL_CH4_0_5_OHM                       (0x00 << 0)         /* 0000: 0.5Ω */
#define TAS6754_DC_LDG_SL_CH4_1_0_OHM                       (0x01 << 0)         /* 0001: 1.0Ω */
#define TAS6754_DC_LDG_SL_CH4_1_5_OHM                       (0x02 << 0)         /* 0010: 1.5Ω */
#define TAS6754_DC_LDG_SL_CH4_2_0_OHM                       (0x03 << 0)         /* 0011: 2.0Ω */
#define TAS6754_DC_LDG_SL_CH4_2_5_OHM                       (0x04 << 0)         /* 0100: 2.5Ω */
#define TAS6754_DC_LDG_SL_CH4_3_0_OHM                       (0x05 << 0)         /* 0101: 3.0Ω */
#define TAS6754_DC_LDG_SL_CH4_3_5_OHM                       (0x06 << 0)         /* 0110: 3.5Ω */
#define TAS6754_DC_LDG_SL_CH4_4_0_OHM                       (0x07 << 0)         /* 0111: 4.0Ω */
#define TAS6754_DC_LDG_SL_CH4_4_5_OHM                       (0x08 << 0)         /* 1000: 4.5Ω */
#define TAS6754_DC_LDG_SL_CH4_5_0_OHM                       (0x09 << 0)         /* 1001: 5.0Ω */
#define TAS6754_DC_LDG_SL_CH3_SET(val)                      ((val & 0x0F) << 4)
#define TAS6754_DC_LDG_SL_CH4_SET(val)                      (val & 0x0F)
/************************************************************************
* 					TAS6754_AC_LDG_CTRL (0xB5)
************************************************************************/
#define TAS6754_AC_LDG_RESERVED_MASK                        GENMASK(7, 5)       /* Bits 7-5: Reserved */
#define TAS6754_AC_LDG_DIAG_GAIN_MASK                       BIT(4)              /* Bit 4: AC DIAG GAIN */
#define TAS6754_AC_LDG_CH1_DIAG_START_MASK                  BIT(3)              /* Bit 3: CH1 AC DIAG START */
#define TAS6754_AC_LDG_CH2_DIAG_START_MASK                  BIT(2)              /* Bit 2: CH2 AC DIAG START */
#define TAS6754_AC_LDG_CH3_DIAG_START_MASK                  BIT(1)              /* Bit 1: CH3 AC DIAG START */
#define TAS6754_AC_LDG_CH4_DIAG_START_MASK                  BIT(0)              /* Bit 0: CH4 AC DIAG START */
#define TAS6754_AC_LDG_DIAG_GAIN_1                          (0x00 << 4)         /* 0: Gain 1 */
#define TAS6754_AC_LDG_DIAG_GAIN_8                          (0x01 << 4)         /* 1: Gain 8 */
#define TAS6754_AC_LDG_CH1_NORMAL                           (0x00 << 3)         /* 0: Normal operation */
#define TAS6754_AC_LDG_CH1_START                            (0x01 << 3)         /* 1: Start AC diagnostic on Channel 1 */
#define TAS6754_AC_LDG_CH2_NORMAL                           (0x00 << 2)         /* 0: Normal operation */
#define TAS6754_AC_LDG_CH2_START                            (0x01 << 2)         /* 1: Start AC diagnostic on Channel 2 */
#define TAS6754_AC_LDG_CH3_NORMAL                           (0x00 << 1)         /* 0: Normal operation */
#define TAS6754_AC_LDG_CH3_START                            (0x01 << 1)         /* 1: Start AC diagnostic on Channel 3 */
#define TAS6754_AC_LDG_CH4_NORMAL                           (0x00 << 0)         /* 0: Normal operation */
#define TAS6754_AC_LDG_CH4_START                            (0x01 << 0)         /* 1: Start AC diagnostic on Channel 4 */
#define TAS6754_AC_LDG_ALL_CH_START_MASK                    (TAS6754_AC_LDG_CH1_DIAG_START_MASK | \
                                                            TAS6754_AC_LDG_CH2_DIAG_START_MASK | \
                                                            TAS6754_AC_LDG_CH3_DIAG_START_MASK | \
                                                            TAS6754_AC_LDG_CH4_DIAG_START_MASK)
/************************************************************************
* 					TAS6754_TWEETER_DETECT_CTRL (0xB6)
************************************************************************/
#define TAS6754_TWEETER_DETECT_RESERVED_7_4_MASK            GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_TWEETER_DETECT_AVG_MASK                     BIT(3)              /* Bit 3: TWEETER DETECT AVG */
#define TAS6754_TWEETER_DETECT_RESERVED_2_MASK              BIT(2)              /* Bit 2: Reserved */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_MASK               BIT(1)              /* Bit 1: TWEETER DETECT CALC TYPE */
#define TAS6754_TWEETER_DETECT_DISABLE_MASK                 BIT(0)              /* Bit 0: TWEETER DETECT DISABLE */
#define TAS6754_TWEETER_DETECT_AVG_4096                     (0x00 << 3)         /* 0: 4096-sample averaging in decimation filter */
#define TAS6754_TWEETER_DETECT_AVG_32768                    (0x01 << 3)         /* 1: 32768-sample averaging in decimation filter */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_2                  (0x00 << 1)         /* 0: AC pass/fail judgement type 2 - Calculate magnitude as Re(Z)+0.5*Im(Z) */
#define TAS6754_TWEETER_DETECT_CALC_TYPE_1                  (0x01 << 1)         /* 1: AC pass/fail judgement type 1 - Calculate magnitude as Re(Z) */
#define TAS6754_TWEETER_DETECT_ENABLE                       (0x00 << 0)         /* 0: Enable Tweeter detection judgement */
#define TAS6754_TWEETER_DETECT_DISABLE                      (0x01 << 0)         /* 1: Disable Tweeter detection calculation */
#define TAS6754_TWEETER_DETECT_RESERVED_MASK                (TAS6754_TWEETER_DETECT_RESERVED_7_4_MASK | \
                                                            TAS6754_TWEETER_DETECT_RESERVED_2_MASK)
/************************************************************************
* 					TAS6754_TWEETER_DETECT_THRESH (0xB7)
************************************************************************/
#define TAS6754_TWEETER_DETECT_THRESH_MASK                  (0xFF)                      /* Bits 7-0: TWEETER DETECT THRESHOLD */
#define TAS6754_TWEETER_DETECT_THRESH_GAIN_1(ohms)          ((uint8_t)((ohms) / 0.8))   /* Convert ohms to register value when AC DIAG GAIN = 0 */
#define TAS6754_TWEETER_DETECT_THRESH_GAIN_8(ohms)          ((uint8_t)((ohms) / 0.1))   /* Convert ohms to register value when AC DIAG GAIN = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_0_8_OHM_G1            (0x01)                      /* 0.8Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_1_6_OHM_G1            (0x02)                      /* 1.6Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_4_0_OHM_G1            (0x05)                      /* 4.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_8_0_OHM_G1            (0x0A)                      /* 8.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_16_0_OHM_G1           (0x14)                      /* 16.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_40_0_OHM_G1           (0x32)                      /* 40.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_80_0_OHM_G1           (0x64)                      /* 80.0Ω with Gain = 1 */
#define TAS6754_TWEETER_DETECT_THRESH_0_1_OHM_G8            (0x01)                      /* 0.1Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_0_5_OHM_G8            (0x05)                      /* 0.5Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_1_0_OHM_G8            (0x0A)                      /* 1.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_2_0_OHM_G8            (0x14)                      /* 2.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_5_0_OHM_G8            (0x32)                      /* 5.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_10_0_OHM_G8           (0x64)                      /* 10.0Ω with Gain = 8 */
#define TAS6754_TWEETER_DETECT_THRESH_20_0_OHM_G8           (0xC8)                      /* 20.0Ω with Gain = 8 */
/************************************************************************
* 					TAS6754_AC_LDG_FREQ_CTRL (0xB8)
************************************************************************/
#define TAS6754_AC_LDG_STIMULUS_FREQ_MASK                   (0xFF)                      /* Bits 7-0: AC LDG STIMULUS FREQUENCY */
#define TAS6754_AC_LDG_FREQ_HZ(freq)                        ((uint8_t)((freq) / 93.75)) /* Convert frequency in Hz to register value */
#define TAS6754_AC_LDG_FREQ_93_75_HZ                        (0x01)                      /* 93.75 Hz */
#define TAS6754_AC_LDG_FREQ_187_5_HZ                        (0x02)                      /* 187.5 Hz */
#define TAS6754_AC_LDG_FREQ_281_25_HZ                       (0x03)                      /* 281.25 Hz */
#define TAS6754_AC_LDG_FREQ_500_HZ                          (0x05)                      /* ~500 Hz (468.75 Hz) */
#define TAS6754_AC_LDG_FREQ_1_KHZ                           (0x0B)                      /* ~1 kHz (1031.25 Hz) */
#define TAS6754_AC_LDG_FREQ_2_KHZ                           (0x15)                      /* ~2 kHz (1968.75 Hz) */
#define TAS6754_AC_LDG_FREQ_5_KHZ                           (0x35)                      /* ~5 kHz (4921.88 Hz) */
#define TAS6754_AC_LDG_FREQ_10_KHZ                          (0x6A)                      /* ~10 kHz (9843.75 Hz) */
#define TAS6754_AC_LDG_FREQ_18_75_KHZ                       (0xC8)                      /* 18.75 kHz */
#define TAS6754_AC_LDG_FREQ_20_KHZ                          (0xD5)                      /* ~20 kHz (19687.5 Hz) */
#define TAS6754_AC_LDG_FREQ_23_91_KHZ                       (0xFF)                      /* 23.91 kHz */
/************************************************************************
* 					TAS6754_TEMP_CH1_CH2 (0xBB)
************************************************************************/
#define TAS6754_TEMP_CH1_CH2_RESERVED_MASK                  GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_TEMP_CH2_MASK                               GENMASK(5, 3)       /* Bits 5-3: CH2 TEMP */
#define TAS6754_TEMP_CH1_MASK                               GENMASK(2, 0)       /* Bits 2-0: CH1 TEMP */
#define TAS6754_TEMP_CH2_BELOW_95C                          (0x00 << 3)         /* 000: < 95°C */
#define TAS6754_TEMP_CH2_95C_110C                           (0x01 << 3)         /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH2_110C_125C                          (0x02 << 3)         /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH2_125C_135C                          (0x03 << 3)         /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH2_135C_145C                          (0x04 << 3)         /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH2_145C_155C                          (0x05 << 3)         /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH2_155C_165C                          (0x06 << 3)         /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH2_ABOVE_165C                         (0x07 << 3)         /* 111: > 165°C */
#define TAS6754_TEMP_CH1_BELOW_95C                          (0x00 << 0)         /* 000: < 95°C */
#define TAS6754_TEMP_CH1_95C_110C                           (0x01 << 0)         /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH1_110C_125C                          (0x02 << 0)         /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH1_125C_135C                          (0x03 << 0)         /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH1_135C_145C                          (0x04 << 0)         /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH1_145C_155C                          (0x05 << 0)         /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH1_155C_165C                          (0x06 << 0)         /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH1_ABOVE_165C                         (0x07 << 0)         /* 111: > 165°C */
#define TAS6754_GET_TEMP_CH2(reg_val)                       ((reg_val & TAS6754_TEMP_CH2_MASK) >> 3)
#define TAS6754_GET_TEMP_CH1(reg_val)                       (reg_val & TAS6754_TEMP_CH1_MASK)
/************************************************************************
* 					TAS6754_TEMP_CH3_CH4 (0xBC)
************************************************************************/
#define TAS6754_TEMP_CH3_CH4_RESERVED_MASK                  GENMASK(7, 6)       /* Bits 7-6: Reserved */
#define TAS6754_TEMP_CH4_MASK                               GENMASK(5, 3)       /* Bits 5-3: CH4 TEMP */
#define TAS6754_TEMP_CH3_MASK                               GENMASK(2, 0)       /* Bits 2-0: CH3 TEMP */
#define TAS6754_TEMP_CH4_BELOW_95C                          (0x00 << 3)         /* 000: < 95°C */
#define TAS6754_TEMP_CH4_95C_110C                           (0x01 << 3)         /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH4_110C_125C                          (0x02 << 3)         /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH4_125C_135C                          (0x03 << 3)         /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH4_135C_145C                          (0x04 << 3)         /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH4_145C_155C                          (0x05 << 3)         /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH4_155C_165C                          (0x06 << 3)         /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH4_ABOVE_165C                         (0x07 << 3)         /* 111: > 165°C */
#define TAS6754_TEMP_CH3_BELOW_95C                          (0x00 << 0)         /* 000: < 95°C */
#define TAS6754_TEMP_CH3_95C_110C                           (0x01 << 0)         /* 001: 95°C~110°C */
#define TAS6754_TEMP_CH3_110C_125C                          (0x02 << 0)         /* 010: 110°C~125°C */
#define TAS6754_TEMP_CH3_125C_135C                          (0x03 << 0)         /* 011: 125°C~135°C */
#define TAS6754_TEMP_CH3_135C_145C                          (0x04 << 0)         /* 100: 135°C~145°C */
#define TAS6754_TEMP_CH3_145C_155C                          (0x05 << 0)         /* 101: 145°C~155°C */
#define TAS6754_TEMP_CH3_155C_165C                          (0x06 << 0)         /* 110: 155°C~165°C */
#define TAS6754_TEMP_CH3_ABOVE_165C                         (0x07 << 0)         /* 111: > 165°C */
#define TAS6754_GET_TEMP_CH4(reg_val)                       ((reg_val & TAS6754_TEMP_CH4_MASK) >> 3)
#define TAS6754_GET_TEMP_CH3(reg_val)                       (reg_val & TAS6754_TEMP_CH3_MASK)
/************************************************************************
* 					TAS6754_WARN_OT_MAX_FLAG (0xBD)
************************************************************************/
#define TAS6754_WARN_OT_MAX_RESERVED_MASK                   GENMASK(7, 3)       /* Bits 7-3: Reserved */
#define TAS6754_WARN_OT_MAX_ALL_CH_TEMP_MASK                GENMASK(2, 0)       /* Bits 2-0: ALL CHANNEL MAX TEMP */
#define TAS6754_WARN_OT_MAX_BELOW_95C                       (0x00)              /* 000: < 95°C */
#define TAS6754_WARN_OT_MAX_95C_110C                        (0x01)              /* 001: 95°C~110°C */
#define TAS6754_WARN_OT_MAX_110C_125C                       (0x02)              /* 010: 110°C~125°C */
#define TAS6754_WARN_OT_MAX_125C_135C                       (0x03)              /* 011: 125°C~135°C */
#define TAS6754_WARN_OT_MAX_135C_145C                       (0x04)              /* 100: 135°C~145°C */
#define TAS6754_WARN_OT_MAX_145C_155C                       (0x05)              /* 101: 145°C~155°C */
#define TAS6754_WARN_OT_MAX_155C_165C                       (0x06)              /* 110: 155°C~165°C */
#define TAS6754_WARN_OT_MAX_ABOVE_165C                      (0x07)              /* 111: > 165°C */
/************************************************************************
* 					TAS6754_DC_LDG_REPORT_CH1_CH2 (0xC0)
************************************************************************/
#define TAS6754_DC_LDG_CH1_S2G_MASK                         BIT(7)              /* Bit 7: CH1 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH1_S2P_MASK                         BIT(6)              /* Bit 6: CH1 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH1_OL_MASK                          BIT(5)              /* Bit 5: CH1 OL (Open Load) */
#define TAS6754_DC_LDG_CH1_SL_MASK                          BIT(4)              /* Bit 4: CH1 SL (Shorted Load) */
#define TAS6754_DC_LDG_CH2_S2G_MASK                         BIT(3)              /* Bit 3: CH2 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH2_S2P_MASK                         BIT(2)              /* Bit 2: CH2 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH2_OL_MASK                          BIT(1)              /* Bit 1: CH2 OL (Open Load) */
#define TAS6754_DC_LDG_CH2_SL_MASK                          BIT(0)              /* Bit 0: CH2 SL (Shorted Load) */
#define TAS6754_DC_LDG_CH1_S2G_NOT_DETECTED                 (0x00 << 7)         /* 0: No short-to-GND detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2G_DETECTED                     (0x01 << 7)         /* 1: Short-to-GND detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2P_NOT_DETECTED                 (0x00 << 6)         /* 0: No short-to-power detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_S2P_DETECTED                     (0x01 << 6)         /* 1: Short-to-power detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_OL_NOT_DETECTED                  (0x00 << 5)         /* 0: No open load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_OL_DETECTED                      (0x01 << 5)         /* 1: Open load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_SL_NOT_DETECTED                  (0x00 << 4)         /* 0: No shorted load detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_SL_DETECTED                      (0x01 << 4)         /* 1: Shorted load detected on Channel 1 */
#define TAS6754_DC_LDG_CH2_S2G_NOT_DETECTED                 (0x00 << 3)         /* 0: No short-to-GND detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2G_DETECTED                     (0x01 << 3)         /* 1: Short-to-GND detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2P_NOT_DETECTED                 (0x00 << 2)         /* 0: No short-to-power detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_S2P_DETECTED                     (0x01 << 2)         /* 1: Short-to-power detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_OL_NOT_DETECTED                  (0x00 << 1)         /* 0: No open load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_OL_DETECTED                      (0x01 << 1)         /* 1: Open load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_SL_NOT_DETECTED                  (0x00 << 0)         /* 0: No shorted load detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_SL_DETECTED                      (0x01 << 0)         /* 1: Shorted load detected on Channel 2 */
#define TAS6754_DC_LDG_CH1_ALL_MASK                         (TAS6754_DC_LDG_CH1_S2G_MASK | \
                                                            TAS6754_DC_LDG_CH1_S2P_MASK | \
                                                            TAS6754_DC_LDG_CH1_OL_MASK | \
                                                            TAS6754_DC_LDG_CH1_SL_MASK)
#define TAS6754_DC_LDG_CH2_ALL_MASK                         (TAS6754_DC_LDG_CH2_S2G_MASK | \
                                                            TAS6754_DC_LDG_CH2_S2P_MASK | \
                                                            TAS6754_DC_LDG_CH2_OL_MASK | \
                                                            TAS6754_DC_LDG_CH2_SL_MASK)
/************************************************************************
* 					TAS6754_DC_LDG_REPORT_CH3_CH4 (0xC1)
************************************************************************/
#define TAS6754_DC_LDG_CH3_S2G_MASK                         BIT(7)              /* Bit 7: CH3 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH3_S2P_MASK                         BIT(6)              /* Bit 6: CH3 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH3_OL_MASK                          BIT(5)              /* Bit 5: CH3 OL (Open Load) */
#define TAS6754_DC_LDG_CH3_SL_MASK                          BIT(4)              /* Bit 4: CH3 SL (Shorted Load) */
#define TAS6754_DC_LDG_CH4_S2G_MASK                         BIT(3)              /* Bit 3: CH4 S2G (Short-to-GND) */
#define TAS6754_DC_LDG_CH4_S2P_MASK                         BIT(2)              /* Bit 2: CH4 S2P (Short-to-Power) */
#define TAS6754_DC_LDG_CH4_OL_MASK                          BIT(1)              /* Bit 1: CH4 OL (Open Load) */
#define TAS6754_DC_LDG_CH4_SL_MASK                          BIT(0)              /* Bit 0: CH4 SL (Shorted Load) */
#define TAS6754_DC_LDG_CH3_S2G_NOT_DETECTED                 (0x00 << 7)         /* 0: No short-to-GND detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2G_DETECTED                     (0x01 << 7)         /* 1: Short-to-GND detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2P_NOT_DETECTED                 (0x00 << 6)         /* 0: No short-to-power detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_S2P_DETECTED                     (0x01 << 6)         /* 1: Short-to-power detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_OL_NOT_DETECTED                  (0x00 << 5)         /* 0: No open load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_OL_DETECTED                      (0x01 << 5)         /* 1: Open load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_SL_NOT_DETECTED                  (0x00 << 4)         /* 0: No shorted load detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_SL_DETECTED                      (0x01 << 4)         /* 1: Shorted load detected on Channel 3 */
#define TAS6754_DC_LDG_CH4_S2G_NOT_DETECTED                 (0x00 << 3)         /* 0: No short-to-GND detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2G_DETECTED                     (0x01 << 3)         /* 1: Short-to-GND detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2P_NOT_DETECTED                 (0x00 << 2)         /* 0: No short-to-power detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_S2P_DETECTED                     (0x01 << 2)         /* 1: Short-to-power detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_OL_NOT_DETECTED                  (0x00 << 1)         /* 0: No open load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_OL_DETECTED                      (0x01 << 1)         /* 1: Open load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_SL_NOT_DETECTED                  (0x00 << 0)         /* 0: No shorted load detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_SL_DETECTED                      (0x01 << 0)         /* 1: Shorted load detected on Channel 4 */
#define TAS6754_DC_LDG_CH3_ALL_MASK                         (TAS6754_DC_LDG_CH3_S2G_MASK | \
                                                            TAS6754_DC_LDG_CH3_S2P_MASK | \
                                                            TAS6754_DC_LDG_CH3_OL_MASK | \
                                                            TAS6754_DC_LDG_CH3_SL_MASK)
#define TAS6754_DC_LDG_CH4_ALL_MASK                         (TAS6754_DC_LDG_CH4_S2G_MASK | \
                                                            TAS6754_DC_LDG_CH4_S2P_MASK | \
                                                            TAS6754_DC_LDG_CH4_OL_MASK | \
                                                            TAS6754_DC_LDG_CH4_SL_MASK)
/************************************************************************
* 					TAS6754_DC_LDG_RESULT (0xC2)
************************************************************************/
#define TAS6754_DC_LDG_CH1_LO_RESULT_MASK                   BIT(7)              /* Bit 7: CH1 LO LDG RESULT */
#define TAS6754_DC_LDG_CH2_LO_RESULT_MASK                   BIT(6)              /* Bit 6: CH2 LO LDG RESULT */
#define TAS6754_DC_LDG_CH3_LO_RESULT_MASK                   BIT(5)              /* Bit 5: CH3 LO LDG RESULT */
#define TAS6754_DC_LDG_CH4_LO_RESULT_MASK                   BIT(4)              /* Bit 4: CH4 LO LDG RESULT */
#define TAS6754_DC_LDG_CH1_RESULT_MASK                      BIT(3)              /* Bit 3: CH1 DC LDG RESULT */
#define TAS6754_DC_LDG_CH2_RESULT_MASK                      BIT(2)              /* Bit 2: CH2 DC LDG RESULT */
#define TAS6754_DC_LDG_CH3_RESULT_MASK                      BIT(1)              /* Bit 1: CH3 DC LDG RESULT */
#define TAS6754_DC_LDG_CH4_RESULT_MASK                      BIT(0)              /* Bit 0: CH4 DC LDG RESULT */
#define TAS6754_DC_LDG_CH1_LO_NOT_DETECTED                  (0x00 << 7)         /* 0: Lineout load not detected on Channel 1 */
#define TAS6754_DC_LDG_CH1_LO_DETECTED                      (0x01 << 7)         /* 1: Lineout load detected on Channel 1 */
#define TAS6754_DC_LDG_CH2_LO_NOT_DETECTED                  (0x00 << 6)         /* 0: Lineout load not detected on Channel 2 */
#define TAS6754_DC_LDG_CH2_LO_DETECTED                      (0x01 << 6)         /* 1: Lineout load detected on Channel 2 */
#define TAS6754_DC_LDG_CH3_LO_NOT_DETECTED                  (0x00 << 5)         /* 0: Lineout load not detected on Channel 3 */
#define TAS6754_DC_LDG_CH3_LO_DETECTED                      (0x01 << 5)         /* 1: Lineout load detected on Channel 3 */
#define TAS6754_DC_LDG_CH4_LO_NOT_DETECTED                  (0x00 << 4)         /* 0: Lineout load not detected on Channel 4 */
#define TAS6754_DC_LDG_CH4_LO_DETECTED                      (0x01 << 4)         /* 1: Lineout load detected on Channel 4 */
#define TAS6754_DC_LDG_CH1_FAILED                           (0x00 << 3)         /* 0: DC Load Diagnostic did not complete without faults on Channel 1 */
#define TAS6754_DC_LDG_CH1_PASSED                           (0x01 << 3)         /* 1: DC Load Diagnostic completed without faults on Channel 1 */
#define TAS6754_DC_LDG_CH2_FAILED                           (0x00 << 2)         /* 0: DC Load Diagnostic did not complete without faults on Channel 2 */
#define TAS6754_DC_LDG_CH2_PASSED                           (0x01 << 2)         /* 1: DC Load Diagnostic completed without faults on Channel 2 */
#define TAS6754_DC_LDG_CH3_FAILED                           (0x00 << 1)         /* 0: DC Load Diagnostic did not complete without faults on Channel 3 */
#define TAS6754_DC_LDG_CH3_PASSED                           (0x01 << 1)         /* 1: DC Load Diagnostic completed without faults on Channel 3 */
#define TAS6754_DC_LDG_CH4_FAILED                           (0x00 << 0)         /* 0: DC Load Diagnostic did not complete without faults on Channel 4 */
#define TAS6754_DC_LDG_CH4_PASSED                           (0x01 << 0)         /* 1: DC Load Diagnostic completed without faults on Channel 4 */
#define TAS6754_DC_LDG_ALL_LO_RESULT_MASK                   (TAS6754_DC_LDG_CH1_LO_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH2_LO_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH3_LO_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH4_LO_RESULT_MASK)
#define TAS6754_DC_LDG_ALL_RESULT_MASK                      (TAS6754_DC_LDG_CH1_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH2_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH3_RESULT_MASK | \
                                                            TAS6754_DC_LDG_CH4_RESULT_MASK)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH1_R (0xC3)
************************************************************************/
#define TAS6754_AC_LDG_CH1_R_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH1_R_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH1_R_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH1_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH1_R_VALUE_MASK)
#define TAS6754_AC_LDG_CH1_R_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH1_R_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH1_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH1_R_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH1_I (0xC4)
************************************************************************/
#define TAS6754_AC_LDG_CH1_I_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH1_I_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH1_I_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH1_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH1_I_VALUE_MASK)
#define TAS6754_AC_LDG_CH1_I_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH1_I_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH1_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH1_I_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH2_R (0xC5)
************************************************************************/
#define TAS6754_AC_LDG_CH2_R_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH2_R_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH2_R_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH2_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH2_R_VALUE_MASK)
#define TAS6754_AC_LDG_CH2_R_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH2_R_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH2_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH2_R_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH2_I (0xC6)
************************************************************************/
#define TAS6754_AC_LDG_CH2_I_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH2_I_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH2_I_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH2_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH2_I_VALUE_MASK)
#define TAS6754_AC_LDG_CH2_I_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH2_I_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH2_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH2_I_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH3_R (0xC7)
************************************************************************/
#define TAS6754_AC_LDG_CH3_R_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH3_R_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH3_R_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH3_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH3_R_VALUE_MASK)
#define TAS6754_AC_LDG_CH3_R_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH3_R_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH3_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH3_R_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH3_I (0xC8)
************************************************************************/
#define TAS6754_AC_LDG_CH3_I_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH3_I_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH3_I_MASK                           0xFF                /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH3_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH3_I_VALUE_MASK)
#define TAS6754_AC_LDG_CH3_I_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH3_I_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH3_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH3_I_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH4_R (0xC9)
************************************************************************/
#define TAS6754_AC_LDG_CH4_R_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH4_R_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH4_R_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH4_R_SIGN_MASK)
#define TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH4_R_VALUE_MASK)
#define TAS6754_AC_LDG_CH4_R_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH4_R_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH4_R_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH4_R_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_AC_LDG_REPORT_CH4_I (0xCA)
************************************************************************/
#define TAS6754_AC_LDG_CH4_I_SIGN_MASK                      BIT(7)              /* Bit 7: Sign bit (0: positive, 1: negative) */
#define TAS6754_AC_LDG_CH4_I_VALUE_MASK                     GENMASK(6, 0)       /* Bits 6-0: Magnitude value */
#define TAS6754_AC_LDG_CH4_I_MASK                           (0xFF)              /* Bits 7-0: Full value including sign */
#define TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val)               ((val) & TAS6754_AC_LDG_CH4_I_SIGN_MASK)
#define TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val)             ((val) & TAS6754_AC_LDG_CH4_I_VALUE_MASK)
#define TAS6754_AC_LDG_CH4_I_TO_OHMS_GAIN_1(val)            ((TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val) * 0.8f)
#define TAS6754_AC_LDG_CH4_I_TO_OHMS_GAIN_8(val)            ((TAS6754_AC_LDG_CH4_I_IS_NEGATIVE(val) ? -1.0f : 1.0f) * \
                                                            TAS6754_AC_LDG_CH4_I_GET_MAGNITUDE(val) * 0.1f)
/************************************************************************
* 					TAS6754_TWEETER_REPORT (0xCB)
************************************************************************/
#define TAS6754_TWEETER_REPORT_RESERVED_MASK                GENMASK(7, 4)       /* Bits 7-4: Reserved */
#define TAS6754_TWEETER_REPORT_CH1_MASK                     BIT(3)              /* Bit 3: CH1 TW DET */
#define TAS6754_TWEETER_REPORT_CH2_MASK                     BIT(2)              /* Bit 2: CH2 TW DET */
#define TAS6754_TWEETER_REPORT_CH3_MASK                     BIT(1)              /* Bit 1: CH3 TW DET */
#define TAS6754_TWEETER_REPORT_CH4_MASK                     BIT(0)              /* Bit 0: CH4 TW DET */
#define TAS6754_TWEETER_REPORT_CH1_NOT_DETECTED             (0x00 << 3)         /* 0: No tweeter detected on Channel 1 */
#define TAS6754_TWEETER_REPORT_CH1_DETECTED                 (0x01 << 3)         /* 1: Tweeter detected on Channel 1 */
#define TAS6754_TWEETER_REPORT_CH2_NOT_DETECTED             (0x00 << 2)         /* 0: No tweeter detected on Channel 2 */
#define TAS6754_TWEETER_REPORT_CH2_DETECTED                 (0x01 << 2)         /* 1: Tweeter detected on Channel 2 */
#define TAS6754_TWEETER_REPORT_CH3_NOT_DETECTED             (0x00 << 1)         /* 0: No tweeter detected on Channel 3 */
#define TAS6754_TWEETER_REPORT_CH3_DETECTED                 (0x01 << 1)         /* 1: Tweeter detected on Channel 3 */
#define TAS6754_TWEETER_REPORT_CH4_NOT_DETECTED             (0x00 << 0)         /* 0: No tweeter detected on Channel 4 */
#define TAS6754_TWEETER_REPORT_CH4_DETECTED                 (0x01 << 0)         /* 1: Tweeter detected on Channel 4 */
#define TAS6754_TWEETER_REPORT_ALL_CH_MASK                  (TAS6754_TWEETER_REPORT_CH1_MASK | \
                                                            TAS6754_TWEETER_REPORT_CH2_MASK | \
                                                            TAS6754_TWEETER_REPORT_CH3_MASK | \
                                                            TAS6754_TWEETER_REPORT_CH4_MASK)
/************************************************************************
* 					TAS6754_CH1_RTLDG_IMP_MSB (0xD1)
************************************************************************/
#define TAS6754_CH1_RTLDG_IMP_MSB_MASK                      (0xFF)              /* Bits 7-0: CH1 RTLDG IMPEDANCE MSB */
#define TAS6754_CH1_RTLDG_IMP_GET_MSB(val)                  ((val) & 0xFF)
#define TAS6754_CH1_RTLDG_IMP_COMBINE(msb, lsb)             (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH1_RTLDG_IMP_LSB (0xD2)
************************************************************************/
#define TAS6754_CH1_RTLDG_IMP_LSB_MASK                      (0xFF)              /* Bits 7-0: CH1 RTLDG IMPEDANCE LSB */
#define TAS6754_CH1_RTLDG_IMP_GET_LSB(val)                  ((val) & 0xFF)
#define TAS6754_CH1_RTLDG_IMP_TO_OHMS(msb, lsb)             (((float)TAS6754_CH1_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH2_RTLDG_IMP_MSB (0xD3)
************************************************************************/
#define TAS6754_CH2_RTLDG_IMP_MSB_MASK                      (0xFF)              /* Bits 7-0: CH2 RTLDG IMPEDANCE MSB */
#define TAS6754_CH2_RTLDG_IMP_GET_MSB(val)                  ((val) & 0xFF)
#define TAS6754_CH2_RTLDG_IMP_COMBINE(msb, lsb)             (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH2_RTLDG_IMP_LSB (0xD4)
************************************************************************/
#define TAS6754_CH2_RTLDG_IMP_LSB_MASK                      (0xFF)              /* Bits 7-0: CH2 RTLDG IMPEDANCE LSB */
#define TAS6754_CH2_RTLDG_IMP_GET_LSB(val)                  ((val) & 0xFF)
#define TAS6754_CH2_RTLDG_IMP_TO_OHMS(msb, lsb)             (((float)TAS6754_CH2_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH3_RTLDG_IMP_MSB (0xD5)
************************************************************************/
#define TAS6754_CH3_RTLDG_IMP_MSB_MASK                      (0xFF)              /* Bits 7-0: CH3 RTLDG IMPEDANCE MSB */
#define TAS6754_CH3_RTLDG_IMP_GET_MSB(val)                  ((val) & 0xFF)
#define TAS6754_CH3_RTLDG_IMP_COMBINE(msb, lsb)             (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH3_RTLDG_IMP_LSB (0xD6)
************************************************************************/
#define TAS6754_CH3_RTLDG_IMP_LSB_MASK                      (0xFF)              /* Bits 7-0: CH3 RTLDG IMPEDANCE LSB */
#define TAS6754_CH3_RTLDG_IMP_GET_LSB(val)                  ((val) & 0xFF)
#define TAS6754_CH3_RTLDG_IMP_TO_OHMS(msb, lsb)             (((float)TAS6754_CH3_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_CH4_RTLDG_IMP_MSB (0xD7)
************************************************************************/
#define TAS6754_CH4_RTLDG_IMP_MSB_MASK                      (0xFF)              /* Bits 7-0: CH4 RTLDG IMPEDANCE MSB */
#define TAS6754_CH4_RTLDG_IMP_GET_MSB(val)                  ((val) & 0xFF)
#define TAS6754_CH4_RTLDG_IMP_COMBINE(msb, lsb)             (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH4_RTLDG_IMP_LSB (0xD8)
************************************************************************/
#define TAS6754_CH4_RTLDG_IMP_LSB_MASK                      (0xFF)              /* Bits 7-0: CH4 RTLDG IMPEDANCE LSB */
#define TAS6754_CH4_RTLDG_IMP_GET_LSB(val)                  ((val) & 0xFF)
#define TAS6754_CH4_RTLDG_IMP_TO_OHMS(msb, lsb)             (((float)TAS6754_CH4_RTLDG_IMP_COMBINE(msb, lsb)) / 320.0f)
/************************************************************************
* 					TAS6754_DC_LDG_DCR_MSB (0xD9)
************************************************************************/
#define TAS6754_DC_LDG_DCR_CH1_MSB_MASK                     GENMASK(7, 6)       /* Bits 7-6: CH1 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH2_MSB_MASK                     GENMASK(5, 4)       /* Bits 5-4: CH2 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH3_MSB_MASK                     GENMASK(3, 2)       /* Bits 3-2: CH3 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH4_MSB_MASK                     GENMASK(1, 0)       /* Bits 1-0: CH4 DC RESISTANCE MSB */
#define TAS6754_DC_LDG_DCR_CH1_MSB_SHIFT                    (6)
#define TAS6754_DC_LDG_DCR_CH2_MSB_SHIFT                    (4)
#define TAS6754_DC_LDG_DCR_CH3_MSB_SHIFT                    (2)
#define TAS6754_DC_LDG_DCR_CH4_MSB_SHIFT                    (0)
#define TAS6754_DC_LDG_DCR_GET_CH1_MSB(val)                 (((val) & TAS6754_DC_LDG_DCR_CH1_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH1_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH2_MSB(val)                 (((val) & TAS6754_DC_LDG_DCR_CH2_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH2_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH3_MSB(val)                 (((val) & TAS6754_DC_LDG_DCR_CH3_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH3_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_GET_CH4_MSB(val)                 (((val) & TAS6754_DC_LDG_DCR_CH4_MSB_MASK) >> TAS6754_DC_LDG_DCR_CH4_MSB_SHIFT)
#define TAS6754_DC_LDG_DCR_COMBINE(msb, lsb)                (((uint16_t)(msb) << 8) | (lsb))
/************************************************************************
* 					TAS6754_CH1_DC_LDG_DCR_LSB (0xDA)
************************************************************************/
#define TAS6754_CH1_DC_LDG_DCR_LSB_MASK                     (0xFF)              /* Bits 7-0: CH1 DC RESISTANCE LSB */
#define TAS6754_CH1_DC_LDG_DCR_GET_LSB(val)                 ((val) & 0xFF)
#define TAS6754_CH1_DC_LDG_DCR_COMBINE(msb_val, lsb_val)    (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_CH1_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val)    (((float)TAS6754_CH1_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH2_DC_LDG_DCR_LSB (0xDB)
************************************************************************/
#define TAS6754_CH2_DC_LDG_DCR_LSB_MASK                     (0xFF)              /* Bits 7-0: CH2 DC RESISTANCE LSB */
#define TAS6754_CH2_DC_LDG_DCR_GET_LSB(val)                 ((val) & 0xFF)
#define TAS6754_CH2_DC_LDG_DCR_COMBINE(msb_val, lsb_val)    (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_CH2_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val)    (((float)TAS6754_CH2_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH3_DC_LDG_DCR_LSB (0xDC)
************************************************************************/
#define TAS6754_CH3_DC_LDG_DCR_LSB_MASK                     (0xFF)              /* Bits 7-0: CH3 DC RESISTANCE LSB */
#define TAS6754_CH3_DC_LDG_DCR_GET_LSB(val)                 ((val) & 0xFF)
#define TAS6754_CH3_DC_LDG_DCR_COMBINE(msb_val, lsb_val)    (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_CH3_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val)    (((float)TAS6754_CH3_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_CH4_DC_LDG_DCR_LSB (0xDD)
************************************************************************/
#define TAS6754_CH4_DC_LDG_DCR_LSB_MASK                     (0xFF)              /* Bits 7-0: CH4 DC RESISTANCE LSB */
#define TAS6754_CH4_DC_LDG_DCR_GET_LSB(val)                 ((val) & 0xFF)
#define TAS6754_CH4_DC_LDG_DCR_COMBINE(msb_val, lsb_val)    (((uint16_t)(msb_val) << 8) | (lsb_val))
#define TAS6754_CH4_DC_LDG_DCR_TO_OHMS(msb_val, lsb_val)    (((float)TAS6754_CH4_DC_LDG_DCR_COMBINE(msb_val, lsb_val)) * 0.1f)
/************************************************************************
* 					TAS6754_OTW_CTRL_CH1_CH2 (0xE2)
************************************************************************/
#define TAS6754_OTW_CTRL_RESERVED_7_MASK                    BIT(7)              /* Bit 7: Reserved */
#define TAS6754_OTW_CTRL_CH1_SEL_MASK                       GENMASK(6, 4)       /* Bits 6-4: CH1 OTW SEL */
#define TAS6754_OTW_CTRL_RESERVED_3_MASK                    BIT(3)              /* Bit 3: Reserved */
#define TAS6754_OTW_CTRL_CH2_SEL_MASK                       GENMASK(2, 0)       /* Bits 2-0: CH2 OTW SEL */
#define TAS6754_OTW_CTRL_CH1_DISABLED                       (0x00 << 4)         /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH1_95C                            (0x01 << 4)         /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH1_110C                           (0x02 << 4)         /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH1_125C                           (0x03 << 4)         /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH1_135C                           (0x04 << 4)         /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH1_145C                           (0x05 << 4)         /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH1_155C                           (0x06 << 4)         /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH1_165C                           (0x07 << 4)         /* 111: >165°C */
#define TAS6754_OTW_CTRL_CH2_DISABLED                       (0x00 << 0)         /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH2_95C                            (0x01 << 0)         /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH2_110C                           (0x02 << 0)         /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH2_125C                           (0x03 << 0)         /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH2_135C                           (0x04 << 0)         /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH2_145C                           (0x05 << 0)         /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH2_155C                           (0x06 << 0)         /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH2_165C                           (0x07 << 0)         /* 111: >165°C */
#define TAS6754_OTW_CTRL_CH1_CH2_RESERVED_MASK              (TAS6754_OTW_CTRL_RESERVED_7_MASK | \
                                                            TAS6754_OTW_CTRL_RESERVED_3_MASK)
/************************************************************************
* 					TAS6754_OTW_CTRL_CH3_CH4 (0xE3)
************************************************************************/
#define TAS6754_OTW_CTRL_RESERVED_7_MASK                    BIT(7)              /* Bit 7: Reserved */
#define TAS6754_OTW_CTRL_CH3_SEL_MASK                       GENMASK(6, 4)       /* Bits 6-4: CH3 OTW SEL */
#define TAS6754_OTW_CTRL_RESERVED_3_MASK                    BIT(3)              /* Bit 3: Reserved */
#define TAS6754_OTW_CTRL_CH4_SEL_MASK                       GENMASK(2, 0)       /* Bits 2-0: CH4 OTW SEL */
#define TAS6754_OTW_CTRL_CH3_DISABLED                       (0x00 << 4)         /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH3_95C                            (0x01 << 4)         /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH3_110C                           (0x02 << 4)         /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH3_125C                           (0x03 << 4)         /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH3_135C                           (0x04 << 4)         /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH3_145C                           (0x05 << 4)         /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH3_155C                           (0x06 << 4)         /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH3_165C                           (0x07 << 4)         /* 111: >165°C */
#define TAS6754_OTW_CTRL_CH4_DISABLED                       (0x00 << 0)         /* 000: OTW disabled */
#define TAS6754_OTW_CTRL_CH4_95C                            (0x01 << 0)         /* 001: >95°C */
#define TAS6754_OTW_CTRL_CH4_110C                           (0x02 << 0)         /* 010: >110°C */
#define TAS6754_OTW_CTRL_CH4_125C                           (0x03 << 0)         /* 011: >125°C */
#define TAS6754_OTW_CTRL_CH4_135C                           (0x04 << 0)         /* 100: >135°C */
#define TAS6754_OTW_CTRL_CH4_145C                           (0x05 << 0)         /* 101: >145°C */
#define TAS6754_OTW_CTRL_CH4_155C                           (0x06 << 0)         /* 110: >155°C */
#define TAS6754_OTW_CTRL_CH4_165C                           (0x07 << 0)         /* 111: >165°C */
#define TAS6754_OTW_CTRL_CH3_CH4_RESERVED_MASK              (TAS6754_OTW_CTRL_RESERVED_7_MASK | \
                                                            TAS6754_OTW_CTRL_RESERVED_3_MASK)

#endif /* __TAS6754_H__ */