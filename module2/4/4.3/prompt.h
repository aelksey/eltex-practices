#ifndef PROMPT_H_
#define PROMPT_H_

#include "contact.h"

contact_t *prompt_create_c();
void prompt_edit_c(contact_t *c);
int prompt_remove_c(contact_t *c);

#endif