#include "prompt.h"
#include <stdio.h>
#include "display.h"
#include <string.h>

#define BUFFER 50

size_t size = BUFFER * sizeof(char);

int id_count = 0;

contact_t *prompt_create_c(){
    int id = id_count;
    id_count++;
    char name[BUFFER], surname[BUFFER], workplace[BUFFER], role[BUFFER], email[BUFFER];
    printf("Create contact prompt:\n");

    fgets(name, BUFFER, stdin);
    
    printf("Name:");
    fgets(name, BUFFER, stdin);

    printf("Lastname:");
    fgets(surname, BUFFER, stdin);

    printf("Workplace:");
    fgets(workplace, BUFFER, stdin);
    
    printf("Role:");
    fgets(role, BUFFER, stdin);
    
    printf("E-mail:");
    fgets(email, BUFFER, stdin);

    return create_contact(id,name,surname,workplace,role,email);
}

void prompt_edit_c(contact_t *c){
    int id = c->id;
    char name[BUFFER], surname[BUFFER], workplace[BUFFER], role[BUFFER], email[BUFFER];
    printf("Edit contact prompt:\n");

    fgets(name, BUFFER, stdin);

    printf("Name: %s :",c->name.name);
    fgets(name, BUFFER, stdin);
    if(strcmp(name,"") != 0)c->name.name = strdup(name);

    printf("Surname: %s :",c->name.surname);
    fgets(surname, BUFFER, stdin);
    if(strcmp(surname,"") != 0)c->name.surname = strdup(surname);

    printf("Workplace: %s :",c->job.workplace);
    fgets(workplace, BUFFER, stdin);
    if(strcmp(workplace,"") != 0)c->job.workplace = strdup(workplace);

    printf("Role: %s :",c->job.role);
    fgets(role, BUFFER, stdin);
    if(strcmp(role,"") != 0)c->job.role = strdup(role);

    printf("E-mail: %s :",c->email.email);
    fgets(email, BUFFER, stdin);
    if(strcmp(email,"") != 0)c->email.email = strdup(email);

}

int prompt_remove_c(contact_t *c){
    int status = 0;
    char s;
    printf("Remove contact:\n");
    display_contact(c);
    printf("y/n?");
    getchar();
    s = getchar();
    if(s == 'Y' || s == 'y') status = 1;
    return status; 
}

