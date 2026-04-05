#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void) {
    Stack stack;
    if (stack_init(&stack, sizeof(int)) != 0) {
        fprintf(stderr, "stack_init failed\n");
        return 1;
    }

    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    if (stack_push(&stack, &value1) != 0) goto fail;
    if (stack_push(&stack, &value2) != 0) goto fail;
    if (stack_push(&stack, &value3) != 0) goto fail;

    int popped;
    if (stack_pop(&stack, &popped) == 0)
        printf("Popped: %d\n", popped);

    if (stack_pop(&stack, &popped) == 0)
        printf("Popped: %d\n", popped);

    int value4 = 40;
    if (stack_push(&stack, &value4) != 0) goto fail;

    int top;
    if (stack_peek(&stack, &top) == 0)
        printf("Top element: %d\n", top);

    printf("Stack size: %zu\n", stack_size(&stack));

    stack_free(&stack);
    return 0;

fail:
    fprintf(stderr, "stack operation failed\n");
    stack_free(&stack);
    return 1;
}
