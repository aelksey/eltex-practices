#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>

#define SHM_NAME "/posix_shm"
#define SHM_SIZE 1024

int processed_sets = 0;
int pipefd[2];

typedef struct {
    int count;
    int numbers[SHM_SIZE/sizeof(int) - 3];
    int min;
    int max;
} shared_data;

void sigint_handler(int sig) {
    char c = 'x';
    write(pipefd[1], &c, 1);
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

    // Create POSIX shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // Configure the size
    if (ftruncate(shm_fd, sizeof(shared_data))) {
        perror("ftruncate");
        exit(1);
    }

    while (1) {
        // Check for signal via pipe
        fd_set rfds;
        struct timeval tv = {0, 0};
        FD_ZERO(&rfds);
        FD_SET(pipefd[0], &rfds);
        
        if (select(pipefd[0] + 1, &rfds, NULL, NULL, &tv) > 0) {
            if (FD_ISSET(pipefd[0], &rfds)) {
                break;
            }
        }

        // Memory map the shared memory
        shared_data *data = mmap(NULL, sizeof(shared_data), 
                               PROT_READ | PROT_WRITE, 
                               MAP_SHARED, shm_fd, 0);
        if (data == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }

        // Parent process generates random numbers
        data->count = rand() % 20 + 1;
        printf("Generated %d numbers: ", data->count);
        for (int i = 0; i < data->count; i++) {
            data->numbers[i] = rand() % 1000;
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
                if (data->numbers[i] < data->min) data->min = data->numbers[i];
                if (data->numbers[i] > data->max) data->max = data->numbers[i];
            }

            munmap(data, sizeof(shared_data));
            exit(0);
        } else { // Parent process
            wait(NULL);
            printf("Min: %d, Max: %d\n", data->min, data->max);
            processed_sets++;
            munmap(data, sizeof(shared_data));
            usleep(100000);
        }
    }

    // Cleanup
    close(pipefd[0]);
    close(pipefd[1]);
    shm_unlink(SHM_NAME);
    printf("\nProcessed %d data sets\n", processed_sets);

    return 0;
}
