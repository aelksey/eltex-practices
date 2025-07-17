#include "contact.h"

#include <string.h>
#include <stdarg.h>

contact_t empty_c(contact_t c){
    c.id = 0;
    strcpy(c.name.firstname,"");
    strcpy(c.name.lastname,"");
    strcpy(c.job.workplace,"");
    strcpy(c.job.role,"");
    strcpy(c.socials.tg,"");
    strcpy(c.socials.ig,"");
    strcpy(c.socials.vk,"");

    return c;
}

contact_t init_c(int id,int count, ...){
    va_list args;
    va_start(args,count);
    char params[FIELDS][BUFFER];

    for (int i = 0; i < FIELDS; i++) strcpy(params[i],"");

    for (int i = 0; i < count; i++ ) strcpy(params[i],va_arg(args,const char*)); 
    va_end(args);

    contact_t c;

    c.id = id;
    strcpy(c.name.firstname,params[0]);
    strcpy(c.name.lastname,params[1]);
    strcpy(c.job.workplace,params[2]);
    strcpy(c.job.role,params[3]);
    strcpy(c.socials.tg,params[4]);
    strcpy(c.socials.ig,params[5]);
    strcpy(c.socials.vk,params[6]);
    return c;
}

// ARCH : prompt forms an contact object for us which we then add to book struct