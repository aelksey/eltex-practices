#include "display.h"

#include <stdio.h>

#include "contact.h"

void display_contact(contact_t contact){
    display_contact_fio(contact);
    display_contact_job(contact);
    display_contact_phone(contact);
    display_contact_phone(contact);
    display_contact_socials(contact);
}

void display_contact_fio(contact_t contact){
    printf("Name%s:\n",contact.fio.name);
    printf("Surname%s:\n",contact.fio.surname);
    printf("Fathername%s:\n",contact.fio.fathername);
}

void display_contact_job(contact_t contact){
    printf("Workplace%s:\n",contact.job.workplace);
    printf("Role%s:\n",contact.job.role);
}

void display_contact_phone(contact_t contact){
    printf("Perosnal phone number%s:\n",contact.phone.personal);
    printf("Work phone number%s:\n",contact.phone.work);
    printf("Other phone number%s:\n",contact.phone.other);
}

void display_contact_socials(contact_t contact){
    printf("Telegram%s:\n",contact.socials.tg);
    printf("Vkontakte%s:\n",contact.socials.vk);
    printf("Instagram%s:\n",contact.socials.ig);
}