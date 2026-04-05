#ifndef BTREE_H
#define BTREE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Opaque types — callers use pointers only. */
typedef struct BTNode BTNode;
typedef struct BTree  BTree;

BTree* bt_create(int (*cmp)(const void *, const void *),
                 void (*free_data)(void *),
                 size_t min_degree);

int    bt_insert(BTree *tree, void *data);

void*  bt_search(const BTree *tree, const void *probe);

int    bt_delete(BTree *tree, const void *probe);

void   bt_inorder(const BTree *tree, void (*visit)(const void *));
void   bt_preorder(const BTree *tree, void (*visit)(const void *));
void   bt_postorder(const BTree *tree, void (*visit)(const void *));

void   bt_print(const BTree *tree,
                void (*print_fn)(const void *));

size_t bt_size(const BTree *tree);
void*  bt_min(const BTree *tree);
void*  bt_max(const BTree *tree);
void** bt_to_array(const BTree *tree, size_t *out_count);

/* Node accessors — for callers that need to walk the tree structure. */
BTNode* bt_root(const BTree *tree);
size_t  bt_node_num_keys(const BTNode *node);
void*   bt_node_key(const BTNode *node, size_t index);
BTNode* bt_node_child(const BTNode *node, size_t index);
int     bt_node_is_leaf(const BTNode *node);

/* Validate all B-tree properties. Returns 1 if valid, 0 if not. */
int    bt_validate(const BTree *tree);

void   bt_free(BTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* BTREE_H */
