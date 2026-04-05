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
    stack_push(&stack, &value1);
    stack_push(&stack, &value2);
    stack_push(&stack, &value3);

    int *poppedValue = (int *)stack_pop(&stack);
    if (poppedValue) {
        printf("Popped: %d\n", *poppedValue);
        free(poppedValue);
    }

    poppedValue = (int *)stack_pop(&stack);
    if (poppedValue) {
        printf("Popped: %d\n", *poppedValue);
        free(poppedValue);
    }

    int value4 = 40;
    stack_push(&stack, &value4);

    int *topValue = (int *)stack_peek(&stack);
    if (topValue)
        printf("Top element: %d\n", *topValue);

    printf("Stack size: %zu\n", stack_size(&stack));

    stack_free(&stack);

    return 0;
}
