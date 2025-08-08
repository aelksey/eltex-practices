#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#define BUFFER_SIZE 4096
#define MAX_CLIENTS 10

// Function prototypes
void handle_client(int client_fd);
int calculate(int a, int b, char op);
void save_file(int client_fd, const char *filename);
void display_file(const char *filename);

int nclients = 0;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int calculate(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if(b == 0) return 0;
            return a / b;
        default: return 0;
    }
}

void display_file(const char *filename) {
    printf("\n=== File Content: %s ===\n", filename);
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    
    fclose(file);
    printf("\n=== End of File ===\n\n");
}

void save_file(int client_fd, const char *filename) {
    int fd;
    char buff[BUFFER_SIZE];
    ssize_t bytes_recv;
    int file_size = 0;
    
    fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("File create error");
        send(client_fd, "ERROR", 5, 0);
        return;
    }
    
    // Tell client we're ready
    send(client_fd, "READY", 5, 0);
    
    while (1) {
        bytes_recv = recv(client_fd, buff, BUFFER_SIZE, 0);
        if (bytes_recv <= 0) {
            break;
        }
        
        // Check for end marker
        if (bytes_recv >= 3 && strncmp(buff + bytes_recv - 3, "END", 3) == 0) {
            if (bytes_recv > 3) {
                write(fd, buff, bytes_recv - 3);
                file_size += bytes_recv - 3;
            }
            break;
        }
        
        write(fd, buff, bytes_recv);
        file_size += bytes_recv;
    }
    
    close(fd);
    printf("Received file %s (%d bytes)\n", filename, file_size);
    display_file(filename);  // Display the file contents
    send(client_fd, "OK", 2, 0);
}

int main(int argc, char *argv[]) {
    int sockfd, new_sockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int opt = 1;
    int max_fd, activity, i, valread;
    fd_set readfds;
    int client_sockets[MAX_CLIENTS];
    
    // Initialize client sockets
    for (i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = 0;
    }
    
    printf("TCP Server with I/O Multiplexing (select)\n");
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // Create master socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        error("ERROR on setsockopt");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    if (listen(sockfd, 5) < 0) {
        error("ERROR on listen");
    }
    
    clilen = sizeof(cli_addr);
    printf("Server started on port %d. Waiting for connections...\n", portno);

    while (1) {
        // Clear the socket set
        FD_ZERO(&readfds);
        
        // Add master socket to set
        FD_SET(sockfd, &readfds);
        max_fd = sockfd;
        
        // Add child sockets to set
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0) {
                FD_SET(client_sockets[i], &readfds);
            }
            if (client_sockets[i] > max_fd) {
                max_fd = client_sockets[i];
            }
        }
        
        // Wait for activity on any socket
        activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            perror("select error");
        }
        
        // If activity on master socket, new connection
        if (FD_ISSET(sockfd, &readfds)) {
            new_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            if (new_sockfd < 0) {
                perror("accept");
                continue;
            }
            
            // Add new socket to array
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_sockfd;
                    nclients++;
                    
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
                    printf("New connection from %s, socket fd %d\n", client_ip, new_sockfd);
                    printf("%d user(s) on-line\n", nclients);
                    break;
                }
            }
            
            if (i == MAX_CLIENTS) {
                printf("Too many connections, rejecting\n");
                close(new_sockfd);
            }
        }
        
        // Check other sockets for IO
        for (i = 0; i < MAX_CLIENTS; i++) {
            int client_fd = client_sockets[i];
            
            if (client_fd > 0 && FD_ISSET(client_fd, &readfds)) {
                char buffer[BUFFER_SIZE];
                int valread = recv(client_fd, buffer, BUFFER_SIZE, 0);
                
                if (valread == 0) {
                    // Client disconnected
                    getpeername(client_fd, (struct sockaddr*)&cli_addr, &clilen);
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
                    
                    printf("Client %s disconnected, socket fd %d\n", client_ip, client_fd);
                    close(client_fd);
                    client_sockets[i] = 0;
                    nclients--;
                    printf("%d user(s) on-line\n", nclients);
                } else {
                    buffer[valread] = '\0';
                    
                    if (strncmp(buffer, "file:", 5) == 0) {
                        char filename[256];
                        sscanf(buffer+5, "%255s", filename);
                        printf("Receiving file: %s\n", filename);
                        save_file(client_fd, filename);
                    } 
                    else if (strncmp(buffer, "calc:", 5) == 0) {
                        char op;
                        int a, b, result;
                        if (sscanf(buffer+5, "%d %c %d", &a, &op, &b) != 3) {
                            send(client_fd, "Invalid calculation format", 26, 0);
                            continue;
                        }
                        
                        result = calculate(a, b, op);
                        char response[BUFFER_SIZE];
                        snprintf(response, BUFFER_SIZE, "Result: %d", result);
                        send(client_fd, response, strlen(response), 0);
                    } 
                    else if (strncmp(buffer, "quit", 4) == 0) {
                        printf("Client requested disconnect\n");
                        close(client_fd);
                        client_sockets[i] = 0;
                        nclients--;
                        printf("%d user(s) on-line\n", nclients);
                    } 
                    else {
                        send(client_fd, "Unknown command", 15, 0);
                    }
                }
            }
        }
    }
    
    close(sockfd);
    return 0;
}