/*
 * inmp441.c -- inmp441 ALSA SoC Codec driver
 */

#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <sound/soc.h>
#include <linux/init.h>               

#define DRV_NAME "inmp441"

/*
 * Private driver data structure for the INMP441 codec
 */
struct inmp441_priv {
	struct gpio_desc *sdmode_gpio; // Optional GPIO for mic shutdown/power
};

/* DAI ops struct: trigger, set_fmt etc. can be implemented if needed */
static const struct snd_soc_dai_ops inmp441_dai_ops = {
	/* Implement trigger(), set_fmt(), hw_params(), etc if needed */
};

/* Capture DAI */
static struct snd_soc_dai_driver inmp441_dai_driver = {
	.name = "capgemini,inmp441",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 1,
		.rates = SNDRV_PCM_RATE_8000_48000, // Accepts 8kHz to 48kHz sampling rates
		.formats = SNDRV_PCM_FMTBIT_S32_LE | // 32-bit PCM
		           SNDRV_PCM_FMTBIT_S24_LE | // 24-bit PCM
		           SNDRV_PCM_FMTBIT_S16_LE,  // 16-bit PCM
	},
	.ops = &inmp441_dai_ops,
};

/* Component probe (runs when ALSA binds this codec) */
static int inmp441_component_probe(struct snd_soc_component *component)
{
	struct inmp441_priv *inmp = snd_soc_component_get_drvdata(component);

	dev_info(component->dev, "INMP441: component probe called\n");

	/* Do codec-specific init here (previously in dai_probe) */
	if (inmp && inmp->sdmode_gpio)
		gpiod_set_value_cansleep(inmp->sdmode_gpio, 1); // Example: power on

	return 0;
}

/* Component driver */
static const struct snd_soc_component_driver inmp441_component_driver = {
	.probe = inmp441_component_probe,
	.name  = "inmp441",
};

/* Platform probe: register codec with ASoC */
static int inmp441_probe(struct platform_device *pdev)
{
	struct inmp441_priv *inmp;

	/* Allocate private data for this device */
	inmp = devm_kzalloc(&pdev->dev, sizeof(*inmp), GFP_KERNEL);
	if (!inmp)
		return -ENOMEM;

	/* Try to get the optional "sdmode" GPIO from device tree */
	inmp->sdmode_gpio = devm_gpiod_get_optional(&pdev->dev,
						   "sdmode",
						   GPIOD_OUT_LOW);
	if (IS_ERR(inmp->sdmode_gpio))
		return PTR_ERR(inmp->sdmode_gpio);

	/* Attach private data to ALSA component */
	platform_set_drvdata(pdev, inmp);

	dev_info(&pdev->dev, "INMP441 platform probe ran\n");

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

