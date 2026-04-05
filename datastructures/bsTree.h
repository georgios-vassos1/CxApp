#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    void *data;
    struct BSTNode *left, *right;
} BSTNode;

typedef struct BSTree {
    BSTNode *root;
    int (*cmp)(const void *, const void *);
    void (*free_data)(void *);
} BSTree;

BSTree* bst_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *));

void   bst_insert(BSTree *tree, void *data);
void   bst_insert_iter(BSTree *tree, void *data);

void*  bst_search(const BSTree *tree, const void *probe);
void*  bst_search_iter(const BSTree *tree, const void *probe);

void   bst_delete(BSTree *tree, const void *probe);
void   bst_delete_iter(BSTree *tree, const void *probe);

void   bst_inorder(const BSTree *tree, void (*visit)(const void *));
void   bst_preorder(const BSTree *tree, void (*visit)(const void *));
void   bst_postorder(const BSTree *tree, void (*visit)(const void *));

void   bst_inorder_iter(const BSTree *tree, void (*visit)(const void *));
void   bst_preorder_iter(const BSTree *tree, void (*visit)(const void *));
void   bst_postorder_iter(const BSTree *tree, void (*visit)(const void *));

void   bst_print(const BSTree *tree,
                 void (*print_fn)(const void *));

void   bst_free(BSTree *tree);

#endif /* BST_H */
