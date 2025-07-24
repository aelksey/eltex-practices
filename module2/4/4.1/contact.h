#ifndef CONTACT_H_
#define CONTACT_H_

// Comparator keys

#define NAME      0
#define SURNAME   1
#define WORKPLACE 2
#define ROLE      3
#define EMAIL     4

typedef struct name {   
    char *name;
    char *surname;
} name_t;

typedef struct job {
    char *workplace;
    char *role;
} job_t;

typedef struct email{
    char *email;
}email_t;

typedef struct contact {
    int id;
    name_t name;
    job_t job;
    email_t email;
} contact_t;

void empty_c(contact_t *c);

void init_c(contact_t *c,int id,char *name,char *surname,char *workplace,char *role,char *email);

// MAN : Custom comparator function
// MAN : Return values : 0(objects are equal) 1(a > b) -1 (b > a)
int cmp_c(contact_t *c1,contact_t *c2,int key); 

#endif