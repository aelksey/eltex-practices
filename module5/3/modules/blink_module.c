#include <linux/init.h> 
#include <linux/kd.h> 
#include <linux/module.h> 
#include <linux/tty.h> 
#include <linux/vt.h> 
#include <linux/vt_kern.h>  
#include <linux/console_struct.h>
#include <linux/string.h>
#include<linux/sysfs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/stat.h>
 
static struct timer_list my_timer; // Таймер
static struct tty_driver *my_driver; // Структура терминального драйвера tty_driver
static unsigned long kbledstatus = 0; // Текущее сотояние светодиодов (0 1 2 3)
static int test;

#define BLINK_DELAY HZ / 5 // Задержка мигания раз в 5 сек 
#define ALL_LEDS_ON 0x07 // Маска светодиодов (Все ВКЛ)
#define RESTORE_LEDS 0xFF // Сбросить статус светодиододов на управление с клавиатуры
#define PERMS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

// Объект kernel /sys/kernel/systest
#define KOBJECT_NAME "systest"

static struct kobject *example_kobject;

static void my_timer_func(struct timer_list *unused);
static int __init kbleds_init(void);
static void __exit kbleds_cleanup(void);
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static int sys_init(void);
static void sys_exit(void);

// Мигает светодиодом выполняя терминальную операцию KBSETLED для ioctl() на драйвере клавиатуры
static void my_timer_func(struct timer_list *unused) { 
    struct tty_struct *t = vc_cons[fg_console].d->port.tty;
    
    // Переключаем статус светодиодов на тот, что получили из sysfs
    kbledstatus = test;
    
    // Переключаем статус светодиодов
    //if (kbledstatus == ALL_LEDS_ON) kbledstatus = RESTORE_LEDS; 
    //else kbledstatus = ALL_LEDS_ON;

    (my_driver->ops->ioctl)(t, KDSETLED, kbledstatus);
    // Переменная jiffies содержит количество тактов с момента bootа системы 
    my_timer.expires = jiffies + BLINK_DELAY; 
    add_timer(&my_timer); 
} 

// Выполняется при загрузке модуля в ядро (явно помечено атрибутом __init)
static int __init kbleds_init(void){
    sys_init(); 
    int i; 
    pr_info("kbleds: loading\n"); 
    pr_info("kbleds: fgconsole is %x\n", fg_console); 

    for (i = 0; i < MAX_NR_CONSOLES; i++) { 
        if (!vc_cons[i].d) break; 
        pr_info("poet_atkm: console[%i/%i] #%i, tty %p\n", i, MAX_NR_CONSOLES, vc_cons[i].d->vc_num, (void *)vc_cons[i].d->port.tty); 
    } 

    pr_info("kbleds: finished scanning consoles\n"); 
    my_driver = vc_cons[fg_console].d->port.tty->driver; 
    pr_info("kbleds: tty driver name %s\n", my_driver->driver_name); 
    // Инициализация таймера
    timer_setup(&my_timer, my_timer_func, 0); 
    my_timer.expires = jiffies + BLINK_DELAY; 
    add_timer(&my_timer); 
    return 0; 
} 

// Выполняется при выгрузке модуля (явно помечено атрибутом __exit)
static void __exit kbleds_cleanup(void){
    sys_exit();
    pr_info("kbleds: unloading...\n"); 
    del_timer(&my_timer); 
    (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS); 
}

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
MODULE_AUTHOR("Aleksey Vladimirov");
MODULE_DESCRIPTION("Eltex module 5 task 3");

module_init(kbleds_init); 
module_exit(kbleds_cleanup); 