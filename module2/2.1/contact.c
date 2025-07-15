#include "contact.h"

#include <string.h>

void empty_contact(contact_t *contact){
    empty_fio(contact);
    empty_job(contact);
    empty_phone(contact);
    empty_socials(contact);
}

void empty_fio(contact_t *contact){
    memset(contact->fio.name,0,sizeof(contact->fio.name));
    memset(contact->fio.surname,0,sizeof(contact->fio.surname));
    memset(contact->fio.fathername,0,sizeof(contact->fio.fathername));
}

void empty_job(contact_t *contact){
    memset(contact->job.role,0,sizeof(contact->job.role));
    memset(contact->job.workplace,0,sizeof(contact->job.workplace));
}

void empty_phone(contact_t *contact){
    memset(contact->phone.personal,0,sizeof(contact->phone.personal));
    memset(contact->phone.work,0,sizeof(contact->phone.work));
    memset(contact->phone.other,0,sizeof(contact->phone.other));
}

void empty_socials(contact_t *contact){
    memset(contact->socials.tg,0,sizeof(contact->socials.tg));
    memset(contact->socials.vk,0,sizeof(contact->socials.vk));
    memset(contact->socials.ig,0,sizeof(contact->socials.ig));
}