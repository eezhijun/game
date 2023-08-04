#include "stdio.h"


#define MAX_SIZE 100 // Maximum size of the stack

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

// Function to initialize the stack
static void initializeStack(Stack *stack) {
    stack->top = -1;
}

// Function to check if the stack is empty
static int isEmpty(Stack *stack) {
    return stack->top == -1;
}

// Function to check if the stack is full
static int isFull(Stack *stack) {
    return stack->top == MAX_SIZE - 1;
}

// Function to push an element onto the stack
static void push(Stack *stack, int value) {
    if (isFull(stack)) {
        printf("Error: Stack overflow\n");
        return;
    }

    stack->top++;
    stack->data[stack->top] = value;
}

// Function to pop an element from the stack
static int pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Error: Stack underflow\n");
        return -1; // You can choose an appropriate value to return for underflow.
    }

    int value = stack->data[stack->top];
    stack->top--;
    return value;
}

// Function to get the top element of the stack
static int top(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Error: Stack is empty\n");
        return -1; // You can choose an appropriate value to return for an empty stack.
    }

    return stack->data[stack->top];
}

int test_stack(void)
{
    Stack stack;
    initializeStack(&stack);

    push(&stack, 10);
    push(&stack, 20);
    push(&stack, 30);

    printf("Top element: %d\n", top(&stack));

    printf("Popping elements: ");
    while (!isEmpty(&stack)) {
        printf("%d ", pop(&stack));
    }
    printf("\n");
    return 0;
}
