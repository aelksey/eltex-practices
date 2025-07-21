#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 3

// TODO: REDO input using getline and sscanf

int main(){
    int size = 3;
    char op;
    int a,b;
    op = getchar();
    char *in_str = (char *) malloc (size);
    char *str = getline (&in_str, &size, stdin);
    sscanf(str,"%d %d",&a,&b);
    printf("Basic calculator\n");
    printf("Controls: q(uit) (+,-,/,*,m ) - operation op1 op2\n");
    while(1){    
        if(op == 'q')break;
        if(op == '+')printf(":=%d",sum(a,b)); 
        if(op == '-')printf(":=%d",sub(a,b));
        if(op == '/')printf(":=%d",del(a,b));
        if(op == '*')printf(":=%d",mul(a,b));
        if(op == 'm')printf(":=%d",mod(a,b));
    }
}