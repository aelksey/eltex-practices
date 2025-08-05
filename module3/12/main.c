#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_MSG_LEN 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <my_port> <other_ip> <other_port>\n", argv[0]);
        exit(1);
    }

    int my_port = atoi(argv[1]);
    char *other_ip = argv[2];
    int other_port = atoi(argv[3]);

    int sockfd;
    struct sockaddr_in my_addr, other_addr;
    socklen_t addr_len = sizeof(other_addr);
    char buffer[MAX_MSG_LEN];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error("Socket creation failed");
    }

    // Configure my address
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(my_port);

    // Bind socket to my port
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr))) {
        error("Bind failed");
    }

    // Configure other client's address
    memset(&other_addr, 0, sizeof(other_addr));
    other_addr.sin_family = AF_INET;
    other_addr.sin_port = htons(other_port);
    if (inet_pton(AF_INET, other_ip, &other_addr.sin_addr) <= 0) {
        error("Invalid IP address");
    }

    printf("Chat started. Type messages and press Enter to send.\n");

    pid_t pid = fork();
    if (pid < 0) {
        error("Fork failed");
    }

    if (pid == 0) {
        // Child process: handles sending messages
        while (1) {
            printf("You: ");
            fgets(buffer, MAX_MSG_LEN, stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline

            if (sendto(sockfd, buffer, strlen(buffer), 0, 
                       (struct sockaddr *)&other_addr, addr_len) < 0) {
                error("Send failed");
            }
        }
    } else {
        // Parent process: handles receiving messages
        while (1) {
            int recv_len = recvfrom(sockfd, buffer, MAX_MSG_LEN, 0, 
                                    (struct sockaddr *)&other_addr, &addr_len);
            if (recv_len < 0) {
                error("Receive failed");
            }
            buffer[recv_len] = '\0';
            printf("\nOther: %s\nYou: ", buffer);
            fflush(stdout); // Ensure prompt appears
        }
    }

    close(sockfd);
    return 0;
}