#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <time.h>

#define FILE_NAME "shared_file.txt"
#define SEM_KEY 1234

// Semaphore operations
void sem_wait(int sem_id) {
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0},{0, 1, 0}};
    // Check if semaphore is locked
    if(semctl(sem_id,0,GETVAL) == 0)semop(sem_id,unlock,2);
    semop(sem_id, &lock, 1);
}

void sem_signal(int sem_id) {
    // TODO : Fix according to presentation about SystemV semaphores
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);
}

int main() {
    srand(time(NULL));
    FILE *file;
    int sem_id;

    // Create or get the semaphore
    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize semaphore to 1 (unlocked)
    semctl(sem_id, 0, SETVAL, 1);

    while (1) {
        // Generate a random string of numbers
        int num_count = rand() % 10 + 1;  // 1-10 numbers
        char line[256] = "";
        char num_str[16];

        for (int i = 0; i < num_count; i++) {
            int num = rand() % 100;  // 0-99
            sprintf(num_str, "%d ", num);
            strcat(line, num_str);
        }
        strcat(line, "\n");

        // Lock semaphore before writing
        sem_wait(sem_id);

        file = fopen(FILE_NAME, "a");
        if (!file) {
            perror("fopen");
            sem_signal(sem_id);
            exit(1);
        }
        fprintf(file, "%s", line);
        fclose(file);

        printf("Producer wrote: %s", line);

        // Unlock semaphore
        sem_signal(sem_id);

        sleep(rand() % 3 + 1);  // Wait 1-3 seconds
    }

    return 0;
}