#ifndef FILE_H_
#define FILE_H_

#include "book.h"
#include "contact.h"

int read_book(book_t *book,const char *filename,int *id); // Status code 0 - ok ; -1 - error
int write_book(book_t *book,const char *filename); 

#endif