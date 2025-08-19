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
 * Component probe callback
 * This is called by the ALSA SoC core when the codec is initialized
 */
static int inmp441_probe(struct snd_soc_component *component)
{
	struct inmp441_priv *inmp = snd_soc_component_get_drvdata(component);

	// If the SD_MODE GPIO is defined, enable the microphone (set GPIO high)
	if (inmp->sdmode_gpio)
		gpiod_set_value_cansleep(inmp->sdmode_gpio, 1); // Power ON mic

	return 0;
}

/*
 * Component remove callback
 * Called when the codec is removed/unloaded (e.g., at shutdown or reboot)
 */
static void inmp441_remove(struct snd_soc_component *component)
{
	struct inmp441_priv *inmp = snd_soc_component_get_drvdata(component);

	// If the SD_MODE GPIO is defined, disable the microphone (set GPIO low)
	if (inmp->sdmode_gpio)
		gpiod_set_value_cansleep(inmp->sdmode_gpio, 0); // Power OFF mic
}

/*
 * DAI (Digital Audio Interface) configuration
 * This defines the audio capture capabilities of the INMP441
 */
static struct snd_soc_dai_driver inmp441_dai = {
	.name = "inmp441-hifi", // DAI name, referenced by simple-audio-card

	.capture = {
		.stream_name = "Capture",    // Name shown in ALSA
		.channels_min = 1,           // INMP441 is mono
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000_48000, // Accepts 8kHz to 48kHz sampling rates
		.formats = SNDRV_PCM_FMTBIT_S32_LE | // 32-bit PCM
		           SNDRV_PCM_FMTBIT_S24_LE | // 24-bit PCM
		           SNDRV_PCM_FMTBIT_S16_LE,  // 16-bit PCM
	},
};

/*
 * Minimal codec component driver
 * This does not include controls or widgets — it's a passive I2S source
 */
static const struct snd_soc_component_driver soc_component_dev_inmp441 = {
	.probe  = inmp441_probe,   // Called during codec init
	.remove = inmp441_remove,  // Called during codec removal
	.name   = "inmp441",       // Codec name shown in `arecord -l`
};

/*
 * Platform driver probe
 * Called when a matching Device Tree node is found (e.g., compatible = "invensense,inmp441")
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

// This macro sets up init and exit routines automatically
module_platform_driver(inmp441_driver);

// Metadata for modinfo
MODULE_AUTHOR("Andres Mora <andres.mora@capgemini.com>");
MODULE_DESCRIPTION("INMP441 I2S MEMS microphone Codec Driver");
MODULE_LICENSE("GPL");