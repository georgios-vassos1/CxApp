#ifndef STACK_H
#define STACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct {
    char  *data;        /* contiguous buffer, elementSize * capacity bytes */
    size_t count;
    size_t capacity;
    size_t elementSize;
} Stack;

int    stack_init(Stack *stack, size_t elementSize);
int    stack_is_empty(const Stack *stack);
int    stack_push(Stack *stack, const void *element);
int    stack_pop(Stack *stack, void *out);
int    stack_peek(const Stack *stack, void *out);
size_t stack_size(const Stack *stack);
void   stack_free(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
