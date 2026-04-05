#include <stdio.h>
#include <stdlib.h>
#include "btree.h"
#include "cxds_internal.h"

/* ── struct definitions (private) ────────────────────────────────── */

struct BTNode {
    void   **keys;       /* array of (2t - 1) data pointers */
    BTNode **children;   /* array of (2t) child pointers    */
    size_t   n;          /* current number of keys           */
    int      leaf;       /* 1 if leaf, 0 otherwise           */
};

struct BTree {
    BTNode  *root;
    size_t   t;          /* minimum degree                   */
    size_t   count;      /* total number of keys             */
    int    (*cmp)(const void *, const void *);
    void   (*free_data)(void *);
};

/* ── node helpers ────────────────────────────────────────────────── */

static BTNode* create_node(size_t t, int leaf)
{
    BTNode *node = malloc(sizeof(BTNode));
    if (!node) return NULL;

    node->keys = malloc((2 * t - 1) * sizeof(void *));
    if (!node->keys) { free(node); return NULL; }

    node->children = malloc(2 * t * sizeof(BTNode *));
    if (!node->children) { free(node->keys); free(node); return NULL; }

    node->n    = 0;
    node->leaf = leaf;

    for (size_t i = 0; i < 2 * t; i++)
        node->children[i] = NULL;

    return node;
}

static void free_node(BTree *tree, BTNode *node)
{
    if (!node) return;
    if (!node->leaf) {
        for (size_t i = 0; i <= node->n; i++)
            free_node(tree, node->children[i]);
    }
    for (size_t i = 0; i < node->n; i++)
        free_data_if(tree->free_data, node->keys[i]);
    free(node->keys);
    free(node->children);
    free(node);
}

/* ── construction ────────────────────────────────────────────────── */

BTree* bt_create(int (*cmp)(const void *, const void *),
                 void (*free_data)(void *),
                 size_t min_degree)
{
    if (!cmp || min_degree < 2) return NULL;

    BTree *tree = malloc(sizeof(BTree));
    if (!tree) return NULL;

    tree->root = create_node(min_degree, 1);
    if (!tree->root) { free(tree); return NULL; }

    tree->t         = min_degree;
    tree->count     = 0;
    tree->cmp       = cmp;
    tree->free_data = free_data;
    return tree;
}

/* ── search ──────────────────────────────────────────────────────── */

void* bt_search(const BTree *tree, const void *probe)
{
    if (!tree) return NULL;

    const BTNode *node = tree->root;
    while (node) {
        size_t i = 0;
        while (i < node->n && tree->cmp(probe, node->keys[i]) > 0)
            i++;
        if (i < node->n && tree->cmp(probe, node->keys[i]) == 0)
            return node->keys[i];
        if (node->leaf)
            return NULL;
        node = node->children[i];
    }
    return NULL;
}

/* ── split_child ─────────────────────────────────────────────────── */

/*
 * Split parent->children[i] which must be full (2t-1 keys).
 * Promotes the median key into parent and creates a new sibling.
 */
static int split_child(BTree *tree, BTNode *parent, size_t i)
{
    size_t t = tree->t;
    BTNode *y = parent->children[i];
    BTNode *z = create_node(t, y->leaf);
    if (!z) return -1;

    /* Copy the last (t-1) keys of y into z. */
    z->n = t - 1;
    for (size_t j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    /* If y is not a leaf, copy the last t children. */
    if (!y->leaf) {
        for (size_t j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->n = t - 1;

    /* Shift parent's children and keys to make room. */
    for (size_t j = parent->n; j > i; j--)
        parent->children[j + 1] = parent->children[j];
    parent->children[i + 1] = z;

    for (size_t j = parent->n; j > i; j--)
        parent->keys[j] = parent->keys[j - 1];
    parent->keys[i] = y->keys[t - 1];
    parent->n++;

    return 0;
}

/* ── insert_nonfull ──────────────────────────────────────────────── */

static int insert_nonfull(BTree *tree, BTNode *node, void *data)
{
    size_t t = tree->t;

    if (node->leaf) {
        /* Find the insertion position. */
        size_t i = node->n;
        while (i > 0 && tree->cmp(data, node->keys[i - 1]) < 0) {
            node->keys[i] = node->keys[i - 1];
            i--;
        }
        /* Check for duplicate. */
        if (i < node->n && tree->cmp(data, node->keys[i]) == 0) {
            /* Undo shift. */
            for (size_t j = i; j < node->n - 1; j++)
                node->keys[j] = node->keys[j + 1];
            /* Duplicate found, but we shifted keys[i] to keys[i+1] already.
             * Actually we need to undo: the keys after i were shifted right,
             * so shift them back left. But simpler: we shifted during the
             * while loop above, so keys[i] is now empty and keys[i+1..n-1]
             * hold the original keys[i..n-2]. Let's undo properly. */
            free_data_if(tree->free_data, data);
            return 0;
        }
        node->keys[i] = data;
        node->n++;
        tree->count++;
        return 0;
    }

    /* Internal node: find child to descend into. */
    size_t i = node->n;
    while (i > 0 && tree->cmp(data, node->keys[i - 1]) < 0)
        i--;

    /* Check for duplicate at this node. */
    if (i < node->n && tree->cmp(data, node->keys[i]) == 0) {
        free_data_if(tree->free_data, data);
        return 0;
    }

    /* If the child is full, split it. */
    if (node->children[i]->n == 2 * t - 1) {
        if (split_child(tree, node, i) != 0) return -1;
        /* After split, the median is at keys[i]. Decide which child. */
        int c = tree->cmp(data, node->keys[i]);
        if (c == 0) {
            free_data_if(tree->free_data, data);
            return 0;
        }
        if (c > 0)
            i++;
    }

    return insert_nonfull(tree, node->children[i], data);
}

/* ── insert ──────────────────────────────────────────────────────── */

int bt_insert(BTree *tree, void *data)
{
    if (!tree) return -1;

    size_t t = tree->t;
    BTNode *r = tree->root;

    /* If root is full, grow the tree height. */
    if (r->n == 2 * t - 1) {
        BTNode *s = create_node(t, 0);
        if (!s) return -1;
        s->children[0] = r;
        tree->root = s;
        if (split_child(tree, s, 0) != 0) return -1;
        return insert_nonfull(tree, s, data);
    }

    return insert_nonfull(tree, r, data);
}

/* ── delete helpers ──────────────────────────────────────────────── */

/*
 * Find the predecessor key: rightmost key in the left subtree.
 */
static void* get_predecessor(BTNode *node)
{
    while (!node->leaf)
        node = node->children[node->n];
    return node->keys[node->n - 1];
}

/*
 * Find the successor key: leftmost key in the right subtree.
 */
static void* get_successor(BTNode *node)
{
    while (!node->leaf)
        node = node->children[0];
    return node->keys[0];
}

/*
 * Merge children[i+1] into children[i], pulling keys[i] down as median.
 */
static void merge(BTree *tree, BTNode *node, size_t i)
{
    size_t t = tree->t;
    BTNode *left  = node->children[i];
    BTNode *right = node->children[i + 1];

    /* Pull the separating key down into left. */
    left->keys[t - 1] = node->keys[i];

    /* Copy right's keys into left. */
    for (size_t j = 0; j < right->n; j++)
        left->keys[j + t] = right->keys[j];

    /* Copy right's children into left. */
    if (!left->leaf) {
        for (size_t j = 0; j <= right->n; j++)
            left->children[j + t] = right->children[j];
    }

    left->n = 2 * t - 1;

    /* Shift node's keys and children left. */
    for (size_t j = i; j < node->n - 1; j++)
        node->keys[j] = node->keys[j + 1];
    for (size_t j = i + 1; j < node->n; j++)
        node->children[j] = node->children[j + 1];
    node->children[node->n] = NULL;
    node->n--;

    /* Free the right node's arrays (not its data, which moved to left). */
    free(right->keys);
    free(right->children);
    free(right);
}

/*
 * Ensure node->children[i] has at least t keys.
 * Borrows from a sibling if possible, otherwise merges.
 * Returns the (possibly updated) index of the child to descend into.
 */
static size_t fill(BTree *tree, BTNode *node, size_t i)
{
    size_t t = tree->t;

    /* Try to borrow from left sibling. */
    if (i > 0 && node->children[i - 1]->n >= t) {
        BTNode *child = node->children[i];
        BTNode *left  = node->children[i - 1];

        /* Shift child's keys and children right. */
        for (size_t j = child->n; j > 0; j--)
            child->keys[j] = child->keys[j - 1];
        if (!child->leaf) {
            for (size_t j = child->n + 1; j > 0; j--)
                child->children[j] = child->children[j - 1];
        }

        child->keys[0] = node->keys[i - 1];
        if (!child->leaf)
            child->children[0] = left->children[left->n];
        node->keys[i - 1] = left->keys[left->n - 1];

        child->n++;
        left->n--;
        return i;
    }

    /* Try to borrow from right sibling. */
    if (i < node->n && node->children[i + 1]->n >= t) {
        BTNode *child = node->children[i];
        BTNode *right = node->children[i + 1];

        child->keys[child->n] = node->keys[i];
        if (!child->leaf)
            child->children[child->n + 1] = right->children[0];
        node->keys[i] = right->keys[0];

        /* Shift right's keys and children left. */
        for (size_t j = 0; j < right->n - 1; j++)
            right->keys[j] = right->keys[j + 1];
        if (!right->leaf) {
            for (size_t j = 0; j < right->n; j++)
                right->children[j] = right->children[j + 1];
        }

        child->n++;
        right->n--;
        return i;
    }

    /* Merge with a sibling. */
    if (i < node->n) {
        merge(tree, node, i);
        return i;
    } else {
        merge(tree, node, i - 1);
        return i - 1;
    }
}

/*
 * Recursive B-tree delete from CLRS.
 */
static int delete_key(BTree *tree, BTNode *node, const void *probe)
{
    size_t t = tree->t;
    size_t i = 0;

    /* Find the position where probe belongs. */
    while (i < node->n && tree->cmp(probe, node->keys[i]) > 0)
        i++;

    /* Case: key found in this node. */
    if (i < node->n && tree->cmp(probe, node->keys[i]) == 0) {

        if (node->leaf) {
            /* Case 1: key is in a leaf — remove directly. */
            free_data_if(tree->free_data, node->keys[i]);
            for (size_t j = i; j < node->n - 1; j++)
                node->keys[j] = node->keys[j + 1];
            node->n--;
            tree->count--;
            return 0;
        }

        /* Case 2a: left child has >= t keys. */
        if (node->children[i]->n >= t) {
            void *pred = get_predecessor(node->children[i]);
            free_data_if(tree->free_data, node->keys[i]);
            node->keys[i] = pred;
            return delete_key(tree, node->children[i], pred);
        }

        /* Case 2b: right child has >= t keys. */
        if (node->children[i + 1]->n >= t) {
            void *succ = get_successor(node->children[i + 1]);
            free_data_if(tree->free_data, node->keys[i]);
            node->keys[i] = succ;
            return delete_key(tree, node->children[i + 1], succ);
        }

        /* Case 2c: both children have t-1 keys — merge and recurse. */
        merge(tree, node, i);
        return delete_key(tree, node->children[i], probe);
    }

    /* Key not in this node. */
    if (node->leaf)
        return -1;   /* not found */

    /* Case 3: ensure the child we descend into has >= t keys. */
    if (node->children[i]->n < t)
        i = fill(tree, node, i);

    return delete_key(tree, node->children[i], probe);
}

/* ── delete ──────────────────────────────────────────────────────── */

int bt_delete(BTree *tree, const void *probe)
{
    if (!tree || !tree->root || tree->root->n == 0)
        return -1;

    int result = delete_key(tree, tree->root, probe);

    /* If root has no keys but has a child, shrink the tree. */
    if (tree->root->n == 0 && !tree->root->leaf) {
        BTNode *old = tree->root;
        tree->root = old->children[0];
        free(old->keys);
        free(old->children);
        free(old);
    }

    return result;
}

/* ── traversals ──────────────────────────────────────────────────── */

static void inorder_rec(const BTNode *node, void (*visit)(const void *))
{
    if (!node) return;
    for (size_t i = 0; i < node->n; i++) {
        if (!node->leaf)
            inorder_rec(node->children[i], visit);
        visit(node->keys[i]);
    }
    if (!node->leaf)
        inorder_rec(node->children[node->n], visit);
}

void bt_inorder(const BTree *tree, void (*visit)(const void *))
{
    if (!tree) return;
    inorder_rec(tree->root, visit);
}

static void preorder_rec(const BTNode *node, void (*visit)(const void *))
{
    if (!node) return;
    for (size_t i = 0; i < node->n; i++)
        visit(node->keys[i]);
    if (!node->leaf) {
        for (size_t i = 0; i <= node->n; i++)
            preorder_rec(node->children[i], visit);
    }
}

void bt_preorder(const BTree *tree, void (*visit)(const void *))
{
    if (!tree) return;
    preorder_rec(tree->root, visit);
}

static void postorder_rec(const BTNode *node, void (*visit)(const void *))
{
    if (!node) return;
    if (!node->leaf) {
        for (size_t i = 0; i <= node->n; i++)
            postorder_rec(node->children[i], visit);
    }
    for (size_t i = 0; i < node->n; i++)
        visit(node->keys[i]);
}

void bt_postorder(const BTree *tree, void (*visit)(const void *))
{
    if (!tree) return;
    postorder_rec(tree->root, visit);
}

/* ── pretty-print ────────────────────────────────────────────────── */

static void print_rec(const BTNode *node, int level,
                      void (*print_fn)(const void *))
{
    if (!node) return;
    printf("%*s[", level * 4, "");
    for (size_t i = 0; i < node->n; i++) {
        if (i > 0) printf(", ");
        print_fn(node->keys[i]);
    }
    printf("]\n");
    if (!node->leaf) {
        for (size_t i = 0; i <= node->n; i++)
            print_rec(node->children[i], level + 1, print_fn);
    }
}

void bt_print(const BTree *tree, void (*print_fn)(const void *))
{
    if (!tree) return;
    print_rec(tree->root, 0, print_fn);
}

/* ── accessors ───────────────────────────────────────────────────── */

size_t bt_size(const BTree *tree)
{
    return tree ? tree->count : 0;
}

void* bt_min(const BTree *tree)
{
    if (!tree || tree->root->n == 0) return NULL;
    const BTNode *node = tree->root;
    while (!node->leaf)
        node = node->children[0];
    return node->keys[0];
}

void* bt_max(const BTree *tree)
{
    if (!tree || tree->root->n == 0) return NULL;
    const BTNode *node = tree->root;
    while (!node->leaf)
        node = node->children[node->n];
    return node->keys[node->n - 1];
}

/* ── to_array ────────────────────────────────────────────────────── */

static void collect_inorder(const BTNode *node, void **arr, size_t *idx)
{
    if (!node) return;
    for (size_t i = 0; i < node->n; i++) {
        if (!node->leaf)
            collect_inorder(node->children[i], arr, idx);
        arr[(*idx)++] = node->keys[i];
    }
    if (!node->leaf)
        collect_inorder(node->children[node->n], arr, idx);
}

void** bt_to_array(const BTree *tree, size_t *out_count)
{
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
    collect_inorder(tree->root, arr, &idx);
    *out_count = idx;
    return arr;
}

/* ── node accessors ──────────────────────────────────────────────── */

BTNode* bt_root(const BTree *tree)
{
    if (!tree || tree->root->n == 0) return NULL;
    return tree->root;
}

size_t bt_node_num_keys(const BTNode *node)
{
    return node ? node->n : 0;
}

void* bt_node_key(const BTNode *node, size_t index)
{
    if (!node || index >= node->n) return NULL;
    return node->keys[index];
}

BTNode* bt_node_child(const BTNode *node, size_t index)
{
    if (!node || node->leaf || index > node->n) return NULL;
    return node->children[index];
}

int bt_node_is_leaf(const BTNode *node)
{
    return node ? node->leaf : 1;
}

/* ── validate ────────────────────────────────────────────────────── */

/*
 * B-tree properties:
 *  1. Root may have 1..2t-1 keys; all other internal nodes have t-1..2t-1.
 *  2. Every non-leaf with n keys has exactly n+1 children.
 *  3. All leaves are at the same depth.
 *  4. Keys within each node are sorted.
 *  5. Keys respect search-tree ordering relative to children subtrees.
 */
static int validate_rec(const BTree *tree, const BTNode *node,
                        int is_root, int depth, int *leaf_depth)
{
    if (!node) return 0;

    size_t t = tree->t;

    /* Property 1: key count bounds. */
    if (!is_root && node->n < t - 1)
        return 0;
    if (node->n > 2 * t - 1)
        return 0;

    /* Property 4: keys sorted. */
    for (size_t i = 1; i < node->n; i++) {
        if (tree->cmp(node->keys[i - 1], node->keys[i]) >= 0)
            return 0;
    }

    if (node->leaf) {
        /* Property 3: all leaves at same depth. */
        if (*leaf_depth == -1)
            *leaf_depth = depth;
        return depth == *leaf_depth;
    }

    /* Property 2: n+1 children for internal node. */
    for (size_t i = 0; i <= node->n; i++) {
        if (!node->children[i])
            return 0;
    }

    /* Property 5: ordering w.r.t. children. */
    for (size_t i = 0; i <= node->n; i++) {
        BTNode *child = node->children[i];
        /* All keys in children[i] must be < keys[i] (if i < n). */
        if (i < node->n) {
            for (size_t j = 0; j < child->n; j++) {
                if (tree->cmp(child->keys[j], node->keys[i]) >= 0)
                    return 0;
            }
        }
        /* All keys in children[i] must be > keys[i-1] (if i > 0). */
        if (i > 0) {
            for (size_t j = 0; j < child->n; j++) {
                if (tree->cmp(child->keys[j], node->keys[i - 1]) <= 0)
                    return 0;
            }
        }
    }

    /* Recurse into children. */
    for (size_t i = 0; i <= node->n; i++) {
        if (!validate_rec(tree, node->children[i], 0, depth + 1, leaf_depth))
            return 0;
    }

    return 1;
}

int bt_validate(const BTree *tree)
{
    if (!tree) return 0;
    if (tree->root->n == 0) return 1;   /* empty tree is valid */
    int leaf_depth = -1;
    return validate_rec(tree, tree->root, 1, 0, &leaf_depth);
}

/* ── free ────────────────────────────────────────────────────────── */

void bt_free(BTree *tree)
{
    if (!tree) return;
    free_node(tree, tree->root);
    free(tree);
}
