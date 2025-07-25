#ifndef LIST_H_
#define LIST_H_

#include "contact.h"

typedef struct node {
    contact_t *data;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct list {
    int size;
    node_t *head;
    node_t *tail;
} list_t;

// Function prototypes

void init_list(list_t *list);
void free_list(list_t *list);
void add_first(list_t *list, contact_t *contact);
void add_last(list_t *list, contact_t *contact);
void add_at(list_t *list, contact_t *contact, int position);
void delete_first(list_t *list);
void delete_last(list_t *list);
void delete_at(list_t *list, int position);
contact_t *get_first(list_t *list);
contact_t *get_last(list_t *list);
contact_t *get_at(list_t *list, int position);
void sort_list(list_t *list, int (*compare)(const contact_t *, const contact_t *));

// Helper functions

static node_t *merge_sorted_lists(node_t *a, node_t *b, int (*compare)(const contact_t *, const contact_t *));
static void merge_sort(node_t **head_ref, int (*compare)(const contact_t *, const contact_t *));
static void split_list(node_t *source, node_t **front_ref, node_t **back_ref);

#endif