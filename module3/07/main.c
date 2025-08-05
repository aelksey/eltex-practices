#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>

#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE + 10)

// Termination message with known priority
#define TERMINATION_PRIORITY 255
#define TERMINATION_MESSAGE "EXIT"

int main(int argc, char *argv[]) {
    mqd_t qd_receive, qd_send;
    char in_buffer[MSG_BUFFER_SIZE];
    char out_buffer[MSG_BUFFER_SIZE];
    char qname_receive[32];
    char qname_send[32];
    unsigned int prio;
    
    if (argc != 3) {
        printf("Usage: %s <my_queue_name> <other_queue_name>\n", argv[0]);
        printf("Example: %s chat1 chat2\n", argv[0]);
        exit(1);
    }
    
    // Ensure queue names start with a slash
    snprintf(qname_receive, sizeof(qname_receive), "/%s", argv[1]);
    snprintf(qname_send, sizeof(qname_send), "/%s", argv[2]);
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    
    // Create and open both queues
    if ((qd_receive = mq_open(qname_receive, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror("mq_open (receive)");
        exit(1);
    }
    
    // Open the send queue with CREATE flag
    if ((qd_send = mq_open(qname_send, O_WRONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        perror("mq_open (send)");
        mq_close(qd_receive);
        exit(1);
    }
    
    printf("Chat started. Type messages (type 'exit' to quit):\n");
    
    // First message is sent by the user
    while (1) {
        // Get user input
        printf("Enter your message: ");
        fgets(out_buffer, MAX_MSG_SIZE, stdin);
        
        // Remove newline character
        out_buffer[strcspn(out_buffer, "\n")] = '\0';
        
        // Check if user wants to exit
        if (strcmp(out_buffer, "exit") == 0) {
            // Send termination message
            if (mq_send(qd_send, TERMINATION_MESSAGE, strlen(TERMINATION_MESSAGE) + 1, TERMINATION_PRIORITY) == -1) {
                perror("mq_send (termination)");
            }
            printf("Ending chat...\n");
            break;
        }
        
        // Send message with normal priority (0)
        if (mq_send(qd_send, out_buffer, strlen(out_buffer) + 1, 0) == -1) {
            perror("mq_send");
            continue;
        }
        
        // Then wait for response
        printf("Waiting for message...\n");
        if (mq_receive(qd_receive, in_buffer, MSG_BUFFER_SIZE, &prio) == -1) {
            perror("mq_receive");
            break;
        }
        
        // Check for termination message
        if (prio == TERMINATION_PRIORITY && strcmp(in_buffer, TERMINATION_MESSAGE) == 0) {
            printf("Other party has ended the chat.\n");
            break;
        }
        
        printf("Received: %s\n", in_buffer);
    }
    
    // Cleanup
    mq_close(qd_send);
    mq_close(qd_receive);
    mq_unlink(qname_receive);
    
    return 0;
}