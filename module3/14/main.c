#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define MAX_PACKET_SIZE 65536
#define DUMP_FILE "dump.bin"
#define ASCII_DISPLAY_LEN 16  // Number of ASCII characters to display per line

void error(const char *msg) {
    perror(msg);
    exit(1);
}

// Function to print packet information with ASCII payload
void print_packet_info(const struct ip *ip_header, const struct udphdr *udp_header, 
                       const char *payload, int payload_len, FILE *dump_file) {
    time_t now;
    time(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("[%s] Packet captured:\n", timestamp);
    printf("Source: %s:%d\n", inet_ntoa(ip_header->ip_src), ntohs(udp_header->uh_sport));
    printf("Destination: %s:%d\n", inet_ntoa(ip_header->ip_dst), ntohs(udp_header->uh_dport));
    printf("Length: %d bytes\n", ntohs(udp_header->uh_ulen));
    
    // Hex dump of payload
    printf("\nHex View:\n");
    for (int i = 0; i < payload_len; i++) {
        printf("%02x ", (unsigned char)payload[i]);
        if ((i + 1) % ASCII_DISPLAY_LEN == 0 || i == payload_len - 1) {
            // Print ASCII representation
            printf("  |  ");
            int line_start = (i / ASCII_DISPLAY_LEN) * ASCII_DISPLAY_LEN;
            for (int j = line_start; j <= i; j++) {
                if (isprint((unsigned char)payload[j])) {
                    putchar(payload[j]);
                } else {
                    putchar('.');
                }
            }
            putchar('\n');
        }
    }
    
    // Write raw packet data to binary file
    fwrite(ip_header, ntohs(ip_header->ip_len), 1, dump_file);
    fflush(dump_file);
    
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port_to_sniff>\n", argv[0]);
        exit(1);
    }
    
    int port_to_sniff = atoi(argv[1]);
    int raw_socket;
    struct sockaddr_in saddr;
    socklen_t saddr_size = sizeof(saddr);
    char buffer[MAX_PACKET_SIZE];
    
    // Create raw socket
    raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_socket < 0) {
        error("Raw socket creation failed");
    }
    
    // Open file for packet dumping
    FILE *dump_file = fopen(DUMP_FILE, "wb");
    if (!dump_file) {
        error("Failed to open dump file");
    }
    
    printf("Sniffer started. Capturing UDP packets for port %d...\n", port_to_sniff);
    printf("Packet dump will be saved to %s\n", DUMP_FILE);
    printf("Press Ctrl+C to stop...\n\n");
    
    while (1) {
        // Receive packet
        int packet_size = recvfrom(raw_socket, buffer, MAX_PACKET_SIZE, 0, 
                                 (struct sockaddr *)&saddr, &saddr_size);
        if (packet_size < 0) {
            error("Packet receive failed");
        }
        
        // Parse IP header
        struct ip *ip_header = (struct ip *)buffer;
        unsigned int ip_header_len = ip_header->ip_hl * 4;
        
        // Check if it's UDP
        if (ip_header->ip_p == IPPROTO_UDP) {
            // Parse UDP header
            struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header_len);
            
            // Check if packet is destined to our port
            if (ntohs(udp_header->uh_dport) == port_to_sniff) {
                // Get payload (skip UDP header)
                char *payload = buffer + ip_header_len + sizeof(struct udphdr);
                int payload_len = ntohs(udp_header->uh_ulen) - sizeof(struct udphdr);
                
                // Print packet info and save to dump file
                print_packet_info(ip_header, udp_header, payload, payload_len, dump_file);
            }
        }
    }
    
    close(raw_socket);
    fclose(dump_file);
    return 0;
}