#include "math.h"
#include <stdio.h>

double sum(double a, double b){return a+b;}

double sub(double a, double b){return a-b;}

double del(double a, double b){
    double result;
    if(b == 0){
        printf("Error: Division by zero(%.2lf/%.2lf)",a,b);
        result = 0;
    }else{
        result = a / b;
    }
    return result;
}

double mul(double a,double b){return a * b;}

double mod(double a, double b){
    int result = (int)a % (int)b;
    return (double)result;
}
