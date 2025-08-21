/**
 * @file
 * @brief [Brief description of the file's purpose]
 *
 * @details [Optional: More detailed description of the file]
 *
 * @author Victor M
 * @date 14-08-2025
 *
 * @version 1.0
 * @note Changelog:
 * - 19-08-2025: initial revision — Victor Martinez
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <sound/soc.h>


/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Victor Martinez");
MODULE_DESCRIPTION("mic driver  for INMP411");
MODULE_VERSION("1.0");

#include <linux/module.h>
#include <sound/soc.h>

static struct snd_soc_dai_driver mh_i2s_dai = {
    .name = "mh-i2s-mic",
    .playback = {
        .stream_name = "Playback",
        .channels_min = 1,
        .channels_max = 2,
        .rates = SNDRV_PCM_RATE_8000_48000,
        .formats = SNDRV_PCM_FMTBIT_S16_LE,
    },
    .capture = {
        .stream_name = "Capture",
        .channels_min = 1,
        .channels_max = 2,
        .rates = SNDRV_PCM_RATE_8000_48000,
        .formats = SNDRV_PCM_FMTBIT_S16_LE,
    },
};

static struct snd_soc_component_driver mh_i2s_component = {
    .name = "mh-i2s-mic",
};

static int mh_i2s_probe(struct platform_device *pdev)
{
    return snd_soc_register_component(&pdev->dev, &mh_i2s_component, &mh_i2s_dai, 1);
}

static int mh_i2s_remove(struct platform_device *pdev)
{
    snd_soc_unregister_component(&pdev->dev);
    return 0;
}

static struct platform_driver mh_i2s_driver = {
    .driver = {
        .name = "mh-i2s-mic",
        .owner = THIS_MODULE,
    },
    .probe = mh_i2s_probe,
    .remove = mh_i2s_remove,
};

module_platform_driver(mh_i2s_driver);

/*  Register init and exit functions */
module_init(hello_init);
module_exit(hello_exit);
