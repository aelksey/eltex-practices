#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


#define STRING 0
#define INTEGER 1
#define DOUBLE 2

int arg_type(const char *arg);

void arg_action(const char *arg);

void handle_arg(int proc_selector ,const char *arg);

int is_double(const char *arg);

int main(int argc,char *argv[]) {
    
    int proc_selector = 0; // 0 - parent ; 1 - child

    for(int i = 1; i < argc;i++){
        handle_arg(proc_selector,argv[i]);
        // Change proc selector to child process
        if(proc_selector == 0)proc_selector = 1;
        // Change proc selector to parent process
        if(proc_selector == 1)proc_selector = 0;
    }
    printf("\n");

    return 0;
}

int is_double(const char *arg){
    int result = 0;
    for(int i = 0;i < strlen(arg); i++){
        if(arg[i] == '.' || arg[i] == '.'){
            result = 1;
            break;
        }
    }
    return result;
}

int arg_type(const char *arg){
    int type = STRING;
    if(atoi(arg) != 0)type = INTEGER;    
    if(is_double(arg) != 0)type = DOUBLE; 
    return type;
}

void arg_action(const char *arg){
    char *endptr;
        if (arg_type(arg) == STRING)printf("%s ",arg);
        if (arg_type(arg) == INTEGER)printf("%d %d ",atoi(arg),atoi(arg)*2);
        if (arg_type(arg) == DOUBLE)printf("%.2lf %.2lf ",strtod(arg,&endptr),strtod(arg,&endptr)*2);
}

void handle_arg(int proc_selector ,const char *arg){
    
    if(proc_selector == 0)arg_action(arg);
    
    if(proc_selector == 1){
        pid_t pid = fork();
        if(pid == 0){arg_action(arg);_exit(EXIT_SUCCESS);}
        if(pid == -1){perror("fork failed");_exit(EXIT_FAILURE);}
    }

}
