#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "contact.h"
#include "book.h"
#include "file.h"

#define SUCESS 0
#define ERROR 1
#define MODE_PROG_READ O_CREAT | O_APPEND | O_RDWR
#define MODE_PROG_WRITE O_CREAT | O_RDWR
#define UP S_IRUSR | S_IWUSR
#define GP S_IRGRP | S_IWGRP
#define OP S_IRGRP | S_IWGRP
#define MODE_OS UP | GP | OP

int read_book(book_t *book,const char *filename,int *id){
    
    int fd;
    int count = 0;
    contact_t current;
    ssize_t bytes_read;

    fd = open(filename, MODE_PROG_READ,MODE_OS);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    // Loop to read structs until end of file
    while ((bytes_read = read(fd, &current, sizeof(contact_t))) > 0) {
        if (bytes_read == sizeof(contact_t)) {
            book->contacts[count] = current;
            book->size = (count + 1);
            *id = (count + 1);
            count++;
        } else {
            fprintf(stderr, "Warning: Partial read or error encountered. Bytes read: %zd\n", bytes_read);
            break; 
        }
    }

    //printf("Read book size:%d\n",book->size);
    //printf("Id count:%d\n",*id);

    if (bytes_read == -1) {
        perror("Error reading from file");
    }

    close(fd);

    return 0;
}

int write_book(book_t *book,const char *filename){

    int fd = open(filename, MODE_PROG_WRITE, MODE_OS);
    if (fd == -1) {
        perror("Error opening file for writing");
        return -1;
    }

    ssize_t bytes_written = write(fd, book->contacts, book->size * sizeof(contact_t));

    //printf("Write book size:%d\n",book->size);

    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
} 
