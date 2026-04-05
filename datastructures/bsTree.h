#ifndef BST_H
#define BST_H

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node* createNode(int key);
Node* insert(Node* node, int key);
Node* search(Node* root, int key);
Node* deleteNode(Node* root, int key);
void inorder(Node* root);
void preorder(Node* root);
void postorder(Node* root);
void freeTree(Node* root);
void printTree(Node* root, int level, char* prefix);

Node* insertIterative(Node* root, int key);
Node* searchIterative(Node* root, int key);
Node* deleteNodeIterative(Node* root, int key);
void inorderIterative(Node* root);
void preorderIterative(Node* root);
void postorderIterative(Node* root);

#endif // BST_H
