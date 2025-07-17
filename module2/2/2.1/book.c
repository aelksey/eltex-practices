#include "book.h"

#include "string.h"

book_t add_c(book_t b,contact_t c){

    if (!(b.size >= CAPACITY)){
        b.contacts[b.size]=c;
        b.size++;
    }

    return b;
}

book_t delete_c(book_t b,int id){
    
    int index = id - 1;

    b.contacts[index] = empty_c(b.contacts[index]);
    
    if(index < b.size){
        for(int i = index ; i < b.size - 1 ; i++){
            b.contacts[i] = b.contacts[i+1];
        }
    }

    b.size--;

    return b;
}

book_t edit_c(book_t b,contact_t c,int id){

    int index = id - 1;

    b.contacts[index] = c;

    return b;
}
