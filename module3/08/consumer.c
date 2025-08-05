#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <limits.h>
#include <time.h>

#define FILE_NAME "shared_file.txt"
#define SEM_KEY 1234
#define TEMP_FILE "temp_file.txt"

void sem_wait(int sem_id) {
    // TODO : Fix according to presentation about SystemV semaphores
    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock[2] = {{0, 0, 0},{0, 1, 0}};
    // Check if semaphore is locked
    if(semctl(sem_id,0,GETVAL) == 0)semop(sem_id,unlock,2);
    semop(sem_id, &lock, 1);
}

void sem_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    semop(sem_id, &sb, 1);
}

void process_line(char *line) {
    int num, min = INT_MAX, max = INT_MIN;
    char *token = strtok(line, " \n");

    while (token != NULL) {
        num = atoi(token);
        if (num < min) min = num;
        if (num > max) max = num;
        token = strtok(NULL, " \n");
    }

    if (min != INT_MAX && max != INT_MIN) {
        printf("Consumer %d: Min=%d, Max=%d\n", getpid(), min, max);
    }
}

int main() {
    FILE *file, *temp_file;
    int sem_id;

    sem_id = semget(SEM_KEY, 1, 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    while (1) {
        // Lock semaphore before reading
        sem_wait(sem_id);

        file = fopen(FILE_NAME, "r");
        if (!file) {
            perror("fopen");
            sem_signal(sem_id);
            exit(1);
        }

        temp_file = fopen(TEMP_FILE, "w");
        if (!temp_file) {
            perror("fopen");
            fclose(file);
            sem_signal(sem_id);
            exit(1);
        }

        char line[256];
        int processed = 0;

        // Read each line, process first unprocessed line
        while (fgets(line, sizeof(line), file)) {
            if (!processed) {
                process_line(line);
                processed = 1;
            } else {
                fprintf(temp_file, "%s", line);
            }
        }

        fclose(file);
        fclose(temp_file);

        // Replace original file with remaining lines
        remove(FILE_NAME);
        rename(TEMP_FILE, FILE_NAME);

        // Unlock semaphore
        sem_signal(sem_id);

        //sleep(1);  // Wait before checking again

        // To show concurrenncy wait random amount of time

        srand(time(NULL));

        sleep(rand() % 3 + 1);  // Wait 1-3 seconds
    }

    return 0;
}