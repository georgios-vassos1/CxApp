#include <stdio.h>
#include "stack.h"
#include "bsTree.h"

int main(void) {
    // Constructing the BST:
    //        4
    //      /   \
    //     2     6
    //    / \   / \
    //   1   3 5   7

    BSTNode *root = bst_create_node(4);
    root = bst_insert_iter(root, 2);
    root = bst_insert_iter(root, 6);
    root = bst_insert_iter(root, 1);
    root = bst_insert_iter(root, 3);
    root = bst_insert_iter(root, 5);
    root = bst_insert_iter(root, 7);

    printf("Inorder traversal of the BST:\n");
    bst_inorder_iter(root);
    printf("\n\n");

    printf("Binary Search Tree:\n");
    bst_print(root, 0, "Root: ");

    root = bst_delete(root, 2);
    printf("\nBinary Search Tree after deleting 2:\n");
    bst_print(root, 0, "Root: ");

    bst_free(root);

    return 0;
}
