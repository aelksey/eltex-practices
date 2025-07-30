#include "book.h"
#include <string.h>
#include <stdio.h>
#include "file.h"

int count = 0;

# define DEFAULT_PATH "storage.bin"

// Display

void display_c(contact_t c);

void display_b(book_t b);

// Prompt

contact_t prompt_c();

void prompt_operation();

int main(int argc,char *argv[]){
    char filename[BUFFER] = DEFAULT_PATH;
    book_t book;
    
    if(argc > 2)strcpy(filename,argv[1]);

    if(read_book(&book,filename,&count) == -1){perror("Error reading file");}

    prompt_operation(&book,filename);    
}

void display_c(contact_t c){
    printf("===================\n");
    printf("id=%d\n",c.id);
    if(strcmp(c.name.firstname,"") != 0) printf("First name:%s\n",c.name.firstname);
    if(strcmp(c.name.lastname,"") != 0) printf("Last name:%s\n",c.name.lastname);
    if(strcmp(c.job.workplace,"") != 0) printf("Workplace:%s\n",c.job.workplace);
    if(strcmp(c.job.role,"") != 0) printf("Role:%s\n",c.job.role);
    if(strcmp(c.socials.tg,"") != 0) printf("Telegram:%s\n",c.socials.tg);
    if(strcmp(c.socials.ig,"") != 0) printf("Instagram:%s\n",c.socials.ig);
    if(strcmp(c.socials.vk,"") != 0) printf("Vkontakte:%s\n",c.socials.vk);
}

void display_b(book_t b){
    if(b.size != 0){
        printf("Displaying book:\n");
        printf("Size:%d\n",b.size);
        for(int i = 0; i < b.size ; i++) display_c(b.contacts[i]);
    }
}

contact_t prompt_c(){
    contact_t c;
    c = empty_c(c);
    count++;
    int id = count;
    int selector;
    char firstname[BUFFER], lastname[BUFFER], workplace[BUFFER], role[BUFFER], tg[BUFFER], ig[BUFFER], vk[BUFFER];
    
    printf("Contact prompt:\n");

    printf("First name:");
    scanf("%s",firstname);

    printf("Last name:");
    scanf("%s",lastname);
    
    printf("Additional field selector(1 - all fields):");
    scanf("%d",&selector);
    
    switch (selector){
    case 1:

        printf("Workplace:");
        scanf("%s",workplace);
        
        printf("Role:");
        scanf("%s",role);

        printf("Telegram:");
        scanf("%s",tg);

        printf("Instagram:");
        scanf("%s",ig);

        printf("Vkontakte:");
        scanf("%s",vk);
        
        c = init_c(id,7,firstname,lastname,workplace,role,tg,ig,vk);
        break;
    
    // TODO : Add more field states(i.e 1 workplace etc.)

    default:
        c = init_c(id,2,firstname,lastname);
        break;

    }
    return c;
}

void prompt_operation(book_t *b,const char *filename) {
    char operation;
    printf("==== Book app ====\n");
    printf("Controls: a(dd) e(edit) d(elete) v(iew) q(uit)\n");
    
    while (1){
        
        printf("Enter key:");
        scanf("%c",&operation);
        
        if(operation == 'q'){
            if(write_book(b,filename) == -1)perror("Error writing to file");
            break;
        }
        
        if(operation == 'a'){
            printf("Add contact:\n");
            *b = add_c(*b,prompt_c());
        }

        if(operation == 'e'){
            int id;
            printf("Edit contact:\n");
            printf("Id:");
            scanf("%d",&id);
            *b = edit_c(*b,prompt_c(),id);
        }

        if (operation == 'd'){
            int id;
            printf("Delete contact:\n");
            printf("Id:");
            scanf("%d",&id);
            *b = delete_c(*b,id);
        }

        if (operation == 'v'){
            printf("View book:\n");
            display_b(*b);
        }
        
    }
}