#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "tree.h"
#include "contact.h"
#include "display.h"

// Test suite initialization and cleanup
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

// Test cases
void test_tree_init(void) {
    tree_t tree;
    init_tree(&tree);
    CU_ASSERT_PTR_NULL(tree.root);
}

void test_insert_and_search(void) {
    tree_t tree;
    init_tree(&tree);
    
    contact_t *contact1 = create_contact(1, "John", "Doe", "Company", "Developer", "john@example.com");
    contact_t *contact2 = create_contact(2, "Jane", "Smith", "Corp", "Manager", "jane@example.com");
    contact_t *contact3 = create_contact(3, "Bob", "Johnson", "Inc", "CEO", "bob@example.com");
    
    CU_ASSERT_PTR_NOT_NULL(contact1);
    CU_ASSERT_PTR_NOT_NULL(contact2);
    CU_ASSERT_PTR_NOT_NULL(contact3);
    
    insert(&tree, contact1);
    insert(&tree, contact2);
    insert(&tree, contact3);
    
    node_t *node1 = search(&tree, 1);
    CU_ASSERT_PTR_NOT_NULL(node1);
    CU_ASSERT_EQUAL(node1->data->id, 1);
    CU_ASSERT_STRING_EQUAL(node1->data->name.name, "John");
    
    node_t *node2 = search(&tree, 2);
    CU_ASSERT_PTR_NOT_NULL(node2);
    CU_ASSERT_EQUAL(node2->data->id, 2);
    
    node_t *node3 = search(&tree, 3);
    CU_ASSERT_PTR_NOT_NULL(node3);
    CU_ASSERT_EQUAL(node3->data->id, 3);
    
    node_t *node4 = search(&tree, 4);
    CU_ASSERT_PTR_NULL(node4);
    
    free_tree(&tree);
}

void test_delete(void) {
    tree_t tree;
    init_tree(&tree);
    
    contact_t *contact1 = create_contact(1, "John", "Doe", "Company", "Developer", "john@example.com");
    contact_t *contact2 = create_contact(2, "Jane", "Smith", "Corp", "Manager", "jane@example.com");
    contact_t *contact3 = create_contact(3, "Bob", "Johnson", "Inc", "CEO", "bob@example.com");
    
    insert(&tree, contact1);
    insert(&tree, contact2);
    insert(&tree, contact3);
    
    delete(&tree, 2);
    node_t *node = search(&tree, 2);
    CU_ASSERT_PTR_NULL(node);
    
    node = search(&tree, 1);
    CU_ASSERT_PTR_NOT_NULL(node);
    node = search(&tree, 3);
    CU_ASSERT_PTR_NOT_NULL(node);
    
    delete(&tree, 4); // Should not crash
    
    free_tree(&tree);
}

void test_balance_tree(void) {
    tree_t tree;
    init_tree(&tree);
    
    // Create a skewed tree
    for (int i = 1; i <= 10; i++) {
        char name[20], email[30];
        sprintf(name, "Name%d", i);
        sprintf(email, "name%d@example.com", i);
        contact_t *contact = create_contact(i, name, "Surname", "Work", "Role", email);
        insert(&tree, contact);
    }
    
    balance_tree(&tree);
    CU_ASSERT_TRUE(height(tree.root) <= 4); // For 10 nodes, balanced height should be 4 or less
    
    free_tree(&tree);
}

void test_free_tree(void) {
    tree_t tree;
    init_tree(&tree);
    
    contact_t *contact1 = create_contact(1, "John", "Doe", "Company", "Developer", "john@example.com");
    contact_t *contact2 = create_contact(2, "Jane", "Smith", "Corp", "Manager", "jane@example.com");
    
    insert(&tree, contact1);
    insert(&tree, contact2);
    
    free_tree(&tree);
    CU_ASSERT_PTR_NULL(tree.root);
    
    // Verify we can still use the tree after freeing
    contact_t *contact3 = create_contact(3, "Bob", "Johnson", "Inc", "CEO", "bob@example.com");
    insert(&tree, contact3);
    node_t *node = search(&tree, 3);
    CU_ASSERT_PTR_NOT_NULL(node);
    
    free_tree(&tree);
}

void test_display_tree(void) {
    tree_t tree;
    init_tree(&tree);
    
    contact_t *contact1 = create_contact(50, "John", "Doe", "Company", "Developer", "john@example.com");
    contact_t *contact2 = create_contact(30, "Jane", "Smith", "Corp", "Manager", "jane@example.com");
    contact_t *contact3 = create_contact(70, "Bob", "Johnson", "Inc", "CEO", "bob@example.com");
    contact_t *contact4 = create_contact(20, "Alice", "Brown", "Tech", "Engineer", "alice@example.com");
    contact_t *contact5 = create_contact(40, "Charlie", "Wilson", "Soft", "Designer", "charlie@example.com");
    contact_t *contact6 = create_contact(60, "David", "Lee", "Data", "Scientist", "david@example.com");
    contact_t *contact7 = create_contact(80, "Eve", "Taylor", "Cloud", "Architect", "eve@example.com");
    
    insert(&tree, contact1);
    insert(&tree, contact2);
    insert(&tree, contact3);
    insert(&tree, contact4);
    insert(&tree, contact5);
    insert(&tree, contact6);
    insert(&tree, contact7);
    
    printf("\nDisplaying the tree:\n");
    display_tree(&tree);
    
    free_tree(&tree);
}

int main() {
    CU_pSuite pSuite = NULL;
    
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    pSuite = CU_add_suite("Binary Tree Test Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if ((NULL == CU_add_test(pSuite, "test_tree_init", test_tree_init)) ||
        (NULL == CU_add_test(pSuite, "test_insert_and_search", test_insert_and_search)) ||
        (NULL == CU_add_test(pSuite, "test_delete", test_delete)) ||
        (NULL == CU_add_test(pSuite, "test_balance_tree", test_balance_tree)) ||
        (NULL == CU_add_test(pSuite, "test_free_tree", test_free_tree)) ||
        (NULL == CU_add_test(pSuite, "test_display_tree", test_display_tree))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}