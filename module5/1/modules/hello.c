#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("Apache License 2.0"); // Changed from "GPL" to "Apache License 2.0" 
MODULE_AUTHOR("Aleksey Vladimirov"); // Changed from "Lakshmanan" to "Aleksey Vladimirov"
MODULE_DESCRIPTION("Eltex Module 5 Task 1"); // Changed from "A Simple Hello World module" to "Eltex Module 5 Task 1 module" 

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world! I'm Eltex Student\n"); // Changed from "Hello world!\n" to "Hello world!\nI'm Eltex Student\"
    // Added lines below
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);