#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"


// Main function to demonstrate the stack with integer data
int main() {
    Stack stack;
    initStack(&stack, sizeof(int));

    // Push integers onto the stack
    int value1 = 10;
    int value2 = 20;
    int value3 = 30;
    push(&stack, &value1);
    push(&stack, &value2);
    push(&stack, &value3);

    // Pop and print integer values from the stack
    int *poppedValue = (int *)pop(&stack);
    printf("Popped: %d\n", *poppedValue);
    free(poppedValue); // Free the popped value

    poppedValue = (int *)pop(&stack);
    printf("Popped: %d\n", *poppedValue);
    free(poppedValue); // Free the popped value

    // Push another integer
    int value4 = 40;
    push(&stack, &value4);

    // Print the value of the top integer
    int *topValue = (int *)peek(&stack);
    printf("Top element: %d\n", *topValue);

    // Free the stack memory
    freeStack(&stack);

    return 0;
}
