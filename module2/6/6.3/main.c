#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>

#define OP_SIZE 5
#define BUFFER 20

typedef double (*operation)(double,double);

typedef struct operations {
   double (*op[OP_SIZE])(double,double);
}operations_t;

operation load_lib(const char *path,char *func_name);

operations_t load_libs(const char* path);

int main(){
    operations_t operations;
    size_t size = 3;
    const char* path = "./";
    char op;
    double a,b;
    operations = load_libs(path);
    printf("Basic calculator\n");
    printf("Controls: q(uit) (+,-,/,*,m ) - operation op1 op2\n");
    while(1){
        double result = 0;
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

operations_t load_libs(const char* path){
    
    int count = 0;
    DIR *dir;
    struct dirent *entry;
    operations_t operations;

    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    char func_name[BUFFER];

    while ((entry = readdir(dir)) != NULL) {
        if (strlen(entry->d_name) >= 3 && strcmp(entry->d_name + strlen(entry->d_name) - 3, ".so") == 0) {
            strcpy(func_name,"");
            strncpy(func_name,entry->d_name,strlen(entry->d_name) - 3);
            printf("Func name: %s count = %d ",func_name,count);
            operations.op[count] = load_lib(entry->d_name,func_name);
            count++;
        }
    }
    closedir(dir);

    return operations;
}

operation load_lib(const char *path,char* func_name){
    void *handle;
    handle = dlopen(path,RTLD_NOW);

    if(!handle){
        fprintf(stderr,"Error loading lib: %s\n",dlerror());
        exit(EXIT_FAILURE);
    }

    operation func = (operation)dlsym(handle,func_name);

    dlclose(handle);

    return func;
}

