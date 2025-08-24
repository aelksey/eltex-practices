#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "my_chardev"
#define CLASS_NAME "chardev_class"
#define BUFFER_SIZE 1024

static int major_number;
static struct class *chardev_class = NULL;
static struct cdev my_cdev;

// Kernel buffer for data storage
static char *kernel_buffer;
static size_t buffer_size = 0;

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset);

// Device file operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write,
};

static int device_open(struct inode *inode, struct file *file){
    printk(KERN_INFO "Chardev: Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file){
    printk(KERN_INFO "Chardev: Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset){
    int bytes_to_read;
    int retval;

    // If offset is beyond buffer, reset to beginning (like a circular buffer)
    if (*offset >= buffer_size) {
        *offset = 0;
    }

    bytes_to_read = min(buffer_size - *offset, length);

    if (bytes_to_read == 0) {
        printk(KERN_INFO "Chardev: Buffer empty\n");
        return 0;
    }

    retval = copy_to_user(buffer, kernel_buffer + *offset, bytes_to_read);
    
    if (retval) {
        printk(KERN_ERR "Chardev: Failed to send %d bytes to user\n", retval);
        return -EFAULT;
    }

    *offset += bytes_to_read;
    printk(KERN_INFO "Chardev: Read %d bytes from offset %lld\n", bytes_to_read, *offset);

    return bytes_to_read;
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset){
    
    int bytes_to_write;
    int bytes_written = 0;
    int retval;

    // Check if we need to allocate more space
    if (*offset + length > BUFFER_SIZE) {
        printk(KERN_ERR "Chardev: Write would exceed buffer size\n");
        return -ENOMEM;
    }

    bytes_to_write = min(BUFFER_SIZE - (size_t)*offset, length);

    if (bytes_to_write == 0) {
        printk(KERN_INFO "Chardev: Buffer full\n");
        return -ENOMEM;
    }

    // Copy data from user space to kernel space
    retval = copy_from_user(kernel_buffer + *offset, buffer, bytes_to_write);
    
    if (retval) {
        printk(KERN_ERR "Chardev: Failed to write %d bytes\n", retval);
        return -EFAULT;
    }

    *offset += bytes_to_write;
    bytes_written = bytes_to_write;
    buffer_size = max(buffer_size, (size_t)*offset);

    printk(KERN_INFO "Chardev: Wrote %d bytes at offset %lld\n", bytes_written, *offset);

    return bytes_written;
}

static int __init chardev_init(void){
    
    int retval;

    printk(KERN_INFO "Chardev: Initializing module\n");

    // Allocate kernel buffer
    kernel_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        printk(KERN_ERR "Chardev: Failed to allocate kernel buffer\n");
        return -ENOMEM;
    }
    memset(kernel_buffer, 0, BUFFER_SIZE);

    // Allocate major number
    retval = alloc_chrdev_region(&major_number, 0, 1, DEVICE_NAME);
    if (retval < 0) {
        printk(KERN_ERR "Chardev: Failed to allocate major number\n");
        kfree(kernel_buffer);
        return retval;
    }

    printk(KERN_INFO "Chardev: Registered with major number %d\n", major_number);

    // Create device class
    chardev_class = class_create(CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        unregister_chrdev_region(major_number, 1);
        kfree(kernel_buffer);
        return PTR_ERR(chardev_class);
    }

    // Create device
    device_create(chardev_class, NULL, major_number, NULL, DEVICE_NAME);

    // Initialize and add character device
    cdev_init(&my_cdev, &fops);

    retval = cdev_add(&my_cdev, major_number, 1);
    if (retval < 0) {
        device_destroy(chardev_class, major_number);
        class_destroy(chardev_class);
        unregister_chrdev_region(major_number, 1);
        kfree(kernel_buffer);
        return retval;
    }

    printk(KERN_INFO "Chardev: Module initialized successfully\n");
    return 0;
}

static void __exit chardev_exit(void){
    
    // Remove device
    device_destroy(chardev_class, major_number);
    
    // Destroy class
    class_destroy(chardev_class);
    
    // Remove character device
    cdev_del(&my_cdev);
    
    // Unregister major number
    unregister_chrdev_region(major_number, 1);
    
    // Free kernel buffer
    kfree(kernel_buffer);
    
    printk(KERN_INFO "Chardev: Module unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Vladimirov");
MODULE_DESCRIPTION("Eltex module 5 task 4");