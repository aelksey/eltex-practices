#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define NETLINK_USER 31  // Custom Netlink protocol number
#define USER_MSG    (NETLINK_USER + 1)
#define KERNEL_MSG  (NETLINK_USER + 2)

static struct sock *nl_sock = NULL;

// Structure for our custom message
struct custom_msg {
    char message[256];
    int value;
};

static void send_to_user(struct custom_msg *msg, pid_t pid);
static void nl_recv_msg(struct sk_buff *skb);
static int __init netlink_init(void);
static void __exit netlink_exit(void);

// Netlink configuration
static struct netlink_kernel_cfg nl_cfg = {
    .input = nl_recv_msg,
};

// Function to send message to userspace
static void send_to_user(struct custom_msg *msg, pid_t pid){
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    int msg_size = sizeof(struct custom_msg);
    int res;

    // Allocate skb with enough size
    skb = nlmsg_new(msg_size, GFP_KERNEL);
    if (!skb) {
        printk(KERN_ERR "Failed to allocate new skb\n");
        return;
    }

    // Create Netlink message header
    nlh = nlmsg_put(skb, 0, 0, NLMSG_DONE, msg_size, 0);
    if (!nlh) {
        printk(KERN_ERR "Failed to create nlmsg header\n");
        kfree_skb(skb);
        return;
    }

    // Copy our message data to the skb
    memcpy(nlmsg_data(nlh), msg, msg_size);

    // Send the message
    res = nlmsg_unicast(nl_sock, skb, pid);
    if (res < 0) {
        printk(KERN_ERR "Error while sending message to user: %d\n", res);
    }
}

// Netlink message receive callback
static void nl_recv_msg(struct sk_buff *skb){
    struct nlmsghdr *nlh;
    struct custom_msg *msg;
    pid_t pid;

    nlh = nlmsg_hdr(skb);
    pid = nlh->nlmsg_pid; // PID of the sending process

    // Extract the message data
    msg = (struct custom_msg *)nlmsg_data(nlh);

    printk(KERN_INFO "Received from userspace: %s, value: %d\n", msg->message, msg->value);

    // Process the message (example: increment value and send back)
    struct custom_msg response;
    
    strncpy(response.message, msg->message , sizeof(response.message));
    response.value = msg->value + 1;

    // Send response back to userspace
    send_to_user(&response, pid);
}

// Module initialization
static int __init netlink_init(void){
    printk(KERN_INFO "Netlink example module loaded\n");

    // Create Netlink socket
    nl_sock = netlink_kernel_create(&init_net, NETLINK_USER, &nl_cfg);
    if (!nl_sock) {
        printk(KERN_ALERT "Error creating Netlink socket\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Netlink socket created successfully\n");
    return 0;
}

// Module cleanup
static void __exit netlink_exit(void) {
    if (nl_sock) {
        netlink_kernel_release(nl_sock);
        printk(KERN_INFO "Netlink socket released\n");
    }
    printk(KERN_INFO "Netlink example module unloaded\n");
}

module_init(netlink_init);
module_exit(netlink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Vladimirov");
MODULE_DESCRIPTION("Module 5 task 5");