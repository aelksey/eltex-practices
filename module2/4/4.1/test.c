#include <CUnit/Basic.h>
#include "list.h"

// Test cases
void test_init_list(void) {
    list_t list;
    init_list(&list);
    
    CU_ASSERT_EQUAL(list.size, 0);
    CU_ASSERT_PTR_NULL(list.head);
    CU_ASSERT_PTR_NULL(list.tail);
}

void test_add_operations(void) {
    list_t list;
    init_list(&list);
    
    contact_t *c1 = create_contact(1, "John", "Doe", "ACME", "Developer", "john@acme.com");
    contact_t *c2 = create_contact(2, "Jane", "Smith", "XYZ Corp", "Manager", "jane@xyz.com");
    contact_t *c3 = create_contact(3, "Bob", "Johnson", "Tech Inc", "Designer", "bob@tech.com");
    
    add_first(&list, c1);
    CU_ASSERT_EQUAL(list.size, 1);
    CU_ASSERT_EQUAL(get_first(&list)->id, 1);
    
    add_last(&list, c2);
    CU_ASSERT_EQUAL(list.size, 2);
    CU_ASSERT_EQUAL(get_last(&list)->id, 2);
    
    add_at(&list, c3, 1);
    CU_ASSERT_EQUAL(list.size, 3);
    CU_ASSERT_EQUAL(get_at(&list, 1)->id, 3);
    
    free_list(&list);
}

void test_delete_operations(void) {
    list_t list;
    init_list(&list);
    
    contact_t *c1 = create_contact(1, "John", "Doe", "ACME", "Developer", "john@acme.com");
    contact_t *c2 = create_contact(2, "Jane", "Smith", "XYZ Corp", "Manager", "jane@xyz.com");
    contact_t *c3 = create_contact(3, "Bob", "Johnson", "Tech Inc", "Designer", "bob@tech.com");
    
    add_last(&list, c1);
    add_last(&list, c2);
    add_last(&list, c3);
    
    delete_first(&list);
    CU_ASSERT_EQUAL(list.size, 2);
    CU_ASSERT_EQUAL(get_first(&list)->id, 2);
    
    delete_last(&list);
    CU_ASSERT_EQUAL(list.size, 1);
    CU_ASSERT_EQUAL(get_last(&list)->id, 2);
    
    delete_at(&list, 0);
    CU_ASSERT_EQUAL(list.size, 0);
    
    free_list(&list);
}

void test_sort_operations(void) {
    list_t list;
    init_list(&list);
    
    contact_t *c1 = create_contact(3, "Charlie", "Brown", "C Company", "Intern", "charlie@c.com");
    contact_t *c2 = create_contact(1, "Alice", "Adams", "A Inc", "CEO", "alice@a.com");
    contact_t *c3 = create_contact(2, "Bob", "Baker", "B Ltd", "CTO", "bob@b.com");
    
    add_last(&list, c1);
    add_last(&list, c2);
    add_last(&list, c3);
    
    // Sort by ID
    sort_list(&list, compare_by_id);
    CU_ASSERT_EQUAL(get_at(&list, 0)->id, 1);
    CU_ASSERT_EQUAL(get_at(&list, 1)->id, 2);
    CU_ASSERT_EQUAL(get_at(&list, 2)->id, 3);
    
    // Sort by name (surname then name)
    sort_list(&list, compare_by_name);
    CU_ASSERT_STRING_EQUAL(get_at(&list, 0)->name.surname, "Adams");
    CU_ASSERT_STRING_EQUAL(get_at(&list, 1)->name.surname, "Baker");
    CU_ASSERT_STRING_EQUAL(get_at(&list, 2)->name.surname, "Brown");
    
    // Sort by workplace
    sort_list(&list, compare_by_workplace);
    CU_ASSERT_STRING_EQUAL(get_at(&list, 0)->job.workplace, "A Inc");
    CU_ASSERT_STRING_EQUAL(get_at(&list, 1)->job.workplace, "B Ltd");
    CU_ASSERT_STRING_EQUAL(get_at(&list, 2)->job.workplace, "C Company");
    
    free_list(&list);
}

void test_edge_cases(void) {
    list_t list;
    init_list(&list);
    
    // Test empty list operations
    delete_first(&list);
    delete_last(&list);
    delete_at(&list, 0);
    CU_ASSERT_PTR_NULL(get_first(&list));
    CU_ASSERT_PTR_NULL(get_last(&list));
    CU_ASSERT_PTR_NULL(get_at(&list, 0));
    
    // Test single element
    contact_t *c1 = create_contact(1, "Single", "Contact", "Test", "Role", "test@test.com");
    add_first(&list, c1);
    
    delete_last(&list);
    CU_ASSERT_EQUAL(list.size, 0);
    
    free_list(&list);
}

int main() {
    CU_initialize_registry();
    
    CU_pSuite suite = CU_add_suite("Doubly Linked List Tests", NULL, NULL);
    CU_add_test(suite, "Initialization Test", test_init_list);
    CU_add_test(suite, "Add Operations Test", test_add_operations);
    CU_add_test(suite, "Delete Operations Test", test_delete_operations);
    CU_add_test(suite, "Sort Operations Test", test_sort_operations);
    CU_add_test(suite, "Edge Cases Test", test_edge_cases);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}