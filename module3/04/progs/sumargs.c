// Sums arguments

#include <stdio.h>
#include <stdlib.h>

double sum(double nums[],int size);

int main(int argc,char *argv[]){
    double nums[argc];
    if(argc > 2){
        for(int i = 1; i < argc; i++) nums[i] = atof(argv[i]);
        fprintf(stdout,"sum=%.2lf ",sum(nums,argc));
        fprintf(stdout,"\n");
    }else{fprintf(stdout,"Usage: <num1> <num2> ...\n");}
    return 0;
}

double sum(double nums[],int size){
    double sum = 0;
    for(int i = 0; i < size; i++)sum += nums[i];
    return sum;
}
