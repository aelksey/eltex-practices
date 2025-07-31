#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFSIZE 1024
#define PROMPT ">:"
#define MAX_COMMANDS 10

char *get_user_input(void);
int should_exit(char *input);
void handle_command(char *input);
void cleanup_commands(char **commands, int count);

int main() {
    printf("si - simple interpreter\n");
    while (1) {
        printf(PROMPT);
        fflush(stdout);
        
        char *input = get_user_input();
        if (input == NULL) continue;
        
        if (should_exit(input)) {
            free(input);
            break;
        }
        
        handle_command(input);
        free(input);
    }
    return EXIT_SUCCESS;
}

char *get_user_input(void) {
    char *input = malloc(BUFFSIZE);
    if (input == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    if (fgets(input, BUFFSIZE, stdin) == NULL) {
        free(input);
        return NULL;
    }
    
    input[strcspn(input, "\n")] = '\0';
    return input;
}

int should_exit(char *input) {
    return (strcmp(input, "exit") == 0 ||
           strcmp(input, "Exit") == 0 ||
           strcmp(input, "e") == 0 ||
           strcmp(input, "E") == 0 ||
           strcmp(input, "q") == 0);
}

void cleanup_commands(char **commands, int count) {
    for (int i = 0; i < count; i++) {
        free(commands[i]);
    }
}

void handle_command(char *input) {
    char *commands[MAX_COMMANDS];
    int num_commands = 0;
    char *saveptr;
    
    char *input_copy = strdup(input);
    if (input_copy == NULL) {
        perror("strdup failed");
        return;
    }

    // Split by pipes
    char *token = strtok_r(input_copy, "|", &saveptr);
    while (token != NULL && num_commands < MAX_COMMANDS) {
        // Trim whitespace
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        
        commands[num_commands] = strdup(token);
        if (commands[num_commands] == NULL) {
            perror("strdup failed");
            free(input_copy);
            cleanup_commands(commands, num_commands);
            return;
        }
        num_commands++;
        token = strtok_r(NULL, "|", &saveptr);
    }
    free(input_copy);

    if (num_commands == 0) {
        return;
    }

    int prev_pipe[2] = {-1, -1};
    pid_t pids[MAX_COMMANDS];
    int num_pids = 0;
    int success = 1;  // Flag to track success through pipeline

    for (int i = 0; i < num_commands && success; i++) {
        int next_pipe[2] = {-1, -1};
        
        if (i < num_commands - 1) {
            if (pipe(next_pipe)) {
                perror("pipe failed");
                success = 0;
                break;
            }
        }

        pids[num_pids] = fork();
        if (pids[num_pids] < 0) {
            perror("fork failed");
            success = 0;
            if (next_pipe[0] != -1) close(next_pipe[0]);
            if (next_pipe[1] != -1) close(next_pipe[1]);
            break;
        }

        if (pids[num_pids] == 0) {  // Child process
            // Handle input redirection
            if (prev_pipe[0] != -1) {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // Handle output redirection
            if (next_pipe[1] != -1) {
                dup2(next_pipe[1], STDOUT_FILENO);
                close(next_pipe[0]);
                close(next_pipe[1]);
            }

            // Parse command and arguments
            char *argv[BUFFSIZE];
            int argc = 0;
            char *saveptr2;
            char *cmd_copy = strdup(commands[i]);
            if (cmd_copy == NULL) {
                perror("strdup failed");
                _exit(EXIT_FAILURE);
            }

            token = strtok_r(cmd_copy, " ", &saveptr2);
            while (token != NULL && argc < BUFFSIZE - 1) {
                argv[argc++] = token;
                token = strtok_r(NULL, " ", &saveptr2);
            }
            argv[argc] = NULL;

            // Handle I/O redirection within command
            for (int j = 0; j < argc; j++) {
                if (strcmp(argv[j], "<") == 0 && j + 1 < argc) {
                    int fd = open(argv[j+1], O_RDONLY);
                    if (fd < 0) {
                        perror("open input file failed");
                        free(cmd_copy);
                        _exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    argv[j] = NULL;
                    break;
                } else if (strcmp(argv[j], ">") == 0 && j + 1 < argc) {
                    int fd = open(argv[j+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd < 0) {
                        perror("open output file failed");
                        free(cmd_copy);
                        _exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    argv[j] = NULL;
                    break;
                }
            }

            execvp(argv[0], argv);
            perror("execvp failed");
            free(cmd_copy);
            _exit(EXIT_FAILURE);
        }

        // Parent process cleanup
        if (prev_pipe[0] != -1) close(prev_pipe[0]);
        if (prev_pipe[1] != -1) close(prev_pipe[1]);
        
        prev_pipe[0] = next_pipe[0];
        prev_pipe[1] = next_pipe[1];
        num_pids++;
    }

    // Close any remaining pipe ends
    if (prev_pipe[0] != -1) close(prev_pipe[0]);
    if (prev_pipe[1] != -1) close(prev_pipe[1]);

    // Wait for all child processes
    for (int i = 0; i < num_pids; i++) {
        waitpid(pids[i], NULL, 0);
    }

    cleanup_commands(commands, num_commands);
}