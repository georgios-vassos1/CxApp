#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"


// Function to initialize the stack
void initStack(Stack *stack, size_t elementSize) {
    stack->data = (void **)malloc(INITIAL_CAPACITY * sizeof(void *));
    if (stack->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    stack->capacity = INITIAL_CAPACITY;
    stack->elementSize = elementSize;
}

// Function to check if the stack is empty
int isEmpty(Stack *stack) {
    return stack->top == -1;
}

// Function to resize the stack
void resize(Stack *stack, int newCapacity) {
    void **newData = (void **)realloc(stack->data, newCapacity * sizeof(void *));
    if (newData == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->data = newData;
    stack->capacity = newCapacity;
}

// Function to push an element onto the stack
void push(Stack *stack, void *element) {
    // If the stack is full, double its capacity
    if (stack->top == stack->capacity - 1) {
        resize(stack, stack->capacity * 2);
    }
    void *newElement = malloc(stack->elementSize);
    if (newElement == NULL) {
        fprintf(stderr, "Memory allocation for new element failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newElement, element, stack->elementSize);
    stack->data[++stack->top] = newElement;
}

// Function to pop an element from the stack
void *pop(Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    void *element = stack->data[stack->top--];
    
    // If the stack is less than a quarter full, halve its capacity
    if (stack->top + 1 > 0 && stack->top + 1 <= stack->capacity / 4) {
        resize(stack, stack->capacity / 2);
    }

    return element;
}

// Function to get the top element of the stack without popping it
void *peek(Stack *stack) {
    if (isEmpty(stack)) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top];
}

// Function to free the stack memory
void freeStack(Stack *stack) {
    while (!isEmpty(stack)) {
        free(pop(stack));
    }
    free(stack->data);
    stack->data = NULL;
    stack->top = -1;
    stack->capacity = 0;
}

