#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "rbt.h"
#include "cxds_internal.h"

/* ── colour enum ─────────────────────────────────────────────────── */

typedef enum { RBT_RED = 0, RBT_BLACK = 1 } RBTColor;

/* ── struct definitions (private) ────────────────────────────────── */

struct RBTNode {
    void     *data;
    RBTNode  *left, *right, *parent;
    RBTColor  color;
};

struct RBTree {
    RBTNode  *root;
    RBTNode   nil;          /* per-tree sentinel, always BLACK */
    size_t    count;
    int     (*cmp)(const void *, const void *);
    void    (*free_data)(void *);
};

/* ── sentinel helpers ────────────────────────────────────────────── */

/* Return pointer to the embedded sentinel. */
static inline RBTNode* NIL(const RBTree *tree) {
    return (RBTNode *)&tree->nil;
}

/* Is this node the sentinel? */
static inline int is_nil(const RBTree *tree, const RBTNode *n) {
    return n == &tree->nil;
}

/* ── construction ────────────────────────────────────────────────── */

RBTree* rbt_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *))
{
    RBTree *tree = malloc(sizeof(RBTree));
    if (!tree) return NULL;

    /* Initialise sentinel: black, NULL data, pointers to itself. */
    tree->nil.data   = NULL;
    tree->nil.left   = &tree->nil;
    tree->nil.right  = &tree->nil;
    tree->nil.parent = &tree->nil;
    tree->nil.color  = RBT_BLACK;

    tree->root      = &tree->nil;
    tree->count     = 0;
    tree->cmp       = cmp;
    tree->free_data = free_data;
    return tree;
}

static RBTNode* create_node(RBTree *tree, void *data) {
    RBTNode *n = malloc(sizeof(RBTNode));
    if (!n) return NULL;
    n->data   = data;
    n->left   = NIL(tree);
    n->right  = NIL(tree);
    n->parent = NIL(tree);
    n->color  = RBT_RED;
    return n;
}

/* ── rotations ───────────────────────────────────────────────────── */

static void left_rotate(RBTree *tree, RBTNode *x) {
    RBTNode *y = x->right;
    x->right = y->left;
    if (!is_nil(tree, y->left))
        y->left->parent = x;
    y->parent = x->parent;
    if (is_nil(tree, x->parent))
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left  = x;
    x->parent = y;
}

static void right_rotate(RBTree *tree, RBTNode *x) {
    RBTNode *y = x->left;
    x->left = y->right;
    if (!is_nil(tree, y->right))
        y->right->parent = x;
    y->parent = x->parent;
    if (is_nil(tree, x->parent))
        tree->root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

/* ── insert fixup ────────────────────────────────────────────────── */

static void insert_fixup(RBTree *tree, RBTNode *z) {
    while (z->parent->color == RBT_RED) {
        if (z->parent == z->parent->parent->left) {
            RBTNode *y = z->parent->parent->right;   /* uncle */
            if (y->color == RBT_RED) {
                /* Case 1: uncle is red → recolour */
                z->parent->color         = RBT_BLACK;
                y->color                 = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    /* Case 2: triangle → rotate parent */
                    z = z->parent;
                    left_rotate(tree, z);
                }
                /* Case 3: line → rotate grandparent */
                z->parent->color         = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                right_rotate(tree, z->parent->parent);
            }
        } else {
            /* Mirror: parent is right child of grandparent */
            RBTNode *y = z->parent->parent->left;    /* uncle */
            if (y->color == RBT_RED) {
                z->parent->color         = RBT_BLACK;
                y->color                 = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(tree, z);
                }
                z->parent->color         = RBT_BLACK;
                z->parent->parent->color = RBT_RED;
                left_rotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = RBT_BLACK;
}

/* ── insert (both recursive helper and iterative share fixup) ──── */

static int insert_node(RBTree *tree, void *data) {
    RBTNode *y = NIL(tree);
    RBTNode *x = tree->root;

    while (!is_nil(tree, x)) {
        y = x;
        int c = tree->cmp(data, x->data);
        if (c < 0)
            x = x->left;
        else if (c > 0)
            x = x->right;
        else {
            /* duplicate */
            free_data_if(tree->free_data, data);
            return 0;
        }
    }

    RBTNode *z = create_node(tree, data);
    if (!z) return -1;
    z->parent = y;

    if (is_nil(tree, y))
        tree->root = z;
    else if (tree->cmp(data, y->data) < 0)
        y->left = z;
    else
        y->right = z;

    tree->count++;
    insert_fixup(tree, z);
    return 0;
}

int rbt_insert(RBTree *tree, void *data) {
    if (!tree) return -1;
    return insert_node(tree, data);
}

int rbt_insert_iter(RBTree *tree, void *data) {
    if (!tree) return -1;
    return insert_node(tree, data);
}

/* ── search ──────────────────────────────────────────────────────── */

static void* search_node(const RBTree *tree, const void *probe) {
    RBTNode *x = tree->root;
    while (!is_nil(tree, x)) {
        int c = tree->cmp(probe, x->data);
        if (c == 0) return x->data;
        x = (c < 0) ? x->left : x->right;
    }
    return NULL;
}

void* rbt_search(const RBTree *tree, const void *probe) {
    if (!tree) return NULL;
    return search_node(tree, probe);
}

void* rbt_search_iter(const RBTree *tree, const void *probe) {
    if (!tree) return NULL;
    return search_node(tree, probe);
}

/* ── transplant ──────────────────────────────────────────────────── */

static void rbt_transplant(RBTree *tree, RBTNode *u, RBTNode *v) {
    if (is_nil(tree, u->parent))
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

/* ── delete fixup ────────────────────────────────────────────────── */

static void delete_fixup(RBTree *tree, RBTNode *x) {
    while (x != tree->root && x->color == RBT_BLACK) {
        if (x == x->parent->left) {
            RBTNode *w = x->parent->right;     /* sibling */
            if (w->color == RBT_RED) {
                /* Case 1: sibling red → rotate */
                w->color = RBT_BLACK;
                x->parent->color = RBT_RED;
                left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == RBT_BLACK && w->right->color == RBT_BLACK) {
                /* Case 2: both nephews black → recolour */
                w->color = RBT_RED;
                x = x->parent;
            } else {
                if (w->right->color == RBT_BLACK) {
                    /* Case 3: far nephew black → rotate sibling */
                    w->left->color = RBT_BLACK;
                    w->color = RBT_RED;
                    right_rotate(tree, w);
                    w = x->parent->right;
                }
                /* Case 4: far nephew red → rotate parent */
                w->color = x->parent->color;
                x->parent->color = RBT_BLACK;
                w->right->color = RBT_BLACK;
                left_rotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            /* Mirror */
            RBTNode *w = x->parent->left;
            if (w->color == RBT_RED) {
                w->color = RBT_BLACK;
                x->parent->color = RBT_RED;
                right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == RBT_BLACK && w->left->color == RBT_BLACK) {
                w->color = RBT_RED;
                x = x->parent;
            } else {
                if (w->left->color == RBT_BLACK) {
                    w->right->color = RBT_BLACK;
                    w->color = RBT_RED;
                    left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = RBT_BLACK;
                w->left->color = RBT_BLACK;
                right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = RBT_BLACK;
}

/* ── delete core ─────────────────────────────────────────────────── */

static RBTNode* tree_minimum(const RBTree *tree, RBTNode *n) {
    while (!is_nil(tree, n->left))
        n = n->left;
    return n;
}

static int rbt_delete_node(RBTree *tree, const void *probe) {
    /* Find the node */
    RBTNode *z = tree->root;
    while (!is_nil(tree, z)) {
        int c = tree->cmp(probe, z->data);
        if (c == 0) break;
        z = (c < 0) ? z->left : z->right;
    }
    if (is_nil(tree, z)) return -1;   /* not found */

    RBTNode *y = z;
    RBTColor y_orig_color = y->color;
    RBTNode *x;

    if (is_nil(tree, z->left)) {
        x = z->right;
        rbt_transplant(tree, z, z->right);
    } else if (is_nil(tree, z->right)) {
        x = z->left;
        rbt_transplant(tree, z, z->left);
    } else {
        y = tree_minimum(tree, z->right);
        y_orig_color = y->color;
        x = y->right;
        if (y->parent == z) {
            /* x might be sentinel; set its parent for fixup */
            x->parent = y;
        } else {
            rbt_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbt_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free_data_if(tree->free_data, z->data);
    free(z);
    tree->count--;

    if (y_orig_color == RBT_BLACK)
        delete_fixup(tree, x);

    return 0;
}

int rbt_delete(RBTree *tree, const void *probe) {
    if (!tree) return -1;
    return rbt_delete_node(tree, probe);
}

int rbt_delete_iter(RBTree *tree, const void *probe) {
    if (!tree) return -1;
    return rbt_delete_node(tree, probe);
}

/* ── recursive traversals ────────────────────────────────────────── */

static void inorder_rec(const RBTree *tree, const RBTNode *n,
                        void (*visit)(const void *))
{
    if (!is_nil(tree, n)) {
        inorder_rec(tree, n->left, visit);
        visit(n->data);
        inorder_rec(tree, n->right, visit);
    }
}

static void preorder_rec(const RBTree *tree, const RBTNode *n,
                         void (*visit)(const void *))
{
    if (!is_nil(tree, n)) {
        visit(n->data);
        preorder_rec(tree, n->left, visit);
        preorder_rec(tree, n->right, visit);
    }
}

static void postorder_rec(const RBTree *tree, const RBTNode *n,
                          void (*visit)(const void *))
{
    if (!is_nil(tree, n)) {
        postorder_rec(tree, n->left, visit);
        postorder_rec(tree, n->right, visit);
        visit(n->data);
    }
}

void rbt_inorder(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree) return;
    inorder_rec(tree, tree->root, visit);
}

void rbt_preorder(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree) return;
    preorder_rec(tree, tree->root, visit);
}

void rbt_postorder(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree) return;
    postorder_rec(tree, tree->root, visit);
}

/* ── iterative traversals ────────────────────────────────────────── */

void rbt_inorder_iter(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree || is_nil(tree, tree->root)) return;
    Stack stack;
    if (stack_init(&stack, sizeof(const RBTNode *)) != 0) return;
    const RBTNode *cur = tree->root;
    while (!is_nil(tree, cur) || !stack_is_empty(&stack)) {
        while (!is_nil(tree, cur)) {
            if (stack_push(&stack, &cur) != 0) { stack_free(&stack); return; }
            cur = cur->left;
        }
        const RBTNode *node;
        if (stack_pop(&stack, &node) != 0) break;
        visit(node->data);
        cur = node->right;
    }
    stack_free(&stack);
}

void rbt_preorder_iter(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree || is_nil(tree, tree->root)) return;
    Stack stack;
    if (stack_init(&stack, sizeof(const RBTNode *)) != 0) return;
    const RBTNode *r = tree->root;
    if (stack_push(&stack, &r) != 0) { stack_free(&stack); return; }
    while (!stack_is_empty(&stack)) {
        const RBTNode *cur;
        if (stack_pop(&stack, &cur) != 0) break;
        visit(cur->data);
        if (!is_nil(tree, cur->right)) {
            if (stack_push(&stack, &cur->right) != 0) { stack_free(&stack); return; }
        }
        if (!is_nil(tree, cur->left)) {
            if (stack_push(&stack, &cur->left) != 0) { stack_free(&stack); return; }
        }
    }
    stack_free(&stack);
}

void rbt_postorder_iter(const RBTree *tree, void (*visit)(const void *)) {
    if (!tree || is_nil(tree, tree->root)) return;
    Stack s1, s2;
    if (stack_init(&s1, sizeof(const RBTNode *)) != 0) return;
    if (stack_init(&s2, sizeof(const RBTNode *)) != 0) { stack_free(&s1); return; }
    const RBTNode *r = tree->root;
    if (stack_push(&s1, &r) != 0) { stack_free(&s1); stack_free(&s2); return; }
    while (!stack_is_empty(&s1)) {
        const RBTNode *cur;
        if (stack_pop(&s1, &cur) != 0) break;
        if (stack_push(&s2, &cur) != 0) { stack_free(&s1); stack_free(&s2); return; }
        if (!is_nil(tree, cur->left)) {
            if (stack_push(&s1, &cur->left) != 0) { stack_free(&s1); stack_free(&s2); return; }
        }
        if (!is_nil(tree, cur->right)) {
            if (stack_push(&s1, &cur->right) != 0) { stack_free(&s1); stack_free(&s2); return; }
        }
    }
    while (!stack_is_empty(&s2)) {
        const RBTNode *node;
        if (stack_pop(&s2, &node) != 0) break;
        visit(node->data);
    }
    stack_free(&s1);
    stack_free(&s2);
}

/* ── pretty-print ────────────────────────────────────────────────── */

static void print_rec(const RBTree *tree, const RBTNode *n,
                      int level, const char *prefix,
                      void (*print_fn)(const void *))
{
    if (!is_nil(tree, n)) {
        printf("%*s%s", level * 4, "", prefix);
        print_fn(n->data);
        printf(" %s\n", n->color == RBT_RED ? "[R]" : "[B]");
        print_rec(tree, n->left,  level + 1, "L-- ", print_fn);
        print_rec(tree, n->right, level + 1, "R-- ", print_fn);
    }
}

void rbt_print(const RBTree *tree, void (*print_fn)(const void *)) {
    if (!tree) return;
    print_rec(tree, tree->root, 0, "Root: ", print_fn);
}

/* ── accessors ───────────────────────────────────────────────────── */

size_t rbt_size(const RBTree *tree) {
    return tree ? tree->count : 0;
}

void* rbt_min(const RBTree *tree) {
    if (!tree || is_nil(tree, tree->root)) return NULL;
    const RBTNode *n = tree->root;
    while (!is_nil(tree, n->left)) n = n->left;
    return n->data;
}

void* rbt_max(const RBTree *tree) {
    if (!tree || is_nil(tree, tree->root)) return NULL;
    const RBTNode *n = tree->root;
    while (!is_nil(tree, n->right)) n = n->right;
    return n->data;
}

static void collect_inorder(const RBTree *tree, const RBTNode *n,
                            void **arr, size_t *idx)
{
    if (!is_nil(tree, n)) {
        collect_inorder(tree, n->left, arr, idx);
        arr[(*idx)++] = n->data;
        collect_inorder(tree, n->right, arr, idx);
    }
}

void** rbt_to_array(const RBTree *tree, size_t *out_count) {
    if (!tree || !out_count) return NULL;
    *out_count = tree->count;
    if (tree->count == 0)
        return calloc(1, sizeof(void *));
    void **arr = malloc(tree->count * sizeof(void *));
    if (!arr) {
        *out_count = 0;
        return NULL;
    }
    size_t idx = 0;
    collect_inorder(tree, tree->root, arr, &idx);
    *out_count = idx;
    return arr;
}

/* ── node accessors ──────────────────────────────────────────────── */

RBTNode* rbt_root(const RBTree *tree) {
    if (!tree || is_nil(tree, tree->root)) return NULL;
    return tree->root;
}

RBTNode* rbt_node_left(const RBTNode *node) {
    if (!node) return NULL;
    /* Translate sentinel back to NULL: sentinel has data == NULL. */
    if (node->left && node->left->data == NULL) return NULL;
    return node->left;
}

RBTNode* rbt_node_right(const RBTNode *node) {
    if (!node) return NULL;
    if (node->right && node->right->data == NULL) return NULL;
    return node->right;
}

void* rbt_node_data(const RBTNode *node) {
    return node ? node->data : NULL;
}

/* ── validate ────────────────────────────────────────────────────── */

/*
 * Red-Black Tree properties:
 *  1. Every node is either red or black.
 *  2. The root is black.
 *  3. Every leaf (NIL sentinel) is black.
 *  4. If a node is red, both its children are black.
 *  5. For each node, all simple paths from the node to descendant
 *     leaves contain the same number of black nodes.
 */

static int validate_rec(const RBTree *tree, const RBTNode *n,
                        int black_count, int *path_black)
{
    if (is_nil(tree, n)) {
        /* Property 3: sentinel is black (guaranteed by init). */
        if (*path_black == -1)
            *path_black = black_count;
        /* Property 5: all paths have the same black-node count. */
        return black_count == *path_black;
    }

    /* Property 1: colour must be valid. */
    if (n->color != RBT_RED && n->color != RBT_BLACK)
        return 0;

    /* Property 4: red node must have two black children. */
    if (n->color == RBT_RED) {
        if (n->left->color == RBT_RED || n->right->color == RBT_RED)
            return 0;
    }

    if (n->color == RBT_BLACK)
        black_count++;

    return validate_rec(tree, n->left,  black_count, path_black)
        && validate_rec(tree, n->right, black_count, path_black);
}

int rbt_validate(const RBTree *tree) {
    if (!tree) return 0;
    /* Property 2: root is black. */
    if (!is_nil(tree, tree->root) && tree->root->color != RBT_BLACK)
        return 0;
    int path_black = -1;
    return validate_rec(tree, tree->root, 0, &path_black);
}

/* ── free ────────────────────────────────────────────────────────── */

static void free_rec(RBTree *tree, RBTNode *n) {
    if (!is_nil(tree, n)) {
        free_rec(tree, n->left);
        free_rec(tree, n->right);
        free_data_if(tree->free_data, n->data);
        free(n);
    }
}

void rbt_free(RBTree *tree) {
    if (!tree) return;
    free_rec(tree, tree->root);
    free(tree);
}
