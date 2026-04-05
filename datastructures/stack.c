#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int stack_init(Stack *stack, size_t elementSize) {
    stack->data = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (stack->data == NULL) {
        return -1;
    }
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->elementSize = elementSize;
    return 0;
}

bool stack_is_empty(const Stack *stack) {
    return stack->count == 0;
}

static int resize(Stack *stack, size_t newCapacity) {
    void **newData = realloc(stack->data, newCapacity * sizeof(void *));
    if (newData == NULL) {
        return -1;
    }
    stack->data = newData;
    stack->capacity = newCapacity;
    return 0;
}

int stack_push(Stack *stack, const void *element) {
    if (stack->count == stack->capacity) {
        if (resize(stack, stack->capacity * 2) != 0)
            return -1;
    }
    void *newElement = malloc(stack->elementSize);
    if (newElement == NULL) {
        return -1;
    }
    memcpy(newElement, element, stack->elementSize);
    stack->data[stack->count++] = newElement;
    return 0;
}

void *stack_pop(Stack *stack) {
    if (stack->count == 0) {
        return NULL;
    }
    void *element = stack->data[--stack->count];

    if (stack->count > 0 && stack->count <= stack->capacity / 4) {
        resize(stack, stack->capacity / 2);  /* best-effort shrink */
    }

    return element;
}

void *stack_peek(const Stack *stack) {
    if (stack->count == 0) {
        return NULL;
    }
    return stack->data[stack->count - 1];
}

size_t stack_size(const Stack *stack) {
    return stack->count;
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
