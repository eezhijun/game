#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100 // Maximum size of the stack

typedef struct
{
    int data[MAX_SIZE];
    int top;
} Stack;

// Function to initialize the stack
void stack_init(Stack *stack)
{
    stack->top = -1;
}

// Function to check if the stack is empty
bool stack_is_empty(const Stack *stack)
{
    return stack->top == -1;
}

// Function to check if the stack is full
bool stack_is_full(const Stack *stack)
{
    return stack->top == MAX_SIZE - 1;
}

// Function to push an element onto the stack
void stack_push(Stack *stack, int value)
{
    if (stack_is_full(stack))
    {
        fprintf(stderr, "Error: Stack overflow\n");
        return;
    }

    stack->top++;
    stack->data[stack->top] = value;
}

// Function to pop an element from the stack
int stack_pop(Stack *stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Error: Stack underflow\n");
        return -1; // You can choose an appropriate value to return for underflow.
    }

    int value = stack->data[stack->top];
    stack->top--;
    return value;
}

// Function to get the top element of the stack
int stack_top(const Stack *stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Error: Stack is empty\n");
        return -1; // You can choose an appropriate value to return for an empty stack.
    }

    return stack->data[stack->top];
}

int stack_test(void)
{
    Stack stack;
    stack_init(&stack);

    stack_push(&stack, 10);
    stack_push(&stack, 20);
    stack_push(&stack, 30);

    printf("Top element: %d\n", stack_top(&stack));

    printf("Popping elements: ");
    while (!stack_is_empty(&stack))
    {
        printf("%d ", stack_pop(&stack));
    }
    printf("\n");

    return 0;
}
