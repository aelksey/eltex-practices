#pragma once

#define BUFFER 20
#define FIELDS 7

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

typedef struct contact {
    int id;
    name_t name;
    job_t job;
    socials_t socials;
} contact_t;

contact_t empty_c(contact_t c);

contact_t init_c(int id, int count , ...);


