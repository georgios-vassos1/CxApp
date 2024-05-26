#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"

int main() {
    Stack stack;
    initStack(&stack, sizeof(Node*));

    // Create some tree nodes
    Node *node1 = createNode(1);
    Node *node2 = createNode(2);
    Node *node3 = createNode(3);

    // Push tree nodes onto the stack
    push(&stack, &node1);
    push(&stack, &node2);
    push(&stack, &node3);

    // Pop and print tree node keys from the stack
    Node **poppedNode = (Node **)pop(&stack);
    printf("Popped: %d\n", (*poppedNode)->key);

    poppedNode = (Node **)pop(&stack);
    printf("Popped: %d\n", (*poppedNode)->key);

    // Push another tree node
    Node *node4 = createNode(4);
    push(&stack, &node4);

    // Print the key of the top tree node
    Node **topNode = (Node **)peek(&stack);
    printf("Top element: %d\n", (*topNode)->key);

    // Free the stack memory
    freeStack(&stack);

    // Free the tree nodes
    free(node1);
    free(node2);
    free(node3);
    free(node4);

    return 0;
}
