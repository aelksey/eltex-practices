#define BOOK_H_
#ifndef BOOK_H_

#include "contact.h"

#define CAPACITY 100


typedef struct book book_t;

book_t add_c(book_t b,contact_t c);
book_t delete_c(book_t b,contact_t c);
book_t edit_c(book_t b,contact_t c,int s,char v[]);

#endif