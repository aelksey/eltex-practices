#include "contact.h"
#include <stddef.h>
#include <stdlib.h>

static char *copy_string(const char *src) {
    if (src == NULL) return NULL;
    char *dst = strdup(src);
    if (!dst) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return dst;
}

static contact_t *create_contact(int id, const char *name, const char *surname, const char *workplace, const char *role, const char *email) {
    
    contact_t *contact = (contact_t *)malloc(sizeof(contact_t));
    if (!contact) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    contact->id = id;
    contact->name.name = copy_string(name);
    contact->name.surname = copy_string(surname);
    contact->job.workplace = copy_string(workplace);
    contact->job.role = copy_string(role);
    contact->email.email = copy_string(email);
    
    return contact;
}

static void free_contact(contact_t *contact) {
    free(contact->name.name);
    free(contact->name.surname);
    free(contact->job.workplace);
    free(contact->job.role);
    free(contact->email.email);
    free(contact);
}

int compare_by_id(const contact_t *a, const contact_t *b) {return a->id - b->id;}

int compare_by_name(const contact_t *a, const contact_t *b) {
    int surname_cmp = strcmp(a->name.surname, b->name.surname);
    if (surname_cmp != 0) return surname_cmp;
    return strcmp(a->name.name, b->name.name);
}

int compare_by_workplace(const contact_t *a, const contact_t *b) { return strcmp(a->job.workplace, b->job.workplace);}