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
#include <pthread.h>
#include <time.h>
#include <uuid/uuid.h>
#include <sys/select.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 9090
#define MAX_CLIENTS 100
#define CLIENT_TIMEOUT 300 // 5 minutes in seconds
#define CLEANUP_INTERVAL 60 // 1 minute in seconds

typedef struct {
    uint32_t ip;
    uint16_t port;
    char client_id[37]; // UUID string (36 chars + null terminator)
    int counter;
    time_t last_active;
    int socket_fd; // Track client socket fd if using TCP in future
} ClientInfo;

// Global variables
ClientInfo clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int raw_sock;
volatile sig_atomic_t running = 1;
fd_set read_fds;
int max_fd = 0;

// Function prototypes
void handle_signal(int sig);
unsigned short checksum(void *b, int len);
int find_client(uint32_t ip, uint16_t port, const char *client_id);
void add_client(uint32_t ip, uint16_t port, const char *client_id, int sock_fd);
void remove_client(uint32_t ip, uint16_t port, const char *client_id);
void send_response(uint32_t dest_ip, uint16_t dest_port, const char *client_id, 
                  const char *message, int counter);
void *client_cleanup_thread(void *arg);
void process_packet(char *buffer, ssize_t recv_len);
void initialize_server();
void cleanup_resources();

// Signal handler
void handle_signal(int sig) {
    running = 0;
}

// Checksum calculation
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

// Client management functions
int find_client(uint32_t ip, uint16_t port, const char *client_id) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].ip == ip && 
            clients[i].port == port && 
            strcmp(clients[i].client_id, client_id) == 0) {
            return i;
        }
    }
    return -1;
}

void add_client(uint32_t ip, uint16_t port, const char *client_id, int sock_fd) {
    pthread_mutex_lock(&clients_mutex);
    
    int index = find_client(ip, port, client_id);
    if (index != -1) {
        clients[index].last_active = time(NULL);
        pthread_mutex_unlock(&clients_mutex);
        return;
    }

    if (client_count >= MAX_CLIENTS) {
        // Find and replace the least recently used client
        time_t oldest = time(NULL);
        int oldest_index = 0;
        for (int i = 0; i < client_count; i++) {
            if (clients[i].last_active < oldest) {
                oldest = clients[i].last_active;
                oldest_index = i;
            }
        }
        index = oldest_index;
        
        // Clean up socket fd if it exists
        if (clients[index].socket_fd > 0) {
            FD_CLR(clients[index].socket_fd, &read_fds);
            close(clients[index].socket_fd);
        }
    } else {
        index = client_count++;
    }

    clients[index].ip = ip;
    clients[index].port = port;
    strncpy(clients[index].client_id, client_id, sizeof(clients[index].client_id));
    clients[index].counter = 0;
    clients[index].last_active = time(NULL);
    clients[index].socket_fd = sock_fd;

    // Update max_fd if using socket_fd
    if (sock_fd > 0 && sock_fd > max_fd) {
        max_fd = sock_fd;
    }

    printf("New client connected: %s:%d (ID: %s)\n", 
           inet_ntoa(*(struct in_addr *)&ip), port, client_id);
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(uint32_t ip, uint16_t port, const char *client_id) {
    pthread_mutex_lock(&clients_mutex);
    int index = find_client(ip, port, client_id);
    if (index == -1) {
        pthread_mutex_unlock(&clients_mutex);
        return;
    }

    printf("Client disconnected: %s:%d (ID: %s)\n", 
           inet_ntoa(*(struct in_addr *)&ip), port, client_id);

    // Clean up socket fd if it exists
    if (clients[index].socket_fd > 0) {
        FD_CLR(clients[index].socket_fd, &read_fds);
        close(clients[index].socket_fd);
    }

    // Shift array to remove the client
    for (int i = index; i < client_count - 1; i++) {
        clients[i] = clients[i + 1];
    }
    client_count--;
    pthread_mutex_unlock(&clients_mutex);
}

// Network functions
void send_response(uint32_t dest_ip, uint16_t dest_port, const char *client_id, 
                  const char *message, int counter) {
    char buffer[BUFFER_SIZE];
    struct iphdr *ip_header = (struct iphdr *)buffer;
    struct udphdr *udp_header = (struct udphdr *)(buffer + sizeof(struct iphdr));
    char *data = buffer + sizeof(struct iphdr) + sizeof(struct udphdr);

    // Prepare the response message with client ID and counter
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "%s %d ", message, counter);

    // Build IP header
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(response) + 1;
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 255;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->check = 0;
    ip_header->saddr = inet_addr("127.0.0.1");
    ip_header->daddr = dest_ip;
    ip_header->check = checksum(ip_header, sizeof(struct iphdr));

    // Build UDP header
    udp_header->source = htons(SERVER_PORT);
    udp_header->dest = htons(dest_port);
    udp_header->len = htons(sizeof(struct udphdr) + strlen(response) + 1);
    udp_header->check = 0;

    // Copy data
    strcpy(data, response);

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

// Thread functions
void *client_cleanup_thread(void *arg) {
    while (running) {
        sleep(CLEANUP_INTERVAL);
        time_t now = time(NULL);
        
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < client_count; i++) {
            if (difftime(now, clients[i].last_active) > CLIENT_TIMEOUT) {
                printf("Removing inactive client: %s:%d (ID: %s)\n", 
                      inet_ntoa(*(struct in_addr *)&clients[i].ip), 
                      clients[i].port, clients[i].client_id);
                
                // Clean up socket fd if it exists
                if (clients[i].socket_fd > 0) {
                    FD_CLR(clients[i].socket_fd, &read_fds);
                    close(clients[i].socket_fd);
                }
                
                // Shift array to remove the client
                for (int j = i; j < client_count - 1; j++) {
                    clients[j] = clients[j + 1];
                }
                client_count--;
                i--; // Re-check this index as we shifted the array
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

// Packet processing
void process_packet(char *buffer, ssize_t recv_len) {
    // Parse IP header
    struct iphdr *ip_header = (struct iphdr *)buffer;
    if (ip_header->protocol != IPPROTO_UDP) return;

    // Parse UDP header
    struct udphdr *udp_header = (struct udphdr *)(buffer + (ip_header->ihl * 4));
    char *data = buffer + (ip_header->ihl * 4) + sizeof(struct udphdr);
    int data_len = recv_len - (ip_header->ihl * 4) - sizeof(struct udphdr);

    // Check if it's for our server port
    if (ntohs(udp_header->dest) != SERVER_PORT) return;

    // Null-terminate the data
    if (data_len >= BUFFER_SIZE) data_len = BUFFER_SIZE - 1;
    data[data_len] = '\0';

    uint32_t client_ip = ip_header->saddr;
    uint16_t client_port = ntohs(udp_header->source);

    printf("Received from %s:%d: %s\n", 
           inet_ntoa(*(struct in_addr *)&client_ip), 
           client_port, data);

    // Extract client ID from message (format: "ID:<uuid> <message>")
    char client_id[37] = {0};
    char *message = data;
    
    if (strncmp(data, "ID:", 3) == 0) {
        char *space = strchr(data, ' ');
        if (space != NULL) {
            size_t id_len = space - data - 3; // Length of UUID part
            if (id_len > 36) id_len = 36;
            strncpy(client_id, data + 3, id_len);
            client_id[id_len] = '\0';
            message = space + 1;
        }
    }

    // If no ID found, generate a new one
    if (strlen(client_id) == 0) {
        uuid_t uuid;
        uuid_generate_random(uuid);
        uuid_unparse(uuid, client_id);
    }

    // Check for shutdown message
    if (strcmp(message, "SHUTDOWN") == 0) {
        remove_client(client_ip, client_port, client_id);
        return;
    }

    // Add/update client (passing 0 as socket_fd since we're using raw sockets)
    add_client(client_ip, client_port, client_id, 0);

    // Find client and increment counter
    pthread_mutex_lock(&clients_mutex);
    int client_index = find_client(client_ip, client_port, client_id);
    if (client_index != -1) {
        clients[client_index].counter++;
        clients[client_index].last_active = time(NULL);
        pthread_mutex_unlock(&clients_mutex);
        send_response(client_ip, client_port, client_id, message, clients[client_index].counter);
    } else {
        pthread_mutex_unlock(&clients_mutex);
    }
}

// Server initialization
void initialize_server() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

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

    // Initialize fd set
    FD_ZERO(&read_fds);
    FD_SET(raw_sock, &read_fds);
    max_fd = raw_sock;

    // Start client cleanup thread
    pthread_t cleanup_thread;
    if (pthread_create(&cleanup_thread, NULL, client_cleanup_thread, NULL) != 0) {
        perror("pthread_create failed");
        close(raw_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server running. Waiting for messages...\n");
}

// Resource cleanup
void cleanup_resources() {
    printf("Shutting down server...\n");
    
    // Close all client sockets and clear from fd set
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket_fd > 0) {
            FD_CLR(clients[i].socket_fd, &read_fds);
            close(clients[i].socket_fd);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    close(raw_sock);
}

// Main server loop
int main() {
    initialize_server();

    while (running) {
        fd_set tmp_fds = read_fds;
        struct timeval tv = {.tv_sec = 1, .tv_usec = 0}; // 1 second timeout
        
        int activity = select(max_fd + 1, &tmp_fds, NULL, NULL, &tv);
        
        if (activity < 0 && running) {
            perror("select error");
            continue;
        }
        
        if (activity > 0 && FD_ISSET(raw_sock, &tmp_fds)) {
            char buffer[BUFFER_SIZE];
            struct sockaddr_in client_addr;
            socklen_t addr_len = sizeof(client_addr);
            
            ssize_t recv_len = recvfrom(raw_sock, buffer, BUFFER_SIZE, 0, 
                                       (struct sockaddr *)&client_addr, &addr_len);
            if (recv_len < 0) {
                if (running) perror("recvfrom failed");
                continue;
            }
            
            process_packet(buffer, recv_len);
        }
    }

    cleanup_resources();
    return 0;
}