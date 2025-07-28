#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>

#define OP_SIZE 5
#define BUFFER 20

typedef double (*operation)(double,double);

typedef struct operations {
   double (*op[OP_SIZE])(double,double);
} operations_t;

operation load_lib(const char *path, char *func_name);
operations_t load_libs(const char* path);
void initialize_operations(operations_t *ops);

int main() {
    operations_t operations;
    size_t size = 0;
    const char* path = "./";
    char op;
    double a, b;
    char *str = NULL;
    
    initialize_operations(&operations);
    operations = load_libs(path);
    
    printf("Basic calculator\n");
    printf("Controls: q(uit) (+,-,/,*,m) - operation op1 op2\n");
    
    while(1) {
        double result = 0;
        int valid = 0;
        
        // Read the entire line at once
        size_t read = getline(&str, &size, stdin);
        if (read == -1) {
            perror("Error reading input");
            continue;
        }
        
        // Check for quit command first
        if (str[0] == 'q' && (str[1] == '\n' || str[1] == '\0')) {
            break;
        }
        
        // Parse the input
        if (sscanf(str, "%c %lf %lf", &op, &a, &b) != 3) {
            printf("Invalid input format. Use: operator number number\n");
            continue;
        }
        
        switch(op) {
            case '-': 
                if (operations.op[0]) {
                    result = operations.op[0](a,b);
                    valid = 1;
                }
                break;
            case '*': 
                if (operations.op[1]) {
                    result = operations.op[1](a,b);
                    valid = 1;
                }
                break;
            case '+': 
                if (operations.op[2]) {
                    result = operations.op[2](a,b);
                    valid = 1;
                }
                break;
            case 'm': 
                if (operations.op[3]) {
                    result = operations.op[3](a,b);
                    valid = 1;
                }
                break;
            case '/': 
                if (operations.op[4]) {
                    result = operations.op[4](a,b);
                    valid = 1;
                }
                break;
            default:
                printf("Unknown operation\n");
                continue;
        }
        
        if (valid) {
            printf(":=%.2lf\n", result);
        } else {
            printf("Operation not available\n");
        }
    }
    
    free(str);
    return 0;
}

void initialize_operations(operations_t *ops) {
    for (int i = 0; i < OP_SIZE; i++) {
        ops->op[i] = NULL;
    }
}

operations_t load_libs(const char* path) {
    int count = 0;
    DIR *dir;
    struct dirent *entry;
    operations_t operations;
    
    initialize_operations(&operations);

    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL && count < OP_SIZE) {
        size_t len = strlen(entry->d_name);
        if (len >= 3 && strcmp(entry->d_name + len - 3, ".so") == 0) {
            char func_name[BUFFER] = {0};
            strncpy(func_name, entry->d_name, len - 3);
            func_name[BUFFER-1] = '\0'; // Ensure null termination
            
            //printf("Loading function: %s at position %d\n", func_name, count);
            operations.op[count] = load_lib(entry->d_name, func_name);
            if (operations.op[count]) {
                count++;
            }
        }
    }
    closedir(dir);

    return operations;
}

operation load_lib(const char *path, char* func_name) {
    void *handle = dlopen(path, RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Error loading lib %s: %s\n", path, dlerror());
        return NULL;
    }

    operation func = (operation)dlsym(handle, func_name);
    if (!func) {
        fprintf(stderr, "Error loading symbol %s: %s\n", func_name, dlerror());
        dlclose(handle);
        return NULL;
    }

    // Don't close the handle here - we need to keep it open for the function to work
    return func;
}