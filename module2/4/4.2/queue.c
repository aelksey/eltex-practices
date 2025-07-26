#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Initialize an empty priority queue
void initPriorityQueue(PriorityQueue* pq) {
    pq->front = NULL;
}

// Check if the queue is empty
int isEmpty(PriorityQueue* pq) {
    return pq->front == NULL;
}

// Enqueue (add) an element with given priority
void enqueue(PriorityQueue* pq, int data, int priority) {
    if (priority < 0 || priority > 255) {
        printf("Priority must be between 0 and 255\n");
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->priority = priority;
    newNode->next = NULL;

    // If queue is empty or new node has higher priority than front
    if (isEmpty(pq) || priority < pq->front->priority) {
        newNode->next = pq->front;
        pq->front = newNode;
    } else {
        Node* current = pq->front;
        // Find the correct position based on priority
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Dequeue (remove and return) the front element (FIFO for same priority)
int dequeue(PriorityQueue* pq) {
    if (isEmpty(pq)) {
        printf("Queue is empty\n");
        return -1;
    }
    Node* temp = pq->front;
    int data = temp->data;
    pq->front = pq->front->next;
    free(temp);
    return data;
}

// Dequeue an element with specific priority
int dequeueWithPriority(PriorityQueue* pq, int priority) {
    if (isEmpty(pq)) {
        printf("Queue is empty\n");
        return -1;
    }

    Node* current = pq->front;
    Node* prev = NULL;

    // Find the first node with matching priority
    while (current != NULL && current->priority != priority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("No element with priority %d found\n", priority);
        return -1;
    }

    int data = current->data;
    if (prev == NULL) {
        pq->front = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    return data;
}

// Dequeue an element with priority not lower than specified (priority <= max_priority)
int dequeueWithMinPriority(PriorityQueue* pq, int max_priority) {
    if (isEmpty(pq)) {
        printf("Queue is empty\n");
        return -1;
    }

    Node* current = pq->front;
    Node* prev = NULL;

    // Find the first node with priority <= max_priority
    while (current != NULL && current->priority > max_priority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("No element with priority <= %d found\n", max_priority);
        return -1;
    }

    int data = current->data;
    if (prev == NULL) {
        pq->front = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
    return data;
}

// Display the queue contents
void display(PriorityQueue* pq) {
    if (isEmpty(pq)) {
        printf("Queue is empty\n");
        return;
    }

    Node* current = pq->front;
    printf("Queue contents (data:priority): ");
    while (current != NULL) {
        printf("%d:%d ", current->data, current->priority);
        current = current->next;
    }
    printf("\n");
}

// Free all memory used by the queue
void freeQueue(PriorityQueue* pq) {
    Node* current = pq->front;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    pq->front = NULL;
}