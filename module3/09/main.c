#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define FILENAME "numbers.txt"
#define SEM_NAME "/file_semaphore"
#define MAX_NUMBERS_PER_LINE 20
#define MAX_NUMBER_VALUE 1000
#define MAX_BATCHES 3

void generate_numbers(int fd) {
    srand(time(NULL) ^ getpid());
    int lines = rand() % 10 + 5; // Generate 5-14 lines
    
    for (int i = 0; i < lines; i++) {
        int numbers_in_line = rand() % MAX_NUMBERS_PER_LINE + 1;
        
        char line[256] = {0};
        for (int j = 0; j < numbers_in_line; j++) {
            int num = rand() % MAX_NUMBER_VALUE;
            char num_str[16];
            sprintf(num_str, "%d ", num);
            strcat(line, num_str);
        }
        strcat(line, "\n");
        
        write(fd, line, strlen(line));
        printf("Parent wrote: %s", line);
    }
}

void process_numbers(int fd) {
    lseek(fd, 0, SEEK_SET); // Rewind to beginning
    
    char buffer[1024];
    ssize_t bytes_read;
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[bytes_read] = '\0';
        
        char *line = strtok(buffer, "\n");
        while (line != NULL) {
            if (strlen(line) > 0 && line[0] != ' ') { // Skip already processed lines
                printf("Child processing: %s\n", line);
                
                int min = MAX_NUMBER_VALUE, max = -1;
                char *token = strtok(line, " ");
                while (token != NULL) {
                    int num = atoi(token);
                    if (num < min) min = num;
                    if (num > max) max = num;
                    token = strtok(NULL, " ");
                }
                
                printf("Min: %d, Max: %d\n\n", min, max);
                
                // Mark line as processed by overwriting with spaces
                off_t current_pos = lseek(fd, 0, SEEK_CUR);
                lseek(fd, current_pos - bytes_read + (line - buffer), SEEK_SET);
                for (size_t i = 0; i < strlen(line); i++) {
                    write(fd, " ", 1);
                }
                write(fd, "\n", 1);
                lseek(fd, current_pos, SEEK_SET);
            }
            line = strtok(NULL, "\n");
        }
    }
}

int main() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        sem_unlink(SEM_NAME);
        return 1;
    }

    int fd = open(FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        close(fd);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return 1;
    }

    if (pid > 0) { // Parent process
        for (int i = 0; i < MAX_BATCHES; i++) {
            sem_wait(sem);
            generate_numbers(fd);
            sem_post(sem);
            sleep(1); // Give child time to process
        }
        
        // Write termination marker
        sem_wait(sem);
        write(fd, "END\n", 4);
        sem_post(sem);
        
        wait(NULL); // Wait for child to finish
        close(fd);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        printf("Parent: Done.\n");
    } else { // Child process
        int batches_processed = 0;
        while (1) {
            sem_wait(sem);
            
            // Check for termination marker
            lseek(fd, 0, SEEK_SET);
            char buf[5] = {0};
            read(fd, buf, 4);
            if (strcmp(buf, "END\n") == 0) {
                sem_post(sem);
                break;
            }
            
            process_numbers(fd);
            sem_post(sem);
            usleep(500000); // Sleep for 0.5s to prevent busy waiting
        }
        
        close(fd);
        printf("Child: Done.\n");
    }

    return 0;
}