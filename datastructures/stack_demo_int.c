#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main(void) {
    Stack stack;
    stack_init(&stack, sizeof(int));

    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    stack_push(&stack, &value1);
    stack_push(&stack, &value2);
    stack_push(&stack, &value3);

    int *poppedValue = (int *)stack_pop(&stack);
    printf("Popped: %d\n", *poppedValue);
    free(poppedValue);

    poppedValue = (int *)stack_pop(&stack);
    printf("Popped: %d\n", *poppedValue);
    free(poppedValue);

    int value4 = 40;
    stack_push(&stack, &value4);

    int *topValue = (int *)stack_peek(&stack);
    printf("Top element: %d\n", *topValue);

    stack_free(&stack);

    return 0;
}
