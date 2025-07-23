#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#define ALL_MASK S_IRWXU | S_IRWXG | S_IRWXO
#define USER_MASK S_IRUSR | S_IWUSR | S_IXUSR
#define GROUP_MASK S_IRGRP | S_IWGRP | S_IXGRP
#define OTHER_MASK S_IROTH | S_IWOTH | S_IXOTH

#define READ_MASK S_IRUSR | S_IRGRP | S_IROTH
#define WRITE_MASK S_IWUSR | S_IWGRP | S_IWOTH
#define EXECUTE_MASK S_IXUSR | S_IXGRP | S_IXOTH

// TODO : Fix numeric mode

char* parse_target(char *src){
    char *target;
    target = strdup(src);
    strcpy(target,"");
    sscanf(src,"%[augo]",target);
    return target;
}

char* parse_operation(char *src){
    char *operation;
    operation = strdup(src);
    strcpy(operation,"");
    sscanf(src,"%*[augo]%[+-=]",operation);
    return operation;
}

char* parse_action(char *src){
    char *action;
    action = strdup(src);
    strcpy(action,"");
    sscanf(src,"%*[augo]%*[+-=]%[rwx]",action);
    return action;
}

void print_mode(mode_t mode){
    printf("%o ",mode & 0777);
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");
}

mode_t update_mode_numeric(long perms){

}

mode_t update_mode(mode_t current_mode,char *target,char *operation,char *action){

    //printf("target=%s operation=%s action=%s ",target,operation,action);

    /*
    
    Algorhitm:

    1. Determine target mask (i.e. u g o etc.)
    2. Determine action mask (i.e. rwx)
    3. Do logical and(&) on target and action masks
    3. Determine operation mask(-,=,+)
    4. Do appropriate actions on final mask(~|,&,|)
    5. Return result
    
    */
    mode_t mask = current_mode;
    mode_t target_mask = 0;
    mode_t action_mask = 0;
    mode_t operation_mask = 0;
    mode_t target_and_action_mask = 0;

    // Logic : Target

    if(strcmp(target,"a") == 0) target_mask = ALL_MASK;
    if(strcmp(target,"u") == 0) target_mask = USER_MASK;
    if(strcmp(target,"g") == 0) target_mask = GROUP_MASK;
    if(strcmp(target,"o") == 0) target_mask = OTHER_MASK;
    if(strcmp(target,"ug") == 0 || strcmp(target,"gu") == 0) target_mask = USER_MASK | GROUP_MASK;
    if(strcmp(target,"ugo") == 0 || strcmp(target,"oug") == 0) target_mask = ALL_MASK;

    // Logic : Action

    if(strcmp(action,"r") == 0){
        action_mask = READ_MASK;
        target_and_action_mask = target_mask & action_mask;
    }

    if(strcmp(action,"w") == 0){
        action_mask = WRITE_MASK;
        target_and_action_mask = target_mask & action_mask;
    }

    if(strcmp(action,"x") == 0){
        action_mask = EXECUTE_MASK;
        target_and_action_mask = target_mask & action_mask;
    }

    if(strcmp(action,"rw") == 0){
        action_mask = (READ_MASK | WRITE_MASK);
        target_and_action_mask = target_mask & action_mask;
    }
    
    if(strcmp(action,"wx") == 0){
        action_mask = (WRITE_MASK | EXECUTE_MASK);
        target_and_action_mask = target_mask & action_mask;
    }
    
    if(strcmp(action,"rx") == 0){
        action_mask = (READ_MASK | EXECUTE_MASK);
        target_and_action_mask = target_mask & action_mask;
    }
    
    if(strcmp(action,"rwx") == 0){
        action_mask = (READ_MASK | WRITE_MASK | EXECUTE_MASK);
        target_and_action_mask = target_mask & action_mask;
    }

    // Logic : Operation

    if(strcmp(operation,"+") == 0){
        operation_mask = target_and_action_mask;
        mask |= operation_mask;
    }

    if(strcmp(operation,"-") == 0){
        operation_mask = target_and_action_mask;
        mask &= ~operation_mask;
    }

    if(strcmp(operation,"=") == 0){
        operation_mask = target_and_action_mask;
        mask = operation_mask;
    }

    return mask;
}

int main(int argc,char *argv[]){
    mode_t mode = 0;
    mode_t current_mode;
    struct stat file_stat;
    char *file = argv[2];
    stat(file, &file_stat);
    current_mode = file_stat.st_mode;
    char* endptr;

    // Numeric mode
    if(argc > 2){
        printf("Numeric input:%li",strtol(argv[1],&endptr,10));
        mode = update_mode_numeric(strtol(argv[1],&endptr,10));
        //mode = 
    } 

    if(endptr == argv[1]){
        // Relative mode
        char *target;
        char *operation;
        char *action;
        target = strdup(parse_target(argv[1]));
        operation = strdup(parse_operation(argv[1]));
        action = strdup(parse_action(argv[1]));
        mode = update_mode(current_mode,target,operation,action);
        free(target);
        free(action);
        free(operation);
    }
    print_mode(mode);
    
    chmod(file,mode);
}