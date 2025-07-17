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
    int index;
    for(int i = 0; i < b.size ; i++){
        if(c.id == b.contacts[i].id){
            index = i;
            empty_c(b.contacts[i]);
            b.size--;           
        }
    }
    if(index < b.size){
        for(int i = index ; i < b.size ; i++){
            contact_t temp_c = b.contacts[i];
            b.contacts[i-1] = temp_c;
        }
    } // move everything right to one element backwards - no empty spaces
    return b;
}

book_t edit_c(book_t b,contact_t c){
      for(int i = 0; i < b.size ; i++){
        if(c.id == b.contacts[i].id){
            b.contacts[i] = c;
        }
    }
    return b;
}
