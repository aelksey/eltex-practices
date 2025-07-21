#include "math.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: REDO input using getline and sscanf

int main(){
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
        if(op == '+')result = sum(a,b); 
        if(op == '-')result = sub(a,b);
        if(op == '/')result = del(a,b);
        if(op == '*')result = mul(a,b);
        if(op == 'm')result = mod(a,b);
        printf(":=%d\n",result);
    }
}