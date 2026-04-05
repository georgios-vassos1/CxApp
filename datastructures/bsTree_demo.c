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

    // Inorder traversal of the BST
    printf("Inorder traversal of the BST:\n");
    inorderIterative(root);
    printf("\n\n");

    // Print the BST
    printf("Binary Search Tree:\n");
    printTree(root, 0, "Root: ");

    // Delete a node from the BST
    root = deleteNode(root, 2);
    printf("\nBinary Search Tree after deleting 2:\n");
    printTree(root, 0, "Root: ");

    // Free the memory used by the BST
    freeTree(root);

    return 0;
}
