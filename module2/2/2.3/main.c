#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#define OP_SIZE 5

// TODO: Create an struct array of function pointers to store all the data

typedef struct operations {
   int (*op[OP_SIZE])(int,int);
}operations_t;

void init(operations_t *operations){
    operations->op[0] = sum;
    operations->op[1] = sub;
    operations->op[2] = mul;
    operations->op[3] = del;
    operations->op[4] = mod;
}

int main(){
    operations_t operations;
    init(&operations);
    size_t size = 3;
    char op;
    int a,b;
    printf("Basic calculator\n");
    printf("Controls: q(uit) (+,-,/,*,m ) - operation op1 op2\n");
    while(1){
        int result;
        op = getchar();
        char *str = (char*)malloc(size*sizeof(char));
        getline (&str, &size, stdin);
        sscanf(str,"%d %d",&a,&b);    
        if(op == 'q')break;
        if(op == '+')result = operations.op[0](a,b); 
        if(op == '-')result = operations.op[1](a,b);
        if(op == '/')result = operations.op[2](a,b);
        if(op == '*')result = operations.op[3](a,b);
        if(op == 'm')result = operations.op[4](a,b);
        printf(":=%d\n",result);
    }
}