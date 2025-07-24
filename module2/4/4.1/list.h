#ifndef LIST_H_
#define LIST_H_

#include "contact.h"

typedef struct node {
    contact_t *data;
    node_t *next;
    node_t *prev;
} node_t;

typedef struct list {
    int size;
    node_t *head;
    node_t *tail;
    node_t *nodes;
} list_t;

// Operations on list : add,get,remove

// Cases : add first add last add pos, get first get last get pos, delete first delete pos delete last





#endif
