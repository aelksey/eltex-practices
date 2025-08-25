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
#include <linux/bug.h>

static struct timer_list my_timer; // Таймер
static struct tty_driver *my_driver; // Структура символьного устройства tty_driver
static unsigned long kbledstatus = 0; // Текущее сотояние светодиодов

// Blink Controls
// 0 - all_leds : 1 - caps_lock : 2 - num_lock 
static int state;

#define BLINK_DELAY HZ / 5 // Задержка мигания раз в 5 сек
#define ALL_LEDS_ON 0x07 // Маска светодиодов (Все ВКЛ) 
#define NUMLOCK_LED 0x03 // Маска светодиодов (Все ВКЛ)
#define CAPSLOCK_LED 0x05 // Маска светодиодов (Caps_Lock)
#define RESTORE_LEDS 0xFF // Сбросить статус светодиододов на управление с клавиатуры
#define PERMS 0660

// Объект kernel /sys/kernel/blink/state
#define KOBJECTDIR "blink"

static struct kobject *example_kobject;

static unsigned long get_status(int state);
static void my_timer_func(struct timer_list *unused);
static int __init kbleds_init(void);
static void __exit kbleds_cleanup(void);
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count);
static int sys_init(void);
static void sys_exit(void);

static unsigned long get_status(int state){
    unsigned long status;
    if (state == 0) status = ALL_LEDS_ON;
    if(state == 1 ) status = CAPSLOCK_LED;
    if(state == 2) status = NUMLOCK_LED;
    return status;
}

// Мигает светодиодом выполняя терминальную операцию KBSETLED для ioctl() на драйвере клавиатуры
static void my_timer_func(struct timer_list *unused) { 
    struct tty_struct *t = vc_cons[fg_console].d->port.tty;

    // Переключаем статус светодиодов
    if (kbledstatus == get_status(state)) kbledstatus = RESTORE_LEDS; 
    else kbledstatus = get_status(state);

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
    return sprintf(buf, "%d\n", state);
}
 
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
    sscanf(buf, "%du", &state);
    return count;
}
 
static struct kobj_attribute foo_attribute =__ATTR(state, PERMS, foo_show, foo_store);
 
static int sys_init (void) {
    int error = 0;
 
    example_kobject = kobject_create_and_add(KOBJECTDIR, kernel_kobj);
    if(!example_kobject) return -ENOMEM;
 
    error = sysfs_create_file(example_kobject, &foo_attribute.attr);
    if (error) {
        pr_debug("failed to create the state file in /sys/kernel/blink \n");
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