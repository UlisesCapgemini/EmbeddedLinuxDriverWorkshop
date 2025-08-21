// SPDX-License-Identifier: GPL-2.0-only
/*
 * Minimal codec driver for Invensense INMP441 I2S MEMS microphone
 *
 * Author: Andres Mora <andres.mora@capgemini.com>
 */

#include <linux/module.h>              // Core header for modules
#include <linux/init.h>                // For module init/exit macros
#include <linux/platform_device.h>     // For platform device driver infrastructure
#include <linux/of.h>                  // For device tree parsing
#include <linux/gpio/consumer.h>       // For GPIO descriptor framework
#include <sound/soc.h>                 // ALSA SoC audio framework

/*
 * Private driver data structure for the INMP441 codec
 * Used to store the optional SD_MODE GPIO descriptor
 */
struct inmp441_priv {
	struct gpio_desc *sdmode_gpio; // Optional GPIO for mic shutdown/power
};

/*
 * PCM trigger callback
 * Called by ALSA core on stream start/stop/resume/suspend
 * Used here to control the SD_MODE GPIO if present
 */
static int inmp441_trigger(struct snd_pcm_substream *substream,
			   int cmd, struct snd_soc_dai *dai)
{
	struct snd_soc_component *component = dai->component;
	struct inmp441_priv *inmp = snd_soc_component_get_drvdata(component);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (inmp->sdmode_gpio)
			gpiod_set_value(inmp->sdmode_gpio, 1); // Power ON mic
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (inmp->sdmode_gpio)
			gpiod_set_value(inmp->sdmode_gpio, 0); // Power OFF mic
		break;
	}

	return 0;
}

/*
 * Digital Audio Interface (DAI) definition
 * This describes the capabilities of the mic
 */
static const struct snd_soc_dai_ops inmp441_dai_ops = {
	.trigger = inmp441_trigger, // Register trigger callback
};

static struct snd_soc_dai_driver inmp441_dai = {
	.name = "invensense,inmp441", // DAI name
	.capture = {
		.stream_name = "Capture",    // Displayed in arecord -l
		.channels_min = 1,           // Mono microphone
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000_48000, // Common sampling rates
		.formats = SNDRV_PCM_FMTBIT_S16_LE | // Supported PCM formats
		           SNDRV_PCM_FMTBIT_S24_LE |
		           SNDRV_PCM_FMTBIT_S32_LE,
	},
	.ops = &inmp441_dai_ops, // DAI ops for trigger
};

/*
 * DAPM widget: virtual representation of mic input
 */
static const struct snd_soc_dapm_widget inmp441_dapm_widgets[] = {
	SND_SOC_DAPM_INPUT("INMP441 Mic"),
};

/*
 * DAPM route: Connect mic input to capture stream
 */
static const struct snd_soc_dapm_route inmp441_dapm_routes[] = {
	{"Capture", NULL, "INMP441 Mic"},
};

/*
 * ALSA codec component driver
 * Provides DAPM info and ties everything together
 */
static const struct snd_soc_component_driver soc_component_dev_inmp441 = {
	.name = "inmp441", // Shown in ALSA logs
	.dapm_widgets = inmp441_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(inmp441_dapm_widgets),
	.dapm_routes = inmp441_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(inmp441_dapm_routes),
};

/*
 * Platform probe function
 * Called when Device Tree node is matched
 */
static int inmp441_platform_probe(struct platform_device *pdev)
{
	struct inmp441_priv *inmp;

	// Allocate private data for this device
	inmp = devm_kzalloc(&pdev->dev, sizeof(*inmp), GFP_KERNEL);
	if (!inmp)
		return -ENOMEM;

	/*
	 * Try to get the optional "sdmode" GPIO from device tree
	 * If not defined, this returns NULL (and that’s fine)
	 */
	inmp->sdmode_gpio = devm_gpiod_get_optional(&pdev->dev, "sdmode", GPIOD_OUT_LOW);
	if (IS_ERR(inmp->sdmode_gpio))
		return PTR_ERR(inmp->sdmode_gpio);

	// Associate this private data with the ALSA component
	platform_set_drvdata(pdev, inmp);

	// Register the codec component with the SoC audio subsystem
	return devm_snd_soc_register_component(&pdev->dev,
			&soc_component_dev_inmp441, // Component callbacks and name
			&inmp441_dai,               // DAI capabilities (capture only)
			1);                         // Number of DAIs
}

/*
 * Match table for Device Tree binding
 * Binds this driver to `compatible = "invensense,inmp441"`
 */
static const struct of_device_id inmp441_of_match[] = {
	{ .compatible = "invensense,inmp441", },
	{ }
};
MODULE_DEVICE_TABLE(of, inmp441_of_match);

/*
 * Register this as a platform driver in the kernel
 * This driver will be bound if the compatible string is matched
 */
static struct platform_driver inmp441_driver = {
	.driver = {
		.name = "inmp441",                  // Internal driver name
		.of_match_table = inmp441_of_match, // Match against DT
	},
	.probe = inmp441_platform_probe, // Called when DT match occurs
};

/*
 * Register init/exit routines for kernel module
 */
module_platform_driver(inmp441_driver);

// Metadata for modinfo
MODULE_AUTHOR("Andres Mora <andres.mora@capgemini.com>");
MODULE_DESCRIPTION("INMP441 I2S MEMS microphone Codec Driver");
MODULE_LICENSE("GPL");
