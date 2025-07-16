#define BOOK_H_
#ifndef BOOK_H_

#define INITIAL_CAPACITY 100

#include "contact.h"

typedef struct book {
    int size;
    int capacity;
    contact_t* contacts;
} book_t;

void add_c(contact_t *c);
void edit_c(contact_t *c,int s);
void delete_c(contact_t *c,int s);
void delete_c(contact_t *c,int s);

#endif