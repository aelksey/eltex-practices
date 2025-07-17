#pragma once

#include "contact.h"

#define CAPACITY 100

typedef struct book{
    int size;
    contact_t contacts[CAPACITY];
} book_t;

book_t add_c(book_t b,contact_t c);
book_t delete_c(book_t b,int id);
book_t edit_c(book_t b,contact_t c, int id);
