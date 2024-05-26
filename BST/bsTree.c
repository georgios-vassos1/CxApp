#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"
#include "bsTree.h"


// Create a new node
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a new node with given key in BST
Node* insert(Node* node, int key) {
    if (node == NULL) {
        return createNode(key);
    }
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    }
    return node;
}

// Non-recursive insert function
Node* insertIterative(Node* root, int key) {
    Node* newNode = createNode(key);
    if (root == NULL) {
        return newNode;
    }

    Node* current = root;
    while (current != NULL) {
        if (key < current->key) {
            if (current->left == NULL) {
                current->left = newNode;
                break;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = newNode;
                break;
            }
            current = current->right;
        }
    }

    return root;
}

// Function to search a given key in BST
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }
    if (key < root->key) {
        return search(root->left, key);
    }
    return search(root->right, key);
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Function to delete the node with given key in BST
Node* deleteNode(Node* root, int key) {
    if (root == NULL) {
        return root;
    }
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

// Recursive inorder traversal
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

// Iterative inorder traversal
void iterativeInorder(Node* root) {
    if (root == NULL)
        return;

    Stack stack;
    initStack(&stack, sizeof(Node*));

    Node* current = root;

    while (current != NULL || !isEmpty(&stack)) {
        while (current != NULL) {
            push(&stack, &current);
            current = current->left;
        }

        current = *((Node**)pop(&stack));
        printf("%d ", current->key);

        current = current->right;
    }

    freeStack(&stack);
}


// Recursive preorder traversal
void preorder(Node* root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

// Recursive postorder traversal
void postorder(Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->key);
    }
}

// Free the memory used by the BST
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

void printTree(Node* root, int level, char* prefix) {
    if (root != NULL) {
        printf("%*s%s%d\n", level * 4, "", prefix, root->key);
        printTree(root->left, level + 1, "L-- ");
        printTree(root->right, level + 1, "R-- ");
    }
}

