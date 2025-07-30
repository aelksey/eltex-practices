// Sums arguments

#include <stdio.h>
#include <stdlib.h>

double max(double nums[],int size);
double min(double nums[],int size);

int main(int argc,char *argv[]){
    double nums[argc];
    if(argc > 2){
        for(int i = 1; i < argc; i++) nums[i] = atof(argv[i]);
        fprintf(stdout,"min=%.2lf max=%.2lf \n",min(nums,argc),max(nums,argc));
    }else{fprintf(stdout,"Usage: <num1> <num2> ...\n");}
    return 0;
}

double max(double nums[],int size){
    double max = nums[0];
    for(int i = 0; i < size; i++)if(nums[i] > max) max = nums[i];
    return max;
}

double min(double nums[],int size){
    double min = nums[0];
    for(int i = 0; i < size; i++)if(nums[i] < min) min = nums[i];
    return min;
}
