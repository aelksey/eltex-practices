#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#define PROMPT ">:"

#define SERVER_PRIORITY 10
#define MAX_MSG_SIZE 256

typedef struct {
    long mtype;
    int sender;
    char mtext[MAX_MSG_SIZE];
} Message;

int msgqid;
int client_priority;
bool running = true;

void print_prompt() {
    printf("%s",PROMPT);
    fflush(stdout);
}

void* receive_messages(void *arg) {
    Message msg;
    while (running) {
        if (msgrcv(msgqid, &msg, sizeof(msg) - sizeof(long), client_priority, IPC_NOWAIT) == -1) {
            usleep(100000); // sleep for 100ms to avoid busy waiting
            continue;
        }
        if (msg.sender == SERVER_PRIORITY) {
            printf("\n[Server]: %s\n", msg.mtext);
        } else {
            printf("\n[Client %d]: %s\n", msg.sender, msg.mtext);
        }
        print_prompt();
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <client_priority>\n", argv[0]);
        printf("Client priority should be 20, 30, 40, etc. \n");
        return 1;
    }

    int client_num = atoi(argv[1]);
    if (client_num < 2) {
        printf("Client number must be 2 or higher\n");
        return 1;
    }

    client_priority = (client_num);

    key_t key = ftok("groupchat", 65);
    msgqid = msgget(key, 0666);
    if (msgqid == -1) {
        perror("msgget");
        return 1;
    }

    // Register with server
    Message reg_msg;
    reg_msg.mtype = SERVER_PRIORITY;
    reg_msg.sender = client_priority;
    strcpy(reg_msg.mtext, "connect");
    msgsnd(msgqid, &reg_msg, sizeof(reg_msg) - sizeof(long), 0);

    printf("Client %d started (priority %d)\n", client_num, client_priority);
    printf("Available commands:\n");
    printf("  /msg <priority> <message> - Send private message\n");
    printf("  shutdown - Quit the client\n");

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    char input[MAX_MSG_SIZE];
    while (running) {
        print_prompt();
        fgets(input, MAX_MSG_SIZE, stdin);
        input[strcspn(input, "\n")] = 0; // remove newline

        if (strcmp(input, "shutdown") == 0) {
            running = false;
            // Send shutdown message to server
            Message msg;
            msg.mtype = SERVER_PRIORITY;
            msg.sender = client_priority;
            strcpy(msg.mtext, "shutdown");
            msgsnd(msgqid, &msg, sizeof(msg) - sizeof(long), 0);
            break;
        }

        Message msg;
        msg.mtype = SERVER_PRIORITY;
        msg.sender = client_priority;
        strcpy(msg.mtext, input);
        msgsnd(msgqid, &msg, sizeof(msg) - sizeof(long), 0);
    }

    pthread_join(recv_thread, NULL);
    printf("Client %d exiting...\n", client_num);
    return 0;
}