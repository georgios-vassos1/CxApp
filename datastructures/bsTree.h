#ifndef BST_H
#define BST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct BSTNode {
    void *data;
    struct BSTNode *left, *right;
} BSTNode;

typedef struct BSTree {
    BSTNode *root;
    size_t count;
    int (*cmp)(const void *, const void *);
    void (*free_data)(void *);
} BSTree;

BSTree* bst_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *));

int    bst_insert(BSTree *tree, void *data);
int    bst_insert_iter(BSTree *tree, void *data);

void*  bst_search(const BSTree *tree, const void *probe);
void*  bst_search_iter(const BSTree *tree, const void *probe);

int    bst_delete(BSTree *tree, const void *probe);
int    bst_delete_iter(BSTree *tree, const void *probe);

void   bst_inorder(const BSTree *tree, void (*visit)(const void *));
void   bst_preorder(const BSTree *tree, void (*visit)(const void *));
void   bst_postorder(const BSTree *tree, void (*visit)(const void *));

void   bst_inorder_iter(const BSTree *tree, void (*visit)(const void *));
void   bst_preorder_iter(const BSTree *tree, void (*visit)(const void *));
void   bst_postorder_iter(const BSTree *tree, void (*visit)(const void *));

void   bst_print(const BSTree *tree,
                 void (*print_fn)(const void *));

size_t bst_size(const BSTree *tree);
void*  bst_min(const BSTree *tree);
void*  bst_max(const BSTree *tree);
void** bst_to_array(const BSTree *tree, size_t *out_count);

void   bst_free(BSTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* BST_H */
