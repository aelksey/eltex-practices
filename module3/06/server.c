#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>

#define SERVER_PRIORITY 10
#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 256

typedef struct {
    long mtype;
    int sender;
    char mtext[MAX_MSG_SIZE];
} Message;

int msgqid;
bool active_clients[MAX_CLIENTS] = {false};
int client_priorities[MAX_CLIENTS] = {0};

void cleanup() {
    msgctl(msgqid, IPC_RMID, NULL);
    printf("Server shutting down. Message queue removed.\n");
    exit(0);
}

void handle_signal(int sig) {
    cleanup();
}

void forward_to_all(Message msg, int exclude_sender) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (active_clients[i] && client_priorities[i] != exclude_sender) {
            msg.mtype = client_priorities[i];
            if (msgsnd(msgqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                perror("msgsnd");
            }
        }
    }
}

void forward_to_client(Message msg, int recipient_priority) {
    msg.mtype = recipient_priority;
    if (msgsnd(msgqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
    }
}

int parse_command(const char* text, int* recipient) {
    if (strncmp(text, "/msg ", 5) == 0) {
        char* endptr;
        long priority = strtol(text + 5, &endptr, 10);
        if (endptr != text + 5 && *endptr == ' ' && priority >= 20 && priority % 10 == 0) {
            *recipient = (int)priority;
            return 1; // private message
        }
    }
    return 0; // public message
}

int main() {
    key_t key = ftok("groupchat", 65);
    msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(1);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Server started. Message queue ID: %d\n", msgqid);
    printf("Waiting for messages...\n");

    Message msg;
    while (1) {
        if (msgrcv(msgqid, &msg, sizeof(msg) - sizeof(long), SERVER_PRIORITY, 0) == -1) {
            perror("msgrcv");
            continue;
        }

        int client_index = -1;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_priorities[i] == msg.sender) {
                client_index = i;
                break;
            }
        }

        if (strcmp(msg.mtext, "shutdown") == 0) {
            if (client_index != -1) {
                printf("Client %d disconnected.\n", msg.sender);
                active_clients[client_index] = false;
            }
            continue;
        }

        if (client_index == -1) {
            // New client registration
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (!active_clients[i]) {
                    client_index = i;
                    client_priorities[i] = msg.sender;
                    active_clients[i] = true;
                    printf("New client connected: %d\n", msg.sender);
                    break;
                }
            }
            if (client_index == -1) {
                printf("Max clients reached. Cannot register %d\n", msg.sender);
                continue;
            }
        }

        int recipient = 0;
        int is_private = parse_command(msg.mtext, &recipient);

        if (is_private) {
            // Extract the actual message text after the command
            char* message_start = strchr(msg.mtext + 5, ' ') + 1;
            strcpy(msg.mtext, message_start);
            printf("Forwarding private message from %d to %d: %s\n", 
                   msg.sender, recipient, msg.mtext);
            forward_to_client(msg, recipient);
        } else {
            printf("Broadcasting message from %d: %s\n", msg.sender, msg.mtext);
            forward_to_all(msg, msg.sender);
        }
    }

    return 0;
}