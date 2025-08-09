#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <inttypes.h>
#include <signal.h>

#define MAX_DRIVERS 10
#define BUFFER_SIZE 256

typedef enum {
    AVAILABLE,
    BUSY
} DriverStatus;

typedef struct {
    pid_t pid;
    int pipe_fd[2];
    DriverStatus status;
    int timer_fd;
    int task_duration;
    time_t task_start_time;
    char status_str[BUFFER_SIZE];
} DriverInfo;

DriverInfo drivers[MAX_DRIVERS];
int num_drivers = 0;

void cleanup() {
    for (int i = 0; i < num_drivers; i++) {
        close(drivers[i].pipe_fd[1]);
        kill(drivers[i].pid, SIGTERM);
    }
    while (wait(NULL) > 0);
}

void update_driver_status(int index) {
    if (drivers[index].status == BUSY) {
        int remaining = drivers[index].task_duration - (time(NULL) - drivers[index].task_start_time);
        if (remaining <= 0) {
            drivers[index].status = AVAILABLE;
            drivers[index].timer_fd = -1;
            snprintf(drivers[index].status_str, BUFFER_SIZE, "Available");
        } else {
            snprintf(drivers[index].status_str, BUFFER_SIZE, "Busy(%d)", remaining);
        }
    } else {
        snprintf(drivers[index].status_str, BUFFER_SIZE, "Available");
    }
}

void create_driver() {
    if (num_drivers >= MAX_DRIVERS) {
        printf("Maximum number of drivers reached\n");
        return;
    }

    if (pipe(drivers[num_drivers].pipe_fd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(drivers[num_drivers].pipe_fd[0]);
        close(drivers[num_drivers].pipe_fd[1]);
        return;
    }

    if (pid == 0) { // Child process (driver)
        close(drivers[num_drivers].pipe_fd[1]);

        struct pollfd fds[2];
        fds[0].fd = drivers[num_drivers].pipe_fd[0];
        fds[0].events = POLLIN;
        
        int timer_fd = -1;
        fds[1].fd = timer_fd;
        fds[1].events = POLLIN;

        char buffer[BUFFER_SIZE];
        int task_duration = 0;
        time_t task_start_time = 0;

        while (1) {
            int ret = poll(fds, 2, -1);
            if (ret == -1) {
                perror("poll");
                break;
            }

            if (fds[0].revents & POLLIN) {
                ssize_t bytes_read = read(fds[0].fd, buffer, BUFFER_SIZE - 1);
                if (bytes_read <= 0) {
                    if (bytes_read == 0) {
                        printf("Driver %d: CLI closed connection\n", getpid());
                    } else {
                        perror("read");
                    }
                    break;
                }
                buffer[bytes_read] = '\0';

                if (strncmp(buffer, "SEND_TASK", 9) == 0) {
                    sscanf(buffer + 10, "%d", &task_duration);
                    
                    if (timer_fd != -1) {
                        int remaining = task_duration - (time(NULL) - task_start_time);
                        if (remaining < 0) remaining = 0;
                        char response[BUFFER_SIZE];
                        snprintf(response, BUFFER_SIZE, "Busy(%d)", remaining);
                        write(STDOUT_FILENO, response, strlen(response));
                        continue;
                    }

                    timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
                    if (timer_fd == -1) {
                        perror("timerfd_create");
                        continue;
                    }

                    struct itimerspec timer_spec;
                    timer_spec.it_value.tv_sec = task_duration;
                    timer_spec.it_value.tv_nsec = 0;
                    timer_spec.it_interval.tv_sec = 0;
                    timer_spec.it_interval.tv_nsec = 0;

                    if (timerfd_settime(timer_fd, 0, &timer_spec, NULL) == -1) {
                        perror("timerfd_settime");
                        close(timer_fd);
                        timer_fd = -1;
                        continue;
                    }

                    task_start_time = time(NULL);
                    fds[1].fd = timer_fd;
                
                } else if (strncmp(buffer, "GET_STATUS", 10) == 0) {
                    char response[BUFFER_SIZE];
                    if (timer_fd != -1) {
                        int remaining = task_duration - (time(NULL) - task_start_time);
                        if (remaining < 0) remaining = 0;
                        snprintf(response, BUFFER_SIZE, "Busy(%d)", remaining);
                    } else {
                        snprintf(response, BUFFER_SIZE, "Available");
                    }
                    write(STDOUT_FILENO, response, strlen(response));
                }
            }

            if (fds[1].revents & POLLIN) {
                uint64_t expirations;
                ssize_t s = read(timer_fd, &expirations, sizeof(expirations));
                if (s != sizeof(expirations)) {
                    perror("read timer");
                }
                
                close(timer_fd);
                timer_fd = -1;
                fds[1].fd = -1;
                task_duration = 0;
            }
        }

        if (timer_fd != -1) {
            close(timer_fd);
        }
        close(drivers[num_drivers].pipe_fd[0]);
        exit(0);
    } else { // Parent process (CLI)
        close(drivers[num_drivers].pipe_fd[0]);
        drivers[num_drivers].pid = pid;
        drivers[num_drivers].status = AVAILABLE;
        drivers[num_drivers].timer_fd = -1;
        drivers[num_drivers].task_duration = 0;
        drivers[num_drivers].task_start_time = 0;
        snprintf(drivers[num_drivers].status_str, BUFFER_SIZE, "Available");
        num_drivers++;
        printf("Driver created with PID: %d\n", pid);
    }
}

void send_task(pid_t pid, int task_timer) {
    for (int i = 0; i < num_drivers; i++) {
        if (drivers[i].pid == pid) {
            char command[BUFFER_SIZE];
            snprintf(command, BUFFER_SIZE, "SEND_TASK %d", task_timer);
            write(drivers[i].pipe_fd[1], command, strlen(command) + 1);
            
            // Update parent's status information
            drivers[i].status = BUSY;
            drivers[i].task_duration = task_timer;
            drivers[i].task_start_time = time(NULL);
            drivers[i].timer_fd = 1; // Mark as busy
            update_driver_status(i);
            return;
        }
    }
    printf("Driver with PID %d not found\n", pid);
}

void get_status(pid_t pid) {
    for (int i = 0; i < num_drivers; i++) {
        if (drivers[i].pid == pid) {
            update_driver_status(i);
            printf("| %-8d | %-14s |\n", drivers[i].pid, drivers[i].status_str);
            return;
        }
    }
    printf("Driver with PID %d not found\n", pid);
}

void get_drivers() {
    printf("+----------+----------------+\n");
    printf("| %-8s | %-14s |\n", "PID", "Status");
    printf("+----------+----------------+\n");
    
    for (int i = 0; i < num_drivers; i++) {
        update_driver_status(i);
        printf("| %-8d | %-14s |\n", drivers[i].pid, drivers[i].status_str);
    }
    
    printf("+----------+----------------+\n");
}

int main() {
    printf("Taxi Management System\n");
    printf("Available commands:\n");
    printf("create_driver\n");
    printf("send_task <pid> <task_timer>\n");
    printf("get_status <pid>\n");
    printf("get_drivers\n");
    printf("exit\n");

    char input[BUFFER_SIZE];
    while (1) {
        printf("> ");
        fflush(stdout);
        
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "create_driver") == 0) {
            create_driver();
        } else if (strncmp(input, "send_task", 9) == 0) {
            pid_t pid;
            int task_timer;
            if (sscanf(input + 10, "%d %d", &pid, &task_timer) == 2) {
                send_task(pid, task_timer);
            } else {
                printf("Usage: send_task <pid> <task_timer>\n");
            }
        } else if (strncmp(input, "get_status", 10) == 0) {
            pid_t pid;
            if (sscanf(input + 11, "%d", &pid) == 1) {
                get_status(pid);
            } else {
                printf("Usage: get_status <pid>\n");
            }
        } else if (strcmp(input, "get_drivers") == 0) {
            get_drivers();
        } else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("Unknown command\n");
        }
    }

    cleanup();
    return 0;
}