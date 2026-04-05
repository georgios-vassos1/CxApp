#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"
#include "cxds_internal.h"

BSTree* bst_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *))
{
    BSTree *tree = malloc(sizeof(BSTree));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->count = 0;
    tree->cmp = cmp;
    tree->free_data = free_data;
    return tree;
}

static BSTNode* create_node(void *data) {
    BSTNode *n = malloc(sizeof(BSTNode));
    if (!n) return NULL;
    n->data = data;
    n->left = n->right = NULL;
    return n;
}

/* ── recursive insert ────────────────────────────────────────────── */

/* status: 1 = inserted, 0 = duplicate, -1 = alloc failure */
static BSTNode* insert_rec(BSTNode *node, void *data,
                           int (*cmp)(const void *, const void *),
                           void (*free_data)(void *),
                           int *status)
{
    if (node == NULL) {
        BSTNode *n = create_node(data);
        if (!n) { *status = -1; return NULL; }
        *status = 1;
        return n;
    }

    int c = cmp(data, node->data);
    if (c < 0)
        node->left = insert_rec(node->left, data, cmp, free_data, status);
    else if (c > 0)
        node->right = insert_rec(node->right, data, cmp, free_data, status);
    else {
        free_data_if(free_data, data);   /* duplicate – discard */
        *status = 0;
    }

    return node;
}

int bst_insert(BSTree *tree, void *data) {
    if (!tree) return -1;
    int status = 0;
    tree->root = insert_rec(tree->root, data, tree->cmp, tree->free_data, &status);
    if (status == 1) tree->count++;
    return (status == -1) ? -1 : 0;
}

/* ── iterative insert ────────────────────────────────────────────── */

int bst_insert_iter(BSTree *tree, void *data) {
    if (!tree) return -1;
    BSTNode *newNode = create_node(data);
    if (!newNode) return -1;
    if (tree->root == NULL) {
        tree->root = newNode;
        tree->count++;
        return 0;
    }
    BSTNode *cur = tree->root;
    for (;;) {
        int c = tree->cmp(data, cur->data);
        if (c < 0) {
            if (cur->left == NULL) { cur->left = newNode; tree->count++; return 0; }
            cur = cur->left;
        } else if (c > 0) {
            if (cur->right == NULL) { cur->right = newNode; tree->count++; return 0; }
            cur = cur->right;
        } else {
            free_data_if(tree->free_data, data);
            free(newNode);
            return 0;
        }
    }
}

/* ── recursive search ────────────────────────────────────────────── */

static void* search_rec(const BSTNode *node, const void *probe,
                        int (*cmp)(const void *, const void *))
{
    if (node == NULL) return NULL;
    int c = cmp(probe, node->data);
    if (c == 0) return node->data;
    if (c < 0)  return search_rec(node->left, probe, cmp);
    return search_rec(node->right, probe, cmp);
}

void* bst_search(const BSTree *tree, const void *probe) {
    if (!tree) return NULL;
    return search_rec(tree->root, probe, tree->cmp);
}

/* ── iterative search ────────────────────────────────────────────── */

void* bst_search_iter(const BSTree *tree, const void *probe) {
    if (!tree) return NULL;
    const BSTNode *cur = tree->root;
    while (cur) {
        int c = tree->cmp(probe, cur->data);
        if (c == 0) return cur->data;
        cur = (c < 0) ? cur->left : cur->right;
    }
    return NULL;
}

/* ── recursive delete ────────────────────────────────────────────── */

static BSTNode* min_node(BSTNode *n) {
    while (n && n->left) n = n->left;
    return n;
}

static BSTNode* delete_rec(BSTNode *node, const void *probe,
                           int (*cmp)(const void *, const void *),
                           void (*free_data)(void *),
                           int *found)
{
    if (node == NULL) return NULL;

    int c = cmp(probe, node->data);
    if (c < 0) {
        node->left = delete_rec(node->left, probe, cmp, free_data, found);
    } else if (c > 0) {
        node->right = delete_rec(node->right, probe, cmp, free_data, found);
    } else {
        /* found */
        *found = 1;
        if (node->left == NULL) {
            BSTNode *tmp = node->right;
            free_data_if(free_data, node->data);
            free(node);
            return tmp;
        }
        if (node->right == NULL) {
            BSTNode *tmp = node->left;
            free_data_if(free_data, node->data);
            free(node);
            return tmp;
        }
        /* two children: swap data with inorder successor, then
           delete the successor (which has at most one child). */
        BSTNode *succ = min_node(node->right);
        void *tmp_data = node->data;
        node->data = succ->data;
        succ->data = tmp_data;
        node->right = delete_rec(node->right, tmp_data, cmp, free_data, found);
    }
    return node;
}

int bst_delete(BSTree *tree, const void *probe) {
    if (!tree) return -1;
    int found = 0;
    tree->root = delete_rec(tree->root, probe, tree->cmp, tree->free_data, &found);
    if (found) { tree->count--; return 0; }
    return -1;
}

/* ── iterative delete ────────────────────────────────────────────── */

int bst_delete_iter(BSTree *tree, const void *probe) {
    if (!tree) return -1;
    BSTNode *parent = NULL;
    BSTNode *cur = tree->root;
    int c;

    while (cur && (c = tree->cmp(probe, cur->data)) != 0) {
        parent = cur;
        cur = (c < 0) ? cur->left : cur->right;
    }
    if (cur == NULL) return -1;

    /* two children → replace with inorder successor */
    if (cur->left && cur->right) {
        BSTNode *sp = cur;
        BSTNode *succ = cur->right;
        while (succ->left) { sp = succ; succ = succ->left; }
        free_data_if(tree->free_data, cur->data);
        cur->data = succ->data;
        succ->data = NULL;
        cur = succ;
        parent = sp;
    }

    BSTNode *child = cur->left ? cur->left : cur->right;

    if (parent == NULL)
        tree->root = child;
    else if (parent->left == cur)
        parent->left = child;
    else
        parent->right = child;

    if (cur->data) free_data_if(tree->free_data, cur->data);
    free(cur);
    tree->count--;
    return 0;
}

/* ── recursive traversals ────────────────────────────────────────── */

static void inorder_rec(const BSTNode *n, void (*visit)(const void *)) {
    if (n) { inorder_rec(n->left, visit); visit(n->data); inorder_rec(n->right, visit); }
}
static void preorder_rec(const BSTNode *n, void (*visit)(const void *)) {
    if (n) { visit(n->data); preorder_rec(n->left, visit); preorder_rec(n->right, visit); }
}
static void postorder_rec(const BSTNode *n, void (*visit)(const void *)) {
    if (n) { postorder_rec(n->left, visit); postorder_rec(n->right, visit); visit(n->data); }
}

void bst_inorder(const BSTree *tree, void (*visit)(const void *))  {
    if (!tree) return;
    inorder_rec(tree->root, visit);
}
void bst_preorder(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree) return;
    preorder_rec(tree->root, visit);
}
void bst_postorder(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree) return;
    postorder_rec(tree->root, visit);
}

/* ── iterative traversals ────────────────────────────────────────── */

void bst_inorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree || !tree->root) return;
    Stack stack;
    if (stack_init(&stack, sizeof(const BSTNode *)) != 0) return;
    const BSTNode *cur = tree->root;
    while (cur || !stack_is_empty(&stack)) {
        while (cur) {
            if (stack_push(&stack, &cur) != 0) { stack_free(&stack); return; }
            cur = cur->left;
        }
        const BSTNode *node;
        if (stack_pop(&stack, &node) != 0) break;
        visit(node->data);
        cur = node->right;
    }
    stack_free(&stack);
}

void bst_preorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree || !tree->root) return;
    Stack stack;
    if (stack_init(&stack, sizeof(const BSTNode *)) != 0) return;
    const BSTNode *r = tree->root;
    if (stack_push(&stack, &r) != 0) { stack_free(&stack); return; }
    while (!stack_is_empty(&stack)) {
        const BSTNode *cur;
        if (stack_pop(&stack, &cur) != 0) break;
        visit(cur->data);
        if (cur->right) {
            if (stack_push(&stack, &cur->right) != 0) { stack_free(&stack); return; }
        }
        if (cur->left) {
            if (stack_push(&stack, &cur->left) != 0) { stack_free(&stack); return; }
        }
    }
    stack_free(&stack);
}

void bst_postorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree || !tree->root) return;
    Stack s1, s2;
    if (stack_init(&s1, sizeof(const BSTNode *)) != 0) return;
    if (stack_init(&s2, sizeof(const BSTNode *)) != 0) { stack_free(&s1); return; }
    const BSTNode *r = tree->root;
    if (stack_push(&s1, &r) != 0) { stack_free(&s1); stack_free(&s2); return; }
    while (!stack_is_empty(&s1)) {
        const BSTNode *cur;
        if (stack_pop(&s1, &cur) != 0) break;
        if (stack_push(&s2, &cur) != 0) { stack_free(&s1); stack_free(&s2); return; }
        if (cur->left) {
            if (stack_push(&s1, &cur->left) != 0) { stack_free(&s1); stack_free(&s2); return; }
        }
        if (cur->right) {
            if (stack_push(&s1, &cur->right) != 0) { stack_free(&s1); stack_free(&s2); return; }
        }
    }
    while (!stack_is_empty(&s2)) {
        const BSTNode *node;
        if (stack_pop(&s2, &node) != 0) break;
        visit(node->data);
    }
    stack_free(&s1);
    stack_free(&s2);
}

/* ── pretty-print ────────────────────────────────────────────────── */

static void print_rec(const BSTNode *n, int level, const char *prefix,
                      void (*print_fn)(const void *))
{
    if (n) {
        printf("%*s%s", level * 4, "", prefix);
        print_fn(n->data);
        printf("\n");
        print_rec(n->left,  level + 1, "L-- ", print_fn);
        print_rec(n->right, level + 1, "R-- ", print_fn);
    }
}

void bst_print(const BSTree *tree, void (*print_fn)(const void *)) {
    if (!tree) return;
    print_rec(tree->root, 0, "Root: ", print_fn);
}

/* ── accessors ───────────────────────────────────────────────────── */

size_t bst_size(const BSTree *tree) {
    return tree ? tree->count : 0;
}

void* bst_min(const BSTree *tree) {
    if (!tree || !tree->root) return NULL;
    const BSTNode *n = tree->root;
    while (n->left) n = n->left;
    return n->data;
}

void* bst_max(const BSTree *tree) {
    if (!tree || !tree->root) return NULL;
    const BSTNode *n = tree->root;
    while (n->right) n = n->right;
    return n->data;
}

static void collect_inorder(const BSTNode *n, void **arr, size_t *idx) {
    if (n) {
        collect_inorder(n->left, arr, idx);
        arr[(*idx)++] = n->data;
        collect_inorder(n->right, arr, idx);
    }
}

void** bst_to_array(const BSTree *tree, size_t *out_count) {
    if (!tree || !out_count) return NULL;
    *out_count = tree->count;
    if (tree->count == 0)
        return calloc(1, sizeof(void *));   /* non-NULL ⇒ success (empty) */
    void **arr = malloc(tree->count * sizeof(void *));
    if (!arr) {
        *out_count = 0;
        return NULL;                        /* NULL ⇒ alloc failure */
    }
    size_t idx = 0;
    collect_inorder(tree->root, arr, &idx);
    *out_count = idx;
    return arr;
}

/* ── free ────────────────────────────────────────────────────────── */

static void free_rec(BSTNode *n, void (*free_data)(void *)) {
    if (n) {
        free_rec(n->left, free_data);
        free_rec(n->right, free_data);
        free_data_if(free_data, n->data);
        free(n);
    }
}

void bst_free(BSTree *tree) {
    if (!tree) return;
    free_rec(tree->root, tree->free_data);
    free(tree);
}
