#define CAPACITY 100

#include "contact.h"


// ARCH : Prompt asks for operation and manages user input returns contact objects for book
// TODO : Add add , edit, delete , view , operation
// ARCH : Book works as a pseudo dynamic array having it's capacity and size 
// ARCH : if size > capacity can't add and display error
// ARCH : if user tries to edit or delete or add to index > size - display error out of bounds

typedef struct book {    
    int size;
    int capacity;
    contact_t contacts[CAPACITY];
} book_t;

void init_book(book_t *book){
    book->size = 0;
    book->capacity = CAPACITY;
}

void add(book_t *book){
    // TODO : Add error handling
    book->contacts[book->size];
    book->size++;
}

void delete_by_index() {
    // TODO : Add error handling
}

void delete(){
    // TODO : Add error handling
    // TODO : Implement logic
}

void get(int index){

}


