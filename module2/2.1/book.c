#include "contact.c"

#include "string.h"

#define CAPACITY 100

typedef struct book{
    int size;
    contact_t contacts[CAPACITY];
} book_t;

book_t add_c(book_t b,contact_t c){
    if (!(b.size >= CAPACITY)){
        b.contacts[b.size]=c;
        b.size++;
    }    
    return b;
}

book_t delete_c(book_t b,contact_t c){
    // TODO : Fill in empty spaces
    for(int i = 0; i < b.size ; i++){
        if(c.id == b.contacts[i].id){
            empty_c(b.contacts[c.id]);
            b.size--;           
        }
    }
    return b;
}

book_t edit_c(book_t b,contact_t c,int s,char v[]){
      for(int i = 0; i < b.size ; i++){
        if(c.id == b.contacts[i].id){
            switch (s) {
            case 0:
                strcpy(b.contacts[c.id].name.firstname,v);
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
