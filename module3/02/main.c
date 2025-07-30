#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


#define BUFFSIZE 100
#define PROMPT ">:"

char *get_user_input(void);

void exit_handler(char *input);

void handle_command(char *input);

int main() {
    char *input = malloc(BUFFSIZE);
    printf("si - simple interepreter: )\n");
    while (1) {
        printf(PROMPT);
        strcpy(input,get_user_input());
        exit_handler(input);
        handle_command(input);
    }
    free(input);
}

char *get_user_input(void) {
    char *input = malloc(BUFFSIZE);
    if (input == NULL) {
        return NULL;
    }
    
    if (fgets(input, BUFFSIZE, stdin) == NULL) {
        free(input);
        return NULL;
    }
    
    input[strcspn(input, "\n")] = '\0';
    char *str = strdup(input);
    free(input);
    return str;  // Could be NULL if strdup failed
}

void exit_handler(char *input){
    if(strcmp(input,"exit") == 0)exit(EXIT_SUCCESS);
    if(strcmp(input,"Exit") == 0)exit(EXIT_SUCCESS);
    if(strcmp(input,"e") == 0)exit(EXIT_SUCCESS);
    if(strcmp(input,"E") == 0)exit(EXIT_SUCCESS);
    if(strcmp(input,"q") == 0)exit(EXIT_SUCCESS);
}

void handle_command(char *input) {
    int argc = 0;
    char **argv;
    char *token;
    
    // Allocate memory for argv (array of pointers)
    argv = (char **)malloc(BUFFSIZE * sizeof(char *));
    if (argv == NULL) {
        perror("malloc failed");
        return;
    }

    // First tokenization
    token = strtok(input, " ");

    while (token != NULL) {
        
        argv[argc] = strdup(token);  // strdup allocates and copies - no need for strcpy after


        if (argv[argc] == NULL) {
            perror("strdup failed");
            // Clean up already allocated memory
            for (int i = 0; i < argc; i++) {
                free(argv[i]);
            }
        }

       token = strtok(NULL, " ");
        argc++;
    }


    pid_t command_process = fork();
    if (command_process < 0) {
        perror("fork failed");
        // Clean up
        for (int i = 0; i < argc; i++) free(argv[i]);
        free(argv);
        _exit(EXIT_FAILURE);
    }
    if (command_process == 0) {
        // Child process
        if(execvp(argv[0], argv) == -1) perror("execvp failed");
        
        _exit(EXIT_FAILURE);  // Use _exit in child after fork to avoid flushing buffers
    } else {
        for (int i = 0; i < argc; i++) free(argv[i]);
        free(argv);
    }

}