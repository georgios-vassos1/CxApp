#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_CAPACITY 2

typedef struct {
    void **data;
    size_t count;
    size_t capacity;
    size_t elementSize;
} Stack;

void  stack_init(Stack *stack, size_t elementSize);
bool  stack_is_empty(const Stack *stack);
void  stack_push(Stack *stack, const void *element);
void *stack_pop(Stack *stack);
void *stack_peek(const Stack *stack);
void  stack_free(Stack *stack);

#endif // STACK_H
