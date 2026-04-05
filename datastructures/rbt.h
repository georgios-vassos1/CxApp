#ifndef RBT_H
#define RBT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Opaque types — callers use pointers only. */
typedef struct RBTNode RBTNode;
typedef struct RBTree  RBTree;

RBTree* rbt_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *));

int    rbt_insert(RBTree *tree, void *data);
int    rbt_insert_iter(RBTree *tree, void *data);

void*  rbt_search(const RBTree *tree, const void *probe);
void*  rbt_search_iter(const RBTree *tree, const void *probe);

int    rbt_delete(RBTree *tree, const void *probe);
int    rbt_delete_iter(RBTree *tree, const void *probe);

void   rbt_inorder(const RBTree *tree, void (*visit)(const void *));
void   rbt_preorder(const RBTree *tree, void (*visit)(const void *));
void   rbt_postorder(const RBTree *tree, void (*visit)(const void *));

void   rbt_inorder_iter(const RBTree *tree, void (*visit)(const void *));
void   rbt_preorder_iter(const RBTree *tree, void (*visit)(const void *));
void   rbt_postorder_iter(const RBTree *tree, void (*visit)(const void *));

void   rbt_print(const RBTree *tree,
                 void (*print_fn)(const void *));

size_t rbt_size(const RBTree *tree);
void*  rbt_min(const RBTree *tree);
void*  rbt_max(const RBTree *tree);
void** rbt_to_array(const RBTree *tree, size_t *out_count);

/* Node accessors — for callers that need to walk the tree structure. */
RBTNode* rbt_root(const RBTree *tree);
RBTNode* rbt_node_left(const RBTNode *node);
RBTNode* rbt_node_right(const RBTNode *node);
void*    rbt_node_data(const RBTNode *node);

/* Validate all 5 red-black tree properties. Returns 1 if valid, 0 if not. */
int    rbt_validate(const RBTree *tree);

void   rbt_free(RBTree *tree);

#ifdef __cplusplus
}
#endif

#endif /* RBT_H */
