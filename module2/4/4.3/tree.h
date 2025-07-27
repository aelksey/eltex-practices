#ifndef TREE_H_
#define TREE_H_

#include "contact.h"

// Binary tree node structure
typedef struct node_t {
    contact_t *data;
    struct node_t *left;
    struct node_t *right;
    int height;
} node_t;

typedef struct tree_t {
    node_t *root;
} tree_t;

// Function prototypes
void init_tree(tree_t *tree);
void insert(tree_t *tree, contact_t *contact);
void delete(tree_t *tree, int id);
contact_t *search(tree_t *tree, int id);
void balance_tree(tree_t *tree);
void free_tree(tree_t *tree);

// Helper functions
int max(int a, int b);
int height(node_t *node);
int get_balance(node_t *node);
node_t *right_rotate(node_t *y);
node_t *left_rotate(node_t *x);
node_t *insert_helper(node_t *node, contact_t *contact);
node_t *min_value_node(node_t *node);
node_t *delete_helper(node_t *root, int id);
node_t *balance_helper(node_t *node);
void store_nodes(node_t *node, node_t **nodes, int *index);
node_t *build_balanced_tree(node_t **nodes, int start, int end);
void free_helper(node_t *node);


#endif