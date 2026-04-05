#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"

int main(void) {
    Stack stack;
    stack_init(&stack, sizeof(BSTNode *));

    BSTNode *node1 = bst_create_node(1);
    BSTNode *node2 = bst_create_node(2);
    BSTNode *node3 = bst_create_node(3);

    stack_push(&stack, &node1);
    stack_push(&stack, &node2);
    stack_push(&stack, &node3);

    BSTNode **poppedNode = (BSTNode **)stack_pop(&stack);
    printf("Popped: %d\n", (*poppedNode)->key);
    free(poppedNode);

    poppedNode = (BSTNode **)stack_pop(&stack);
    printf("Popped: %d\n", (*poppedNode)->key);
    free(poppedNode);

    BSTNode *node4 = bst_create_node(4);
    stack_push(&stack, &node4);

    BSTNode **topNode = (BSTNode **)stack_peek(&stack);
    printf("Top element: %d\n", (*topNode)->key);

    stack_free(&stack);

    free(node1);
    free(node2);
    free(node3);
    free(node4);

    return 0;
}
