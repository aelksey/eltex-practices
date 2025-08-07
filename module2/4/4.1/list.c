#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "contact.h"

// Implementation
void init_list(list_t *list) {
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

void free_list(list_t *list) {
    node_t *current = list->head;
    while (current != NULL) {
        node_t *next = current->next;
        free_contact(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void add_first(list_t *list, contact_t *contact) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    new_node->data = contact;
    new_node->prev = NULL;
    new_node->next = list->head;

    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    
    list->head = new_node;
    list->size++;
}

void add_last(list_t *list, contact_t *contact) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    new_node->data = contact;
    new_node->next = NULL;
    new_node->prev = list->tail;
    
    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    
    list->tail = new_node;
    list->size++;
}

void add_at(list_t *list, contact_t *contact, int position) {
    if (position < 0 || position > list->size) {
        fprintf(stderr, "Invalid position\n");
        return;
    }
    
    if (position == 0) {
        add_first(list, contact);
        return;
    }
    
    if (position == list->size) {
        add_last(list, contact);
        return;
    }
    
    node_t *current = list->head;
    for (int i = 0; i < position - 1; i++) {
        current = current->next;
    }
    
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    new_node->data = contact;
    new_node->prev = current;
    new_node->next = current->next;
    
    current->next->prev = new_node;
    current->next = new_node;
    
    list->size++;
}

void delete_first(list_t *list) {
    if (list->head == NULL) return;
    
    node_t *temp = list->head;
    list->head = list->head->next;
    
    if (list->head != NULL) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    
    free_contact(temp->data);
    free(temp);
    list->size--;
}

void delete_last(list_t *list) {
    if (list->tail == NULL) return;
    
    node_t *temp = list->tail;
    list->tail = list->tail->prev;
    
    if (list->tail != NULL) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    
    free_contact(temp->data);
    free(temp);
    list->size--;
}

void delete_at(list_t *list, int position) {
    if (position < 0 || position >= list->size) {
        fprintf(stderr, "Invalid position\n");
        return;
    }
    
    if (position == 0) {
        delete_first(list);
        return;
    }
    
    if (position == list->size - 1) {
        delete_last(list);
        return;
    }
    
    node_t *current = list->head;
    for (int i = 0; i < position; i++) {
        current = current->next;
    }
    
    current->prev->next = current->next;
    current->next->prev = current->prev;
    
    free_contact(current->data);
    free(current);
    list->size--;
}

contact_t *get_first(list_t *list) {
    if (list->head == NULL) return NULL;
    return list->head->data;
}

contact_t *get_last(list_t *list) {
    if (list->tail == NULL) return NULL;
    return list->tail->data;
}

contact_t *get_at(list_t *list, int position) {
    if (position < 0 || position >= list->size) {
        fprintf(stderr, "Invalid position\n");
        return NULL;
    }
    
    node_t *current;
    if (position < list->size / 2) {
        current = list->head;
        for (int i = 0; i < position; i++) {
            current = current->next;
        }
    } else {
        current = list->tail;
        for (int i = list->size - 1; i > position; i--) {
            current = current->prev;
        }
    }
    
    return current->data;
}

void sort_list(list_t *list, int (*compare)(const contact_t *, const contact_t *)) {
    if (list->size <= 1) return;
    
    merge_sort(&list->head, compare);
    
    // Update tail pointer
    node_t *current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    list->tail = current;
}

void merge_sort(node_t **head_ref, int (*compare)(const contact_t *, const contact_t *)) {
    node_t *head = *head_ref;
    node_t *a;
    node_t *b;
    
    if ((head == NULL) || (head->next == NULL)) return;
    
    split_list(head, &a, &b);
    
    merge_sort(&a, compare);
    merge_sort(&b, compare);
    
    *head_ref = merge_sorted_lists(a, b, compare);
    if (*head_ref != NULL) {
        (*head_ref)->prev = NULL;
    }
}

node_t *merge_sorted_lists(node_t *a, node_t *b, int (*compare)(const contact_t *, const contact_t *)) {
    node_t *result = NULL;
    
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    if (compare(a->data, b->data) <= 0) {
        result = a;
        result->next = merge_sorted_lists(a->next, b, compare);
        if (result->next != NULL) {
            result->next->prev = result;
        }
    } else {
        result = b;
        result->next = merge_sorted_lists(a, b->next, compare);
        if (result->next != NULL) {
            result->next->prev = result;
        }
    }
    return result;
}

void split_list(node_t *source, node_t **front_ref, node_t **back_ref) {
    node_t *fast = source->next;
    node_t *slow = source;
    
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    
    *front_ref = source;
    *back_ref = slow->next;
    slow->next = NULL;
    if (*back_ref != NULL) {
        (*back_ref)->prev = NULL;
    }
}




