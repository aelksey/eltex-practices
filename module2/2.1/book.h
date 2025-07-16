#define BOOK_H_
#ifndef BOOK_H_

#include "contact.h"

#define CAPACITY 100


typedef struct book{
    int size;
    contact_t contacts[CAPACITY];
} book_t;

void add_c(book_t *b,contact_t *c);
void delete_c(book_t *b,contact_t *c);
void edit_c(book_t *b,contact_t *c,int s,char *v);

#endif