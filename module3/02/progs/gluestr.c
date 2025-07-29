// Strcat analogue

#include <stdio.h>

int main(int argc,char *argv[]){
    if(argc > 2){
        for(int i = 1; i < argc; i++) fprintf(stdout,"%s",argv[i]);
        fprintf(stdout,"\n");
    }else{fprintf(stdout,"Usage: <str1> <str2> ...\n");}
    return 0;
}