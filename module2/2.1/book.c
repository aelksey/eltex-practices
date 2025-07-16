#include "book.h"

#include "contact.h"

//

#define CAPACITY 100

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

typedef struct book{
    int size;
    contact_t contacts[CAPACITY];
} book_t;

//

int add_c(book_t *b,contact_t *c) { // Returns status code 1 if addition fails ; returns status code 0 on success
    int status = 0;
    if(b->size >= CAPACITY){
        status = 1;
    }else{
        b->contacts[b->size]=*c;
        b->size++;
        status = 0;
    }
    return status;
}

void delete_c(book_t *b,contact_t *c){
    // TODO : Fill in empty spaces
    for(int i = 0; i < b->size ; i++){
        if(c->id == b->contacts->id){
            empty_c(b->contacts[c->id]);
            b->size--;           
        }
    }
}

void edit_c(book_t *b,contact_t *c,int s,char v[BUFFER]){
    for(int i = 0; i < b->size ; i++){
        if(c->id == b->contacts->id){
            switch (s) {
            case 0:
                b->contacts[c->id].name.firstname = v;
                break;
            case 1:
                /* code */
                break;
            case 2:
                /* code */
                break;
            case 3:
                /* code */
                break;
            case 4:
                /* code */
                break;
            case 5:
                /* code */
                break;
            case 6:
                /* code */
                break;

            default:
                break;
            }
                       
        }
    }
}