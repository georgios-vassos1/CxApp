#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define INITIAL_CAPACITY 2

int stack_init(Stack *stack, size_t elementSize) {
    if (!stack || elementSize == 0) return -1;
    stack->data = malloc(INITIAL_CAPACITY * elementSize);
    if (!stack->data) return -1;
    stack->count = 0;
    stack->capacity = INITIAL_CAPACITY;
    stack->elementSize = elementSize;
    return 0;
}

int stack_is_empty(const Stack *stack) {
    return !stack || stack->count == 0;
}

static int resize(Stack *stack, size_t newCapacity) {
    if (newCapacity > SIZE_MAX / stack->elementSize) return -1;
    char *newData = realloc(stack->data, newCapacity * stack->elementSize);
    if (!newData) return -1;
    stack->data = newData;
    stack->capacity = newCapacity;
    return 0;
}

int stack_push(Stack *stack, const void *element) {
    if (!stack || !element) return -1;
    if (stack->count == stack->capacity) {
        if (stack->capacity > SIZE_MAX / 2) return -1;
        if (resize(stack, stack->capacity * 2) != 0) return -1;
    }
    memcpy(stack->data + stack->count * stack->elementSize,
           element, stack->elementSize);
    stack->count++;
    return 0;
}

int stack_pop(Stack *stack, void *out) {
    if (!stack || stack->count == 0) return -1;
    stack->count--;
    if (out)
        memcpy(out, stack->data + stack->count * stack->elementSize,
               stack->elementSize);
    if (stack->count > 0 && stack->count <= stack->capacity / 4)
        resize(stack, stack->capacity / 2);  /* best-effort shrink */
    return 0;
}

int stack_peek(const Stack *stack, void *out) {
    if (!stack || stack->count == 0 || !out) return -1;
    memcpy(out, stack->data + (stack->count - 1) * stack->elementSize,
           stack->elementSize);
    return 0;
}

size_t stack_size(const Stack *stack) {
    return stack ? stack->count : 0;
}

void stack_free(Stack *stack) {
    if (!stack) return;
    free(stack->data);
    stack->data = NULL;
    stack->count = 0;
    stack->capacity = 0;
    stack->elementSize = 0;
}
