#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/barrier.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Memory Barrier Example for STM32MP157D");

// Shared data structure between cores
struct shared_data {
    int flag;
    int value;
} *shared;

static int __init barrier_module_init(void)
{
    shared = kmalloc(sizeof(struct shared_data), GFP_KERNEL);
    if (!shared)
        return -ENOMEM;
    
    // Initialize shared data
    shared->flag = 0;
    shared->value = 0;
    
    // Ensure initialization is visible to other cores
    smp_mb();  // Full memory barrier in kernel space
    
    printk(KERN_INFO "Memory barrier module initialized\n");
    return 0;
}

static void update_shared_data(int new_value)
{
    // Update value first
    shared->value = new_value;
    
    // Ensure value is written before flag
    smp_mb();
    
    // Set flag to indicate update is complete
    shared->flag = 1;
}

static void __exit barrier_module_exit(void)
{
    kfree(shared);
    printk(KERN_INFO "Memory barrier module unloaded\n");
}

module_init(barrier_module_init);
module_exit(barrier_module_exit);