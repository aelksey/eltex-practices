#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"

// Test program
int main() {
    PriorityQueue pq;
    initPriorityQueue(&pq);
    srand(time(0));  // Seed for random number generation

    // Test 1: Enqueue elements with random priorities
    printf("Adding 10 elements with random priorities (0-255)...\n");
    for (int i = 1; i <= 10; i++) {
        int priority = rand() % 256;
        enqueue(&pq, i * 10, priority);
        printf("Added %d with priority %d\n", i * 10, priority);
    }
    display(&pq);

    // Test 2: Dequeue normally (FIFO for same priority)
    printf("\nDequeue normally (FIFO for same priority):\n");
    while (!isEmpty(&pq)) {
        printf("Dequeued: %d\n", dequeue(&pq));
    }

    // Test 3: Enqueue specific elements for priority tests
    printf("\nAdding specific elements for priority tests...\n");
    enqueue(&pq, 100, 10);
    enqueue(&pq, 200, 20);
    enqueue(&pq, 300, 10);
    enqueue(&pq, 400, 30);
    enqueue(&pq, 500, 20);
    display(&pq);

    // Test 4: Dequeue with specific priority (20)
    printf("\nDequeue with priority 20:\n");
    printf("Dequeued: %d\n", dequeueWithPriority(&pq, 20));
    display(&pq);

    // Test 5: Dequeue with priority not lower than 15 (priority <= 15)
    printf("\nDequeue with priority not lower than 15:\n");
    printf("Dequeued: %d\n", dequeueWithMinPriority(&pq, 15));
    display(&pq);

    // Free remaining memory
    freeQueue(&pq);

    return 0;
}