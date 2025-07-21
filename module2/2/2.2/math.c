#include "math.h"
#include <stdio.h>

int sum(int a, int b){return a+b;}

int sub(int a, int b){return a-b;}

int del(int a, int b){
    int result;
    if(b == 0){
        printf("Error: Division by zero(%d/%d)",a,b);
        result = 0;
    }else{
        result = a / b;
    }
    return result;
}

int mul(int a,int b){return a * b;}

int mod(int a, int b){return a % b;}
