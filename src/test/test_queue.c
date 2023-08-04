#include <stdio.h>

#define MAX_SIZE 100 // Maximum size of the queue

typedef struct {
    int data[MAX_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
static void initializeQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// Function to check if the queue is empty
static int isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
static int isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

// Function to enqueue (push) an element into the queue
static void enqueue(Queue *queue, int value) {
    if (isFull(queue)) {
        printf("Error: Queue overflow\n");
        return;
    }

    if (isEmpty(queue))
        queue->front = queue->rear = 0;
    else
        queue->rear = (queue->rear + 1) % MAX_SIZE;

    queue->data[queue->rear] = value;
}

// Function to dequeue (pop) an element from the queue
static int dequeue(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Error: Queue underflow\n");
        return -1; // You can choose an appropriate value to return for underflow.
    }

    int value = queue->data[queue->front];

    if (queue->front == queue->rear) // If it's the last element in the queue
        initializeQueue(queue);
    else
        queue->front = (queue->front + 1) % MAX_SIZE;

    return value;
}

// Function to get the front element of the queue
static int front(Queue *queue) {
    if (isEmpty(queue)) {
        printf("Error: Queue is empty\n");
        return -1; // You can choose an appropriate value to return for an empty queue.
    }

    return queue->data[queue->front];
}

int test_queue(void)
{
    Queue queue;
    initializeQueue(&queue);

    enqueue(&queue, 10);
    enqueue(&queue, 20);
    enqueue(&queue, 30);

    printf("Front element: %d\n", front(&queue));

    printf("Dequeue elements: ");
    while (!isEmpty(&queue)) {
        printf("%d ", dequeue(&queue));
    }
    printf("\n");

    return 0;
}
