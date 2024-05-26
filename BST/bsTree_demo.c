#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "bsTree.h"


int main() {
    // Constructing the BST:
    //        4
    //      /   \
    //     2     6
    //    / \   / \
    //   1   3 5   7

    Node* root = createNode(4);
    root = insertIterative(root, 2);
    root = insertIterative(root, 6);
    root = insertIterative(root, 1);
    root = insertIterative(root, 3);
    root = insertIterative(root, 5);
    root = insertIterative(root, 7);

    // Print the BST
    printf("Binary Search Tree:\n");
    printTree(root, 0, "Root: ");

    // Free the memory used by the BST
    freeTree(root);

    return 0;
}
