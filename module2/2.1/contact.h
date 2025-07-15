#ifndef CONTACT_H
#define CONTACT_H

#define BUFFSIZE 20

typedef struct contact {
    fio_t fio;
    job_t job;
    phone_t phone;
    socials_t socials; 
} contact_t;

typedef struct fio {
    char name[BUFFSIZE];
    char surname[BUFFSIZE];
    char fathername[BUFFSIZE];
} fio_t;

typedef struct job {
    char workplace[BUFFSIZE];
    char role[BUFFSIZE];
} job_t;

typedef struct phone {
    char personal[BUFFSIZE];
    char work[BUFFSIZE];
    char other[BUFFSIZE];
} phone_t;

typedef struct socials {
    char tg[BUFFSIZE];
    char vk[BUFFSIZE];
    char ig[BUFFSIZE];
} socials_t;

void empty_contact(contact_t *contact);
void empty_fio(contact_t *contact);
void empty_job(contact_t *contact);
void empty_phone(contact_t *contact);
void empty_socials(contact_t *contact);

#endif 