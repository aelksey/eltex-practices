#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

void print_mode(mode_t mode){
    printf("Битовове представление: %o\n",mode & 0777);

    printf("Буквенное представление: ");
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

mode_t update_mode(mode_t current_mode,char target,char operation,char action){
    mode_t mask = 0;
    mode_t action_mask;

    switch (action)
    {
    case 'r':
        action_mask = S_IRUSR | S_IRGRP | S_IROTH;
        break;

    case 'w':
        action_mask = S_IWUSR | S_IWGRP | S_IWOTH;
        break;

    case 'x':
        action_mask = S_IXUSR | S_IXGRP | S_IXOTH;
        break;
    
    default:
        fprintf(stderr,"Недопустимое действие: %c\n",action);
        break;
    }

    if(target == 'a' || target == 'u') mask |= action_mask & (S_IRUSR | S_IWUSR | S_IXUSR );
    if(target == 'a' || target == 'g') mask |= action_mask & (S_IRGRP | S_IWGRP | S_IXGRP );
    if(target == 'a' || target == 'o') mask |= action_mask & (S_IROTH | S_IWOTH | S_IXOTH );

    switch (operation)
    {
    case '+':
        current_mode |= mask;
        break;
    case '-':
        current_mode |= ~mask;
        break;
    case '=':
        if(target == 'a'){
            current_mode = (current_mode & ~0777) | mask;
        }else{
            if(target == 'u') current_mode = (current_mode & ~0700) | mask;
            if(target == 'g') current_mode = (current_mode & ~0070) | mask;
            if(target == 'o') current_mode = (current_mode & ~0007) | mask;
        }
        break;
    
    default:
        break;
    }
}


int main(int argc, char* argv[]){

    mode_t current_mode;
    char operation = argv[2][0];
    char action = argv[3][0];
    char *file_name = argv[4];

    struct stat file_stat;
    
    if(stat(file_name, &file_stat) == -1){
        if(errno == ENOENT){
            current_mode = 0;
        }else{
            perror("stat");
            exit(EXIT_FAILURE);
        }
    }
    
}