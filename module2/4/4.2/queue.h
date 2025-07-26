#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct Node {
    int data;
    int priority;  // 0 (highest) to 255 (lowest)
    struct Node* next;
} Node;

typedef struct {
    Node* front;
} PriorityQueue;

// Initialize an empty priority queue
void initPriorityQueue(PriorityQueue* pq);

// Check if the queue is empty
int isEmpty(PriorityQueue* pq);

// Enqueue (add) an element with given priority
void enqueue(PriorityQueue* pq, int data, int priority);

// Dequeue (remove and return) the front element (FIFO for same priority)
int dequeue(PriorityQueue* pq);

// Dequeue an element with specific priority
int dequeueWithPriority(PriorityQueue* pq, int priority);

// Dequeue an element with priority not lower than specified (priority <= max_priority)
int dequeueWithMinPriority(PriorityQueue* pq, int max_priority);

// Display the queue contents
void display(PriorityQueue* pq);

// Free all memory used by the queue
void freeQueue(PriorityQueue* pq);

#endif