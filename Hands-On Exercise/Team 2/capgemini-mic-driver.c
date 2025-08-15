/**
 * @file
 * @brief [Brief description of the file's purpose]
 *
 * @details [Optional: More detailed description of the file]
 *
 * @author Ulises G
 * @date 14-08-2025
 *
 * @version 1.0
 * @note Changelog:
 * - 14-08-2025: testing my WSL — Victor Martinez
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A basic kernel module template");
MODULE_VERSION("1.0");

// Module initialization function
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello: Module loaded successfully\n");
    return 0; // Return 0 for success
}

// Module cleanup function
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello: Module unloaded successfully\n");
}

// Register init and exit functions
module_init(hello_init);
module_exit(hello_exit);
