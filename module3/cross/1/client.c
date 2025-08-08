#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <fcntl.h>
#include <poll.h>
#include <uuid/uuid.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 9090
#define CLIENT_PORT 9091

int raw_sock;
volatile sig_atomic_t running = 1;
char client_id[37]; // UUID string (36 chars + null terminator)

void handle_signal(int sig) {
    running = 0;
}

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void send_message(uint32_t dest_ip, uint16_t dest_port, const char *message) {
    char buffer[BUFFER_SIZE];
    struct iphdr *ip_header = (struct iphdr *)buffer;
    struct udphdr *udp_header = (struct udphdr *)(buffer + sizeof(struct iphdr));
    char *data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    // Prepend client ID to message
    char full_message[BUFFER_SIZE];
    snprintf(full_message, BUFFER_SIZE, "ID:%s %s", client_id, message);

    // Build IP header
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(full_message) + 1;
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr("127.0.0.1");
    ip_header->daddr = dest_ip;
    ip_header->check = checksum(ip_header, sizeof(struct iphdr));

    // Build UDP header
    udp_header->source = htons(CLIENT_PORT);
    udp_header->dest = htons(dest_port);
    udp_header->len = htons(sizeof(struct udphdr) + strlen(full_message) + 1);
    udp_header->check = 0;

    // Copy data
    strcpy(data, full_message);

    // Send packet
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = dest_ip;

    if (sendto(raw_sock, buffer, ip_header->tot_len, 0, 
               (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto failed");
    }
}

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL failed");
        return;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    // Generate client UUID
    uuid_t uuid;
    uuid_generate_random(uuid);
    uuid_unparse(uuid, client_id);

    // Create raw socket
    if ((raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable IP_HDRINCL to manually construct IP headers
    int one = 1;
    if (setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt failed");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    set_nonblocking(raw_sock);
    set_nonblocking(STDIN_FILENO);

    uint32_t server_ip = inet_addr(argv[1]);

    printf("Client started (ID: %s). Type messages to send to server.\n", client_id);
    printf("Type 'exit' to quit.\n");

    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[1].fd = raw_sock;
    fds[1].events = POLLIN;

    while (running) {
        int ret = poll(fds, 2, 100); // 100ms timeout
        
        if (ret < 0) {
            perror("poll failed");
            break;
        }

        if (ret == 0) {
            // Timeout occurred
            continue;
        }

        // Check for user input
        if (fds[0].revents & POLLIN) {
            char message[BUFFER_SIZE];
            if (fgets(message, BUFFER_SIZE, stdin) == NULL) {
                if (feof(stdin)) {
                    running = 0;
                    break;
                }
                continue;
            }
            
            message[strcspn(message, "\n")] = '\0'; // Remove newline

            if (strcmp(message, "exit") == 0) {
                running = 0;
                break;
            }

            send_message(server_ip, SERVER_PORT, message);
        }

        // Check for server response
        if (fds[1].revents & POLLIN) {
            char buffer[BUFFER_SIZE];
            struct sockaddr_in server_addr;
            socklen_t addr_len = sizeof(server_addr);
            
            ssize_t recv_len = recvfrom(raw_sock, buffer, BUFFER_SIZE, 0, 
                                       (struct sockaddr *)&server_addr, &addr_len);
            if (recv_len < 0) {
                if (errno != EWOULDBLOCK && errno != EAGAIN) {
                    perror("recvfrom failed");
                }
                continue;
            }

            // Parse IP header
            struct iphdr *ip_header = (struct iphdr *)buffer;
            if (ip_header->protocol != IPPROTO_UDP) continue;

            // Parse UDP header
            struct udphdr *udp_header = (struct udphdr *)(buffer + (ip_header->ihl * 4));
            char *data = buffer + (ip_header->ihl * 4) + sizeof(struct udphdr);
            int data_len = recv_len - (ip_header->ihl * 4) - sizeof(struct udphdr);

            // Check if it's for our client port
            if (ntohs(udp_header->dest) != CLIENT_PORT) continue;

            // Null-terminate the data
            if (data_len >= BUFFER_SIZE) data_len = BUFFER_SIZE - 1;
            data[data_len] = '\0';

            printf("Server response: %s\n", data);
        }
    }

    // Send shutdown message
    send_message(server_ip, SERVER_PORT, "SHUTDOWN");
    printf("Client shutting down...\n");

    close(raw_sock);
    return 0;
}