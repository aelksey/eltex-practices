#include "prompt.h"
#include <stdio.h>
#include "display.h"
#include <string.h>

#define BUFFER 50

int id_count = 0;

contact_t *prompt_create_c(){
    int id = id_count;
    id_count++;
    char name[BUFFER], surname[BUFFER], workplace[BUFFER], role[BUFFER], email[BUFFER];
    printf("Create contact prompt:\n");
    printf("Name:");
    scanf("%s",name);
    printf("Lastname:");
    scanf("%s",surname);
    printf("Workplace:");
    scanf("%s",workplace);
    printf("Role:");
    scanf("%s",role);
    printf("E-mail:");
    scanf("%s",email);
    return create_contact(id,name,surname,workplace,role,email);
}

void prompt_edit_c(contact_t *c){
    int id = c->id;
    char name[BUFFER], surname[BUFFER], workplace[BUFFER], role[BUFFER], email[BUFFER];
    printf("Edit contact prompt:\n");

    printf("Name: %s :",c->name.name);
    scanf("%s",name);
    if(strcmp(name,"") != 0)c->name.name = strdup(name);

    printf("Surname: %s :",c->name.surname);
    scanf("%s",surname);
    if(strcmp(surname,"") != 0)c->name.surname = strdup(surname);

    printf("Workplace: %s :",c->job.workplace);
    scanf("%s",workplace);
    if(strcmp(workplace,"") != 0)c->job.workplace = strdup(workplace);

    printf("Role: %s :",c->job.role);
    scanf("%s",role);
    if(strcmp(role,"") != 0)c->job.role = strdup(role);

    printf("E-mail: %s :",c->email.email);
    scanf("%s",email);
    if(strcmp(email,"") != 0)c->email.email = strdup(email);

}

int prompt_remove_c(contact_t *c){
    int status = 0;
    char c;
    printf("Remove contact:\n");
    display_contact(c);
    printf("y/n?");
    scanf("%c",&c);
    if(c == "Y" || c == "y") status = 1;
    return status; 
}

