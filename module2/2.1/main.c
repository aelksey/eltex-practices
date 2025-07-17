#include "book.h"

#include "contact.h"

#include "book.c"

#include <string.h>

#include <stdio.h>

// Display

void display_c(contact_t c);

void display_b(book_t b);

// Prompt

contact_t prompt_c();

void prompt_operation();

int main(){
    book_t book;
    prompt_operation(&book);    
}

void display_c(contact_t c){
    printf("===================\n");
    printf("id=%d\n",c.id);
    if(!(strcmp(c.name.firstname,""))) printf("First name:%s\n",c.name.firstname);
    if(!(strcmp(c.name.lastname,""))) printf("Last name:%s\n",c.name.lastname);
    if(!(strcmp(c.job.workplace,""))) printf("Workplace:%s\n",c.job.workplace);
    if(!(strcmp(c.job.role,""))) printf("Role:%s\n",c.job.role);
    if(!(strcmp(c.socials.tg,""))) printf("Telegram:%s\n",c.socials.tg);
    if(!(strcmp(c.socials.ig,""))) printf("Instagram:%s\n",c.socials.ig);
    if(!(strcmp(c.socials.vk,""))) printf("Vkontakte:%s\n",c.socials.vk);
}

void display_b(book_t b){
    if(b.size != 0){
        printf("Displaying book:\n");
        printf("Size:,%d\n",b.size);
        for(int i = 0; i < b.size ; i++) display_c(b.contacts[i]);
    }
}

contact_t prompt_c(){
    contact_t c;
    int id,count;
    char *firstname, *lastname, *workplace, *role, *tg, *ig, *vk;
    
    printf("Contact prompt:\n");
    scanf("Id:%d\n",&id);
    scanf("First name:%s\n",&firstname);
    scanf("Last name:%s\n",&lastname);
    scanf("Additional field selector(0 - all fields):%d\n",&count);
    
    switch (count){
    case 0:
        scanf("Workplace:%s\n",&workplace);
        scanf("Role:%s\n",&role);
        scanf("Telegram:%s\n",&tg);
        scanf("Instagram:%s\n",&ig);
        scanf("Vkontakte:%s\n",&vk);
        c = init_c(id,7,firstname,lastname,workplace,role,tg,ig,vk);
        break;
    
    // TODO : Add more field states(i.e 1 workplace etc.)

    default:
        c = init_c(id,2,firstname,lastname);
        break;

    }
    return c;
}

void prompt_operation(book_t *b){
    
    char operation;
    printf("==== Book app ====\n");
    printf("Controls: a(dd) e(edit) d(elete) v(iew) q(uit)\n");

    while (operation != 'q')
    {
        scanf("Enter key:%c",&operation);
        switch (operation){
        case 'a':
            printf("Add contact:\n");
            *b = add_c(*b,prompt_c());
            break;

        case 'e':
            printf("Edit contact:\n");
            *b = edit_c(*b,prompt_c());
            break;

        case 'd':
            printf("Delete contact:\n");
            *b = delete_c(*b,prompt_c());
            break;

        case 'v':
            printf("View book:\n");
            display_b(*b);
            break;
            
        default:
            break;
        }
    }
    
}