// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (c) 2010-2011,2013-2015 The Linux Foundation. All rights reserved.
 *
 * inmp441.c -- inmp441 ALSA SoC Codec driver
 */

#include <linux/acpi.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <sound/pcm.h>
#include <sound/soc.h>
#include <sound/soc-dai.h>
#include <sound/soc-dapm.h>
#include <sound/core.h>
#include <sound/initval.h>
#define DRV_NAME "inmp441"

/* DAI operations (mostly empty because PCM handled by SoC) */
static int inmp441_dai_probe(struct snd_soc_dai *dai)
{
	/* Any codec-specific initialization can go here */
	return 0;
}

/* DAI ops struct: trigger, set_fmt etc. can be implemented if needed */
static const struct snd_soc_dai_ops inmp441_dai_ops = {
	/* For real hardware, you could implement trigger(), set_fmt(), etc. */
};

/* Capture DAI */
static struct snd_soc_dai_driver inmp441_dai_driver = {
	.name = "INMP441 Capture",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = SNDRV_PCM_RATE_48000,
		.formats = SNDRV_PCM_FMTBIT_S16_LE,
	},
	.ops = &inmp441_dai_ops,
};

/* Component driver (mostly empty for this simple codec) */
static struct snd_soc_component_driver inmp441_component_driver = {
	.name = "inmp441",
};

/* Platform probe: register codec with ASoC */
static int inmp441_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
					      &inmp441_component_driver,
					      &inmp441_dai_driver, 1);
}

static const struct of_device_id inmp441_of_match[] = {
	{ .compatible = "capgemini,inmp441" },
	{}
};
MODULE_DEVICE_TABLE(of, inmp441_of_match);

/* Platform driver registration */
static struct platform_driver inmp441_driver = {
	.driver = {
		.name = DRV_NAME,
		.of_match_table = inmp441_of_match,
	},
	.probe = inmp441_probe,
};
module_platform_driver(inmp441_driver);

MODULE_DESCRIPTION("INMP441 MEMS Microphone Codec Driver");
MODULE_LICENSE("GPL v2");
