#include "display.h"
#include "contact.h"
#include "tree.h"
#include "stddef.h"
#include <stdio.h>

void display_contact(contact_t *contact){
    printf("ID: %d Name: %s Surname: %s Workplace: %s Role: %s Email: %s\n",
               contact->id,
               contact->name.name,
               contact->name.surname,
               contact->job.workplace,
               contact->job.role,
               contact->email.email);
}

// Print tree in order
void print_in_order(tree_t *tree) {
    in_order_helper(tree->root);
    printf("\n");
}

// Helper function for in-order traversal
void in_order_helper(node_t *node) {
    if (node != NULL) {
        in_order_helper(node->left);
        display_contact(node->data);
        in_order_helper(node->right);
    }
}

// Display the tree
void display_tree(tree_t *tree) {
    if (tree->root == NULL) {
        printf("Tree is empty.\n");
        return;
    }
    display_helper(tree->root, 0);
}

// Helper function for displaying with indentation
void display_helper(node_t *node, int level) {
    if (node != NULL) {
        display_helper(node->right, level + 1);
        
        for (int i = 0; i < level; i++) {
            printf("    ");
        }

        display_contact(node->data);
        
        display_helper(node->left, level + 1);
    }
}