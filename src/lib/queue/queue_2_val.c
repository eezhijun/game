#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100 // Maximum size of the queue

typedef struct
{
    int data[MAX_SIZE];
    int front;
    int rear;
} Queue;

// Function to initialize the queue
void queue_init(Queue *queue)
{
    queue->front = -1;
    queue->rear = -1;
}

// Function to check if the queue is empty
bool queue_is_empty(const Queue *queue)
{
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
bool queue_is_full(const Queue *queue)
{
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

// Function to enqueue (push) an element into the queue
void enqueue(Queue *queue, int value)
{
    if (queue_is_full(queue))
    {
        fprintf(stderr, "Error: Queue overflow\n");
        return;
    }

    if (queue_is_empty(queue))
        queue->front = queue->rear = 0;
    else
        queue->rear = (queue->rear + 1) % MAX_SIZE;

    queue->data[queue->rear] = value;
}

// Function to dequeue (pop) an element from the queue
int dequeue(Queue *queue)
{
    if (queue_is_empty(queue))
    {
        fprintf(stderr, "Error: Queue underflow\n");
        return -1; // You can choose an appropriate value to return for underflow.
    }

    int value = queue->data[queue->front];

    if (queue->front == queue->rear) // If it's the last element in the queue
        queue_init(queue);
    else
        queue->front = (queue->front + 1) % MAX_SIZE;

    return value;
}

// Function to get the front element of the queue
int front(const Queue *queue)
{
    if (queue_is_empty(queue))
    {
        fprintf(stderr, "Error: Queue is empty\n");
        return -1; // You can choose an appropriate value to return for an empty queue.
    }

    return queue->data[queue->front];
}

int queue_test(void)
{
    Queue queue;
    queue_init(&queue);

    enqueue(&queue, 10);
    enqueue(&queue, 20);
    enqueue(&queue, 30);

    printf("Front element: %d\n", front(&queue));

    printf("Dequeuing elements: ");
    while (!queue_is_empty(&queue))
    {
        printf("%d ", dequeue(&queue));
    }
    printf("\n");

    return 0;
}
