#ifndef CONTACT_H_
#define CONTACT_H_

typedef struct name {
    char *name;
    char *surname;
} name_t;

typedef struct job {
    char *workplace;
    char *role;
} job_t;

typedef struct email {
    char *email;
} email_t;

typedef struct contact {
    int id;
    name_t name;
    job_t job;
    email_t email;
} contact_t;

char *copy_string(const char *src);

contact_t *create_contact(int id, const char *name, const char *surname, const char *workplace, const char *role, const char *email);

void free_contact(contact_t *contact);

int compare_by_id(const contact_t *a, const contact_t *b);
int compare_by_name(const contact_t *a, const contact_t *b);
int compare_by_surname(const contact_t *a, const contact_t *b);
int compare_by_workplace(const contact_t *a, const contact_t *b);
int compare_by_role(const contact_t *a, const contact_t *b);
int compare_by_email(const contact_t *a, const contact_t *b);

#endif