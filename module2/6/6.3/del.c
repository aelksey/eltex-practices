#include "del.h"
#include <stdio.h>

double libdel(double a, double b){
    double result;
    if(b == 0){
        // Experiment : see ./task.md
        //printf("Ошибка: Деление на ноль(%.2lf/%.2lf)",a,b);
        printf("Error: Division by zero(%.2lf/%.2lf)",a,b);
        result = 0;
    }else{
        result = a / b;
    }
    return result;
}