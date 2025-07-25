#include "display.h"
#include "stddef.h"

void display_contact(contact_t *contact){
    printf("  ID: %d, Name: %s %s, Workplace: %s, Role: %s, Email: %s\n",
               contact->id,
               contact->name.name,
               contact->name.surname,
               contact->job.workplace,
               contact->job.role,
               contact->email.email);
}

void display_list(list_t *list) {
    node_t *current = list->head;
    printf("List (size %d):\n", list->size);
    while (current != NULL) {
        display_contact(current->data);
        current = current->next;
    }
}