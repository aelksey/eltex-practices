#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define MODE_PROG O_CREAT | O_WRONLY | O_APPEND
#define MODE_OS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IRGRP | S_IWGRP
#define DEFAULT_PATH "data.txt"

static int sig_count = 1;

static char *filename = DEFAULT_PATH;

int write_count(int count);

int write_signal(int signal);

void signal_handler(int signal);

int main(int argc,char *argv[]){
    int count = 1;
    filename = DEFAULT_PATH;
    if(argc > 2)filename = argv[1];

    while (1){
        write_count(count);
        sleep(1);
        signal(SIGINT,signal_handler); 
        signal(SIGQUIT,signal_handler);
        // Ignore rest of signals
        signal(SIGABRT,SIG_IGN);
        signal(SIGTERM,SIG_IGN);
        signal(SIGTSTP,SIG_IGN);
        signal(SIGSTOP,SIG_IGN);
        signal(SIGCONT,SIG_IGN);
        count++;
        
    }
    
    return 0;
}

int write_count(int count){
    FILE *file = fopen(filename,"a");
    if(file == NULL){perror("Error opening file");return -1;}
    fprintf(file,"%d\n",count);
    fclose(file);
}

int write_signal(int signal){
    FILE *file = fopen(filename,"a");

    const char *msg;

    if(signal == SIGINT)msg="SIGINT";
    if(signal == SIGQUIT)msg="SIGQUIT";

    if(file == NULL){perror("Error opening file");return -1;}
    fprintf(file,"Received signal - %d - %s\n",signal,msg);
    fclose(file);
}

void signal_handler(int signal){
    if (signal == SIGINT){
        write_signal(signal);
        if(sig_count == 3)exit(EXIT_SUCCESS);
        sig_count++;
    }
    if (signal == SIGQUIT){write_signal(signal);}
}