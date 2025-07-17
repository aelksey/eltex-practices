#include "contact.h"

#include <string.h>
#include <stdarg.h>

#define BUFFER 20
#define FIELDS 6

typedef struct contact {
    int id;
    name_t name;
    job_t job;
    socials_t socials;
} contact_t;

typedef struct name {   
    char firstname[BUFFER];
    char lastname[BUFFER];
} name_t;

typedef struct job {
    char workplace[BUFFER];
    char role[BUFFER];
} job_t;

typedef struct socials {
    char tg[BUFFER];
    char ig[BUFFER];
    char vk[BUFFER];
}socials_t;

contact_t empty_c(contact_t c){
    c.id = 0;
    memset(c.name.firstname,0,sizeof(c.name.firstname));
    memset(c.name.lastname,0,sizeof(c.name.lastname));
    memset(c.job.workplace,0,sizeof(c.job.workplace));
    memset(c.job.role,0,sizeof(c.job.role));
    memset(c.socials.tg,0,sizeof(c.socials.tg));
    memset(c.socials.ig,0,sizeof(c.socials.ig));
    memset(c.socials.vk,0,sizeof(c.socials.vk));
    return c;
}

contact_t init_c(int id,int count, ...){
    va_list args;
    va_start(args,count);
    char params[FIELDS][BUFFER];
    for (int i = 0; i < count; i++ ) strcpy(params[i],va_arg(args,const char*)); 
    va_end(args);

    contact_t c;
    c.id = id;
    strcpy(c.name.firstname,params[0]);
    strcpy(c.name.lastname,params[1]);
    strcpy(c.job.workplace,params[0]);
    strcpy(c.job.role,params[1]);
    strcpy(c.socials.tg,params[0]);
    strcpy(c.socials.ig,params[1]);
    strcpy(c.socials.vk,params[0]);
    return c;
}

// ARCH : prompt forms an contact object for us which we then add to book struct
