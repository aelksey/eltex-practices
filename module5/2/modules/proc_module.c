#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/stat.h>

#define SUCESS 0
#define FAILURE 1
#define PROC_FILE_NAME "proc_module"
// Режим доступа: 0666 (rw-rw-rw-)
#define PROC_FILE_PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define MSGSIZE 20

// Keyword static изолирует переменные модуля до собственной единицы трансляции

static int len,temp;
static char *msg;

static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp);
static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp);
static void create_new_proc_entry(void);
static int proc_init (void);
static void proc_cleanup(void);

static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp) {
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    // Побайтово копируем kernelspace в userspace (из msg в buf)
    if(copy_to_user(buf, msg, count)) return FAILURE; // Added error handling when copying from kernelspace to userspace
    if(count == 0)temp = len;
    return count;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    // Побайтово копируем userspace в kernelspace( из buf в msg )
    if(copy_from_user(msg, buf, count)) return FAILURE;
    len = count;
    temp = len;
    return count;
}
 
static const struct proc_ops proc_fops = {
    .proc_read = read_proc,
    .proc_write = write_proc,
};
 
static void create_new_proc_entry(void) {
    // NULL - нулевой родитель proc_dir_entry
    if(proc_create(PROC_FILE_NAME, PROC_FILE_PERMS , NULL, &proc_fops))printk(KERN_ERR "proc_create failed");
    msg = kmalloc(MSGSIZE * sizeof(char), GFP_KERNEL);
}
 
static int proc_init (void) {
    create_new_proc_entry();
    return SUCESS;
}
 
static void proc_cleanup(void) {
    remove_proc_entry(PROC_FILE_NAME, NULL);
    kfree(msg);
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Vladimirov");
module_init(proc_init);
module_exit(proc_cleanup);