#include <linux/module.h> 
#include <linux/printk.h> 
#include <linux/kobject.h> 
#include <linux/sysfs.h> 
#include <linux/init.h> 
#include <linux/fs.h> 


#include <linux/string.h>
#include <linux/slab.h>
#include <linux/stat.h>
 
// Режим доступа: 0666 (rw-rw-rw-)
#define PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define KOBJECT_NAME "systest"

static struct kobject *example_kobject;
static int test;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static int sys_init(void);
static void sys_exit(void);


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    return sprintf(buf, "%d\n", test);
}
 
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    sscanf(buf, "%du", &test);
    return count;
}
 
static struct kobj_attribute foo_attribute =__ATTR(test, PERMS, foo_show, foo_store);
 
static int sys_init (void) {
    int error = 0;
 
    example_kobject = kobject_create_and_add(KOBJECT_NAME, kernel_kobj);
    if(!example_kobject) return -ENOMEM;
 
    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error) {
        pr_debug("failed to create the foo file in /sys/kernel/systest \n");
    }
 
    return error;
}
 
static void sys_exit(void){
    kobject_put(example_kobject);
}
 
MODULE_LICENSE("GPL");
module_init(sys_init);
module_exit(sys_exit);