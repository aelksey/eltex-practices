#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "contact.h"
#include "tree.h"

void display_contact(contact_t *contact);

void print_in_order(tree_t *tree);
void in_order_helper(node_t *node);

void display_tree(tree_t *tree);
void display_helper(node_t *node, int level);


#endif