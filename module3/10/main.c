#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>

#define SHM_SIZE 1024

int processed_sets = 0;
int pipefd[2];

typedef struct {
    int count;
    int numbers[SHM_SIZE/sizeof(int) - 3]; // Reserve space for min/max/count
    int min;
    int max;
} shared_data;

void sigint_handler(int sig) {
    char c = 'x';
    write(pipefd[1], &c, 1); // Signal main process through pipe
}

int main() {
    // Create pipe for signal communication
    if (pipe(pipefd)) {
        perror("pipe");
        exit(1);
    }
    
    // Set up signal handler
    signal(SIGINT, sigint_handler);
    
    srand(time(NULL));
    
    // Create shared memory segment
    key_t key = ftok(".", 'S');
    int shmid = shmget(key, sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    while (1) {
        // Check for signal via pipe (non-blocking)
        fd_set rfds;
        struct timeval tv = {0, 0};
        FD_ZERO(&rfds);
        FD_SET(pipefd[0], &rfds);
        
        if (select(pipefd[0] + 1, &rfds, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(pipefd[0], &rfds)) {
                break; // Exit loop on signal
            }
        }
        
        // Attach shared memory
        shared_data *data = (shared_data *)shmat(shmid, NULL, 0);
        if (data == (void *)-1) {
            perror("shmat");
            exit(1);
        }
        
        // Parent process generates random numbers
        data->count = rand() % 20 + 1; // 1-20 numbers
        printf("Generated %d numbers: ", data->count);
        for (int i = 0; i < data->count; i++) {
            data->numbers[i] = rand() % 1000; // 0-999
            printf("%d ", data->numbers[i]);
        }
        printf("\n");
        
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        }
        
        if (pid == 0) { // Child process
            // Find min and max
            data->min = data->numbers[0];
            data->max = data->numbers[0];
            for (int i = 1; i < data->count; i++) {
                if (data->numbers[i] < data->min) {
                    data->min = data->numbers[i];
                }
                if (data->numbers[i] > data->max) {
                    data->max = data->numbers[i];
                }
            }
            
            // Detach shared memory and exit
            shmdt(data);
            exit(0);
        } else { // Parent process
            wait(NULL); // Wait for child to finish
            
            // Print results
            printf("Min: %d, Max: %d\n", data->min, data->max);
            processed_sets++;
            
            // Detach shared memory
            shmdt(data);
            
            // Small delay
            usleep(100000); // 0.1 second
        }
    }
    
    // Clean up
    close(pipefd[0]);
    close(pipefd[1]);
    shmctl(shmid, IPC_RMID, NULL);
    printf("\nProcessed %d data sets\n", processed_sets);
    
    return 0;
}