#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    int key;
    struct BSTNode *left, *right;
} BSTNode;

BSTNode* bst_create_node(int key);
BSTNode* bst_insert(BSTNode *node, int key);
BSTNode* bst_search(BSTNode *root, int key);
BSTNode* bst_delete(BSTNode *root, int key);
void bst_inorder(const BSTNode *root);
void bst_preorder(const BSTNode *root);
void bst_postorder(const BSTNode *root);
void bst_free(BSTNode *root);
void bst_print(const BSTNode *root, int level, const char *prefix);

BSTNode* bst_insert_iter(BSTNode *root, int key);
BSTNode* bst_search_iter(BSTNode *root, int key);
BSTNode* bst_delete_iter(BSTNode *root, int key);
void bst_inorder_iter(const BSTNode *root);
void bst_preorder_iter(const BSTNode *root);
void bst_postorder_iter(const BSTNode *root);

#endif // BST_H
