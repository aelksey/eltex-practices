#include "math.h"
#include <stdio.h>


// TODO: REDO input using getline and sscanf

int main(){
    char op;
    int a,b;
    
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