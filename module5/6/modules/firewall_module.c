#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/inet.h>
#include <net/ip.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/netfilter_ipv4.h>
#include <linux/net_namespace.h> // для структуры init_net;
/* extern struct net init_net; */



#define PROC_FILE "blacklist_ip"
#define PROC_PERMS 0666
#define CMD_BUFSIZE 16
#define CMD_IP_BUFSIZE 64


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Vladimirov");
MODULE_DESCRIPTION("IP Filtering Module for Outgoing Packets");

static ssize_t read_blacklist(struct file *file, char __user *buf, size_t count, loff_t *ppos);
static ssize_t write_blacklist(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
static unsigned int outgoing_filter(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
static int __init init_firewall(void);
static void __exit cleanup_firewall(void);

// Структура хранения черного списка IP-адресов
static struct blacklist_entry {
    struct in_addr addr;
    struct list_head list;
};

static LIST_HEAD(blacklist);

static struct nf_hook_ops netfilter_ops = {
    .hook = outgoing_filter,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
    .hooknum  = NF_INET_LOCAL_OUT,
};

// Описание файловых операций для /proc
static const struct proc_ops proc_fops = {
    .proc_read = read_blacklist,
    .proc_write = write_blacklist,
};

// Операции чтения / записи в /proc

static ssize_t read_blacklist(struct file *file, char __user *buf, size_t count, loff_t *ppos){
    int len = 0;
    struct blacklist_entry *entry;
    char buffer[PAGE_SIZE];
    
    if (*ppos != 0) return 0;
        
    list_for_each_entry(entry, &blacklist, list) { len += snprintf(buffer + len, PAGE_SIZE - len, "%pI4\n", &(entry->addr)); }
    
    if (copy_to_user(buf, buffer, len)) return -EFAULT;
    
    *ppos += len;

    return len;
}

static ssize_t write_blacklist(struct file *file, const char __user *buf, size_t count, loff_t *ppos){
    int retval;
    const char *end;
    char command_ip[CMD_IP_BUFSIZE],command[CMD_BUFSIZE], ip_str[INET_ADDRSTRLEN];
    struct sockaddr_in sa;
    struct blacklist_entry *new_entry, *tmp, *entry;
    
    // Читаем введенные данные команды и IP
    if (count > sizeof(command_ip)) {
        printk(KERN_ERR "Command too long.\n");
        return -EINVAL;
    }
    
    retval = copy_from_user(command_ip, buf, count);

    if (retval) {
        printk(KERN_ERR "copy_from_user fail");
        return -EFAULT;
    }


    sscanf(command_ip, "%s %s", command, ip_str);
    
    if (!in4_pton(ip_str, sizeof(ip_str), (unsigned char*)&sa.sin_addr, -1, &end)) {
        printk(KERN_ERR "Invalid IP address format.\n");
        return -EINVAL;
    }

    
    new_entry = kmalloc(sizeof(*new_entry), GFP_KERNEL);
    if (!new_entry) {
        printk(KERN_ERR "Memory allocation failed.\n");
        return -ENOMEM;
    }
    
    memcpy(&new_entry->addr, &sa.sin_addr, sizeof(sa.sin_addr));
    INIT_LIST_HEAD(&new_entry->list);
    
    if (strcmp(command, "add") == 0) {   // Добавляем новый IP
        list_add_tail(&new_entry->list, &blacklist);
        printk(KERN_INFO "Adding %p(%s) to blacklist.\n", &new_entry->addr,ip_str);
    } else if (strcmp(command, "del") == 0) { // Удаляем существующий IP
        list_for_each_entry_safe(tmp, entry, &blacklist, list) {
            if (memcmp(&tmp->addr, &new_entry->addr, sizeof(new_entry->addr)) == 0) {
                printk(KERN_INFO "Removing %p(%s) from blacklist.\n", &tmp->addr,ip_str);
                list_del(&tmp->list);
                kfree(tmp);
                break;
            }
        }
        kfree(new_entry); // Очищаем выделенную память
    } else {
        printk(KERN_ERR "Unknown command '%s'.\n", command);
        kfree(new_entry);
        return -EINVAL;
    }
    
    return count;
}

// Функция фильтрации NF_HOOK
static unsigned int outgoing_filter(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    struct iphdr *iph = ip_hdr(skb);
    struct blacklist_entry *entry;
    
    list_for_each_entry(entry, &blacklist, list) {
        if (iph->daddr == entry->addr.s_addr) {
            printk(KERN_INFO "Dropped packet destined to %pI4.\n", &entry->addr);
            return NF_DROP;
        }
    }
    
    return NF_ACCEPT;
}

// Инициализация модуля
static int __init init_firewall(void) {
    nf_register_net_hook(&init_net,&netfilter_ops);
    proc_create(PROC_FILE, PROC_PERMS, NULL, &proc_fops);
    printk(KERN_INFO "Module loaded successfully.\n");
    return 0;
}

// Выход из модуля
static void __exit cleanup_firewall(void) {
    nf_unregister_net_hook(&init_net,&netfilter_ops);
    remove_proc_entry(PROC_FILE, NULL);
    printk(KERN_INFO "Module unloaded successfully.\n");
}

module_init(init_firewall);
module_exit(cleanup_firewall);