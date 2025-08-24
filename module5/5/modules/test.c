#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <assert.h>

#define NETLINK_USER 31
#define MAX_PAYLOAD 1024

struct custom_msg {
    char message[256];
    int value;
};

// Function to create and setup Netlink socket
int setup_netlink_socket(struct sockaddr_nl *src_addr, struct sockaddr_nl *dest_addr) {
    int sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    assert(sock_fd >= 0 && "Failed to create Netlink socket");

    // Setup source address
    memset(src_addr, 0, sizeof(struct sockaddr_nl));
    src_addr->nl_family = AF_NETLINK;
    src_addr->nl_pid = getpid();

    // Bind socket
    int ret = bind(sock_fd, (struct sockaddr*)src_addr, sizeof(struct sockaddr_nl));
    assert(ret == 0 && "Failed to bind Netlink socket");

    // Setup destination address (kernel)
    memset(dest_addr, 0, sizeof(struct sockaddr_nl));
    dest_addr->nl_family = AF_NETLINK;
    dest_addr->nl_pid = 0; // kernel PID
    dest_addr->nl_groups = 0; // unicast

    return sock_fd;
}

// Function to send message to kernel and receive response
void test_netlink_communication(int sock_fd, struct sockaddr_nl *dest_addr, const char *test_message, int test_value){
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    struct msghdr msg;
    int ret;

    // Allocate and prepare message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(struct custom_msg)));
    assert(nlh != NULL && "Failed to allocate memory for Netlink message");

    memset(nlh, 0, NLMSG_SPACE(sizeof(struct custom_msg)));
    nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct custom_msg));
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    // Prepare custom message
    struct custom_msg *cm = (struct custom_msg *)NLMSG_DATA(nlh);
    strncpy(cm->message, test_message, sizeof(cm->message) - 1);
    cm->message[sizeof(cm->message) - 1] = '\0'; // Ensure null termination
    cm->value = test_value;

    printf("Testing: Sending '%s' \n", cm->message);

    // Setup message structure for sending
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)dest_addr;
    msg.msg_namelen = sizeof(struct sockaddr_nl);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send message to kernel
    ret = sendmsg(sock_fd, &msg, 0);
    assert(ret >= 0 && "Failed to send message to kernel");

    // Receive response from kernel
    ret = recvmsg(sock_fd, &msg, 0);
    assert(ret >= 0 && "Failed to receive response from kernel");

    // Parse and validate response
    struct custom_msg *response = (struct custom_msg *)NLMSG_DATA(nlh);
    
    // Assertions to verify proper functionality
    assert(response != NULL && "Received NULL response from kernel");
    assert(strlen(response->message) > 0 && "Empty response message from kernel");
    assert(response->value == test_value + 1 && "Kernel didn't increment value correctly");
    
    printf("Received '%s'\n", response->message);

    printf("Test passed ✅\n");

    free(nlh);
}

// Test different scenarios
void run_tests(int sock_fd, struct sockaddr_nl *dest_addr){
    printf("\n=== Starting Netlink Module Tests ===\n");

    printf("\n1. Test:\n");
    test_netlink_communication(sock_fd, dest_addr, "Hello from Kernel!", 10);

    printf("\n=== All tests passed! ===\n");
}

int main(){
    struct sockaddr_nl src_addr, dest_addr;
    int sock_fd = -1;

    printf("Netlink Kernel Module Test Program\n");
    printf("==================================\n");

    // Setup Netlink connection
    sock_fd = setup_netlink_socket(&src_addr, &dest_addr);
    printf("Netlink socket setup successful (FD: %d)\n", sock_fd);

    // Run comprehensive tests
    run_tests(sock_fd, &dest_addr);

    // Cleanup
    if (sock_fd >= 0) {
        close(sock_fd);
    }

    return EXIT_SUCCESS;
}