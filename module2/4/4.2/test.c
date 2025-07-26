#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "queue.h"

// Test suite initialization
int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

// Test cases
void test_queue_initialization(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    CU_ASSERT(isEmpty(&pq));
}

void test_enqueue_dequeue(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    
    enqueue(&pq, 10, 1);
    CU_ASSERT_FALSE(isEmpty(&pq));
    
    int value = dequeue(&pq);
    CU_ASSERT_EQUAL(value, 10);
    CU_ASSERT(isEmpty(&pq));
    
    freeQueue(&pq);
}

void test_priority_ordering(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    
    enqueue(&pq, 10, 2);
    enqueue(&pq, 20, 1);  // Higher priority
    enqueue(&pq, 30, 3);
    
    CU_ASSERT_EQUAL(dequeue(&pq), 20);  // Highest priority first
    CU_ASSERT_EQUAL(dequeue(&pq), 10);
    CU_ASSERT_EQUAL(dequeue(&pq), 30);
    
    freeQueue(&pq);
}

void test_dequeue_with_priority(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    
    enqueue(&pq, 10, 1);
    enqueue(&pq, 20, 2);
    enqueue(&pq, 30, 1);
    enqueue(&pq, 40, 3);
    
    CU_ASSERT_EQUAL(dequeueWithPriority(&pq, 2), 20);
    CU_ASSERT_EQUAL(dequeueWithPriority(&pq, 1), 10);  // First of priority 1
    CU_ASSERT_EQUAL(dequeueWithPriority(&pq, 1), 30);  // Second of priority 1
    
    freeQueue(&pq);
}

void test_dequeue_with_min_priority(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    
    enqueue(&pq, 10, 5);
    enqueue(&pq, 20, 3);
    enqueue(&pq, 30, 7);
    enqueue(&pq, 40, 2);
    
    CU_ASSERT_EQUAL(dequeueWithMinPriority(&pq, 3), 40);  // Highest priority <= 3
    CU_ASSERT_EQUAL(dequeueWithMinPriority(&pq, 4), 20);  // Highest priority <= 4
    
    freeQueue(&pq);
}

void test_empty_queue_operations(void) {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    
    CU_ASSERT_EQUAL(dequeue(&pq), -1);
    CU_ASSERT_EQUAL(dequeueWithPriority(&pq, 1), -1);
    CU_ASSERT_EQUAL(dequeueWithMinPriority(&pq, 5), -1);
    
    freeQueue(&pq);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Priority Queue Tests", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "test_queue_initialization", test_queue_initialization)) ||
        (NULL == CU_add_test(pSuite, "test_enqueue_dequeue", test_enqueue_dequeue)) ||
        (NULL == CU_add_test(pSuite, "test_priority_ordering", test_priority_ordering)) ||
        (NULL == CU_add_test(pSuite, "test_dequeue_with_priority", test_dequeue_with_priority)) ||
        (NULL == CU_add_test(pSuite, "test_dequeue_with_min_priority", test_dequeue_with_min_priority)) ||
        (NULL == CU_add_test(pSuite, "test_empty_queue_operations", test_empty_queue_operations))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}