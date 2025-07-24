#include "contact.h"
#include <string.h>

void empty_c(contact_t *c){
    c->id = NULL;
    c->name.name = strdup("");
    c->name.surname = strdup("");
    c->job.workplace = strdup("");
    c->job.role = strdup("");
    c->email.email = strdup("");
    free(c->name.name);
    free(c->name.surname);
    free(c->job.workplace);
    free(c->job.role);
    free(c->email.email);
}

void init_c(contact_t *c,int id,char *name,char *surname,char *workplace,char *role,char *email){
    c->id = id;
    c->name.name = strdup(name);
    c->name.surname = strdup(surname);
    c->job.workplace = strdup(workplace);
    c->job.role = strdup(role);
    c->email.email = strdup(email);
}

// MAN : Custom comparator function

// MAN : Return values : 0(objects are equal) 1(a > b) -1 (b > a) - just like strcmp()

int cmp_c(contact_t *c1,contact_t *c2,int key){
    int result = 0;

    if(key == NAME) result = strcmp(c1->name.name,c2->name.name);
    if(key == SURNAME) result = strcmp(c1->name.surname,c2->name.surname);
    if(key == WORKPLACE) result = strcmp(c1->job.workplace,c2->job.workplace);
    if(key == ROLE) result = strcmp(c1->job.role,c2->job.role);
    if(key == EMAIL) result = strcmp(c1->email.email,c2->email.email);
    
    return result;
} 
