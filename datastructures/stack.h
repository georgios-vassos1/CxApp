#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Define the initial capacity of the stack
#define INITIAL_CAPACITY 2

typedef struct {
    void **data;       // Pointer to the array holding the stack elements
    int top;           // Index of the top element
    int capacity;      // Current capacity of the stack
    size_t elementSize; // Size of each element in the stack
} Stack;

// Function to initialize the stack
void initStack(Stack *stack, size_t elementSize);

// Function to check if the stack is empty
int isEmpty(Stack *stack);

// Function to resize the stack
void resize(Stack *stack, int newCapacity);

// Function to push an element onto the stack
void push(Stack *stack, void *element);

// Function to pop an element from the stack
void *pop(Stack *stack);

// Function to get the top element of the stack without popping it
void *peek(Stack *stack);

// Function to free the stack memory
void freeStack(Stack *stack);

#endif // STACK_H

