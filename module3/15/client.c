// Same client code from task 15

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#define h_addr h_addr_list[0]

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void send_file(int sock, const char *filename) {
    char buff[2048];
    int fd, bytes_read;
    
    sprintf(buff, "file:%s", filename);
    write(sock, buff, strlen(buff));
    
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    
    if (strcmp(buff, "READY") != 0) {
        printf("Server not ready to receive file\n");
        return;
    }
    
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("File open error");
        write(sock, "ERROR", 5);
        return;
    }
    
    while ((bytes_read = read(fd, buff, sizeof(buff))) > 0) {
        write(sock, buff, bytes_read);
    }
    
    write(sock, "END", 3);
    bzero(buff, sizeof(buff));
    read(sock, buff, sizeof(buff));
    
    if (strcmp(buff, "OK") == 0) {
        printf("File %s sent successfully\n", filename);
    } else {
        printf("File transfer failed\n");
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[2048];

    printf("TCP Client (Improved with file transfer)\n");
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) error("ERROR opening socket");
    
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(my_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    while (1) {
        printf("Enter command (calc:a op b, file:filename, quit): ");
        bzero(buff, sizeof(buff));
        fgets(buff, sizeof(buff)-1, stdin);
        buff[strlen(buff)-1] = '\0'; // Remove newline
        
        if (strcmp(buff, "quit") == 0) {
            write(my_sock, "quit", 4);
            printf("Exit...\n");
            break;
        }
        else if (strncmp(buff, "file:", 5) == 0) {
            send_file(my_sock, buff+5);
        }
        else if (strncmp(buff, "calc:", 5) == 0) {
            write(my_sock, buff, strlen(buff));
            bzero(buff, sizeof(buff));
            read(my_sock, buff, sizeof(buff));
            printf("Server response: %s\n", buff);
        }
        else {
            printf("Unknown command\n");
        }
    }
    
    close(my_sock);
    return 0;
}