#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "contact.h"

// Tree initialization
void init_tree(tree_t *tree) {
    tree->root = NULL;
}

// Calculate maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Get height of a node
int height(node_t *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Get balance factor of a node
int get_balance(node_t *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Right rotate subtree rooted with y
node_t *right_rotate(node_t *y) {
    node_t *x = y->left;
    node_t *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate subtree rooted with x
node_t *left_rotate(node_t *x) {
    node_t *y = x->right;
    node_t *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


// Insert a contact into the tree
void insert(tree_t *tree, contact_t *contact) {
    if (!contact) return;
    tree->root = insert_helper(tree->root, contact);
}

// Recursive helper function for insertion
node_t *insert_helper(node_t *node, contact_t *contact) {
    if (!contact) return node;

    // 1. Perform the normal BST insertion
    if (node == NULL) {
        node_t *new_node = (node_t *)malloc(sizeof(node_t));
        new_node->data = contact;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->height = 1;
        return new_node;
    }

    if (contact->id < node->data->id)
        node->left = insert_helper(node->left, contact);
    else if (contact->id > node->data->id)
        node->right = insert_helper(node->right, contact);
    else // Equal ids are not allowed in BST
        return node;

    // 2. Update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    // 3. Get the balance factor
    int balance = get_balance(node);

    // Perform rotations if needed
    // Left Left Case
    if (balance > 1 && contact->id < node->left->data->id)
        return right_rotate(node);

    // Right Right Case
    if (balance < -1 && contact->id > node->right->data->id)
        return left_rotate(node);

    // Left Right Case
    if (balance > 1 && contact->id > node->left->data->id) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && contact->id < node->right->data->id) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

// Find the node with minimum value
node_t *min_value_node(node_t *node) {
    node_t *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete a contact from the tree
void delete(tree_t *tree, int id) {
    tree->root = delete_helper(tree->root, id);
}

// Recursive helper function for deletion
node_t *delete_helper(node_t *root, int id) {
    if (root == NULL)
        return root;

    if (id < root->data->id)
        root->left = delete_helper(root->left, id);
    else if (id > root->data->id)
        root->right = delete_helper(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            node_t *temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            // Free the contact data before freeing the node
            if (temp->data) {
                free_contact(temp->data);
            }
            free(temp);
        } else {
            node_t *temp = min_value_node(root->right);
            // Copy only the data pointer, not the entire contact
            contact_t *temp_contact = root->data;
            root->data = temp->data;
            temp->data = temp_contact;
            root->right = delete_helper(root->right, temp->data->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = get_balance(root);

    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

// Search for a contact in the tree
contact_t *search(tree_t *tree, int id) {
    node_t *current = tree->root;
    while (current != NULL) {
        if (id < current->data->id)
            current = current->left;
        else if (id > current->data->id)
            current = current->right;
        else
            return current->data;
    }
    return NULL;
}

// Balance the entire tree
void balance_tree(tree_t *tree) {
    int count = 0;
    store_nodes(tree->root, NULL, &count);
    
    node_t **nodes = (node_t **)malloc(count * sizeof(node_t *));
    if (!nodes) return;
    
    int index = 0;
    store_nodes(tree->root, nodes, &index);
    
    tree->root = build_balanced_tree(nodes, 0, count - 1);
    free(nodes);
}

// Store nodes in sorted order (in-order traversal)
void store_nodes(node_t *node, node_t **nodes, int *index) {
    if (node == NULL)
        return;
    
    store_nodes(node->left, nodes, index);
    if (nodes != NULL) {
        nodes[*index] = node;
    }
    (*index)++;
    store_nodes(node->right, nodes, index);
}

// Build a balanced tree from sorted nodes
node_t *build_balanced_tree(node_t **nodes, int start, int end) {
    if (start > end)
        return NULL;
    
    int mid = (start + end) / 2;
    node_t *root = nodes[mid];
    
    root->left = build_balanced_tree(nodes, start, mid - 1);
    root->right = build_balanced_tree(nodes, mid + 1, end);
    
    root->height = 1 + max(height(root->left), height(root->right));
    
    return root;
}

// Free the entire tree
void free_tree(tree_t *tree) {
    free_helper(tree->root);
    tree->root = NULL;
}

// Helper function for freeing nodes
void free_helper(node_t *node) {
    if (node != NULL) {
        free_helper(node->left);
        free_helper(node->right);
        
        if (node->data) {
            free_contact(node->data);
        }
        free(node);
    }
}