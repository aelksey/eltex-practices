#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#define OP_SIZE 5

// TODO: Create an struct array of function pointers to store all the data

typedef struct operations {
   double (*op[OP_SIZE])(double,double);
}operations_t;

void init(operations_t *operations){
    operations->op[0] = sum;
    operations->op[1] = sub;
    operations->op[2] = del;
    operations->op[3] = mul;
    operations->op[4] = mod;
}

int main(){
    operations_t operations;
    init(&operations);
    size_t size = 3;
    char op;
    double a,b;
    printf("Basic calculator\n");
    printf("Controls: q(uit) (+,-,/,*,m ) - operation op1 op2\n");
    while(1){
        double result;
        op = getchar();
        char *str = (char*)malloc(size*sizeof(char));
        getline (&str, &size, stdin);
        sscanf(str,"%lf %lf",&a,&b);    
        if(op == 'q')break;
        if(op == '+')result = operations.op[0](a,b); 
        if(op == '-')result = operations.op[1](a,b);
        if(op == '/')result = operations.op[2](a,b);
        if(op == '*')result = operations.op[3](a,b);
        if(op == 'm')result = operations.op[4](a,b);
        printf(":=%.2lf\n",result);
    }
}