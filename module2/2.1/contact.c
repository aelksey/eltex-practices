#include "contact.h"

#include "string.h"

#define BUFFER 20

void empty_c(contact_t *c){
    c->id = 0;
    memset(c->name.firstname,0,sizeof(c->name.firstname));
    memset(c->name.lastname,0,sizeof(c->name.lastname));
    memset(c->job.workplace,0,sizeof(c->job.workplace));
    memset(c->job.role,0,sizeof(c->job.role));
    memset(c->socials.tg,0,sizeof(c->socials.tg));
    memset(c->socials.ig,0,sizeof(c->socials.ig));
    memset(c->socials.vk,0,sizeof(c->socials.vk));
}