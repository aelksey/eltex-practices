#define CONTACT_H_
#ifndef CONTACT_H_

#define BUFFER 20

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

void empty_c(contact_t *c);

// ARCH : prompt forms an contact object for us which we then add to book struct

#endif