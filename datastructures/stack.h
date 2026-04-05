#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_CAPACITY 2

typedef struct {
    void **data;
    size_t count;
    size_t capacity;
    size_t elementSize;
} Stack;

int    stack_init(Stack *stack, size_t elementSize);
bool   stack_is_empty(const Stack *stack);
int    stack_push(Stack *stack, const void *element);
void  *stack_pop(Stack *stack);
void  *stack_peek(const Stack *stack);
size_t stack_size(const Stack *stack);
void   stack_free(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif // STACK_H
