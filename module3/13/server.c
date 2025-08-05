#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 4096  // Increased buffer size for better performance

// Function prototypes
void dostuff(int sock);
void error(const char *msg);
void printusers(void);
int calculate(int a, int b, char op);
void display_file(const char *filename);

int nclients = 0;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void printusers(void) {
    if(nclients) {
        printf("%d user(s) on-line\n", nclients);
    } else {
        printf("No users online\n");
    }
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

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int opt = 1;

    printf("TCP Server (Improved with file transfer)\n");
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

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

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    
    printf("Server started on port %d. Waiting for connections...\n", portno);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;
        }
        
        nclients++;
        
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cli_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("New connection from %s\n", client_ip);
        printusers();

        pid_t pid = fork();
        if (pid < 0) {
            perror("ERROR on fork");
            close(newsockfd);
            nclients--;
            continue;
        }

        if (pid == 0) {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
    }
    
    close(sockfd);
    return 0;
}

void dostuff(int sock) {
    int bytes_recv;
    char buff[BUFFER_SIZE];
    char filename[256];
    int fd;
    int file_size = 0;
    int ready_received = 0;

    while(1) {
        bzero(buff, BUFFER_SIZE);
        bytes_recv = recv(sock, buff, BUFFER_SIZE-1, 0);
        if (bytes_recv < 0) {
            perror("ERROR reading from socket");
            break;
        }
        if (bytes_recv == 0) {
            printf("Client disconnected\n");
            break;
        }
        buff[bytes_recv] = '\0';
        
        if (!ready_received && strncmp(buff, "file:", 5) == 0) {
            // File transfer initialization
            sscanf(buff+5, "%255s", filename);
            printf("Receiving file: %s\n", filename);
            
            fd = open(filename, O_WRONLY | O_CREAT, 0644);
            if (fd < 0) {
                perror("File create error");
                send(sock, "ERROR", 5, 0);
                continue;
            }
            
            send(sock, "READY", 5, 0);
            ready_received = 1;
            continue;
        }
        
        if (ready_received) {
            // File data receiving mode
            if (strncmp(buff, "END", 3) == 0) {
                printf("Received complete file (%d bytes)\n", file_size);
                close(fd);
                display_file(filename);
                send(sock, "OK", 2, 0);
                ready_received = 0;
                file_size = 0;
                continue;
            }
            
            int bytes_written = write(fd, buff, bytes_recv);
            if (bytes_written < 0) {
                perror("Error writing to file");
                close(fd);
                send(sock, "ERROR", 5, 0);
                ready_received = 0;
                continue;
            }
            file_size += bytes_written;
        }
        else if (strncmp(buff, "calc:", 5) == 0) {
            // Calculation mode
            char op;
            int a, b, result;
            if (sscanf(buff+5, "%d %c %d", &a, &op, &b) != 3) {
                send(sock, "Invalid calculation format", 26, 0);
                continue;
            }
            
            result = calculate(a, b, op);
            snprintf(buff, BUFFER_SIZE, "Result: %d", result);
            send(sock, buff, strlen(buff), 0);
        }
        else if (strncmp(buff, "quit", 4) == 0) {
            printf("Client disconnected\n");
            break;
        }
        else {
            send(sock, "Unknown command", 15, 0);
        }
    }
    
    nclients--;
    printusers();
    close(sock);
}