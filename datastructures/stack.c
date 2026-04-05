#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void stack_init(Stack *stack, size_t elementSize) {
    stack->data = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (stack->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->elementSize = elementSize;
}

bool stack_is_empty(const Stack *stack) {
    return stack->count == 0;
}

static void resize(Stack *stack, size_t newCapacity) {
    void **newData = realloc(stack->data, newCapacity * sizeof(void *));
    if (newData == NULL) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->data = newData;
    stack->capacity = newCapacity;
}

void stack_push(Stack *stack, const void *element) {
    if (stack->count == stack->capacity) {
        resize(stack, stack->capacity * 2);
    }
    void *newElement = malloc(stack->elementSize);
    if (newElement == NULL) {
        fprintf(stderr, "Memory allocation for new element failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newElement, element, stack->elementSize);
    stack->data[stack->count++] = newElement;
}

void *stack_pop(Stack *stack) {
    if (stack->count == 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    void *element = stack->data[--stack->count];

    if (stack->count > 0 && stack->count <= stack->capacity / 4) {
        resize(stack, stack->capacity / 2);
    }

    return element;
}

void *stack_peek(const Stack *stack) {
    if (stack->count == 0) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->count - 1];
}

void stack_free(Stack *stack) {
    for (size_t i = 0; i < stack->count; i++) {
        free(stack->data[i]);
    }
    free(stack->data);
    stack->data = NULL;
    stack->count = 0;
    stack->capacity = 0;
}
