#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"

BSTree* bst_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *))
{
    BSTree *tree = malloc(sizeof(BSTree));
    if (!tree) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;
    tree->cmp = cmp;
    tree->free_data = free_data;
    return tree;
}

static BSTNode* create_node(void *data) {
    BSTNode *n = malloc(sizeof(BSTNode));
    if (!n) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    n->data = data;
    n->left = n->right = NULL;
    return n;
}

/* ── helpers ─────────────────────────────────────────────────────── */

static void free_data_if(void (*free_data)(void *), void *data) {
    if (free_data) free_data(data);
}

/* ── recursive insert ────────────────────────────────────────────── */

static BSTNode* insert_rec(BSTNode *node, void *data,
                           int (*cmp)(const void *, const void *),
                           void (*free_data)(void *))
{
    if (node == NULL)
        return create_node(data);

    int c = cmp(data, node->data);
    if (c < 0)
        node->left = insert_rec(node->left, data, cmp, free_data);
    else if (c > 0)
        node->right = insert_rec(node->right, data, cmp, free_data);
    else
        free_data_if(free_data, data);   /* duplicate – discard */

    return node;
}

void bst_insert(BSTree *tree, void *data) {
    tree->root = insert_rec(tree->root, data, tree->cmp, tree->free_data);
}

/* ── iterative insert ────────────────────────────────────────────── */

void bst_insert_iter(BSTree *tree, void *data) {
    BSTNode *newNode = create_node(data);
    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }
    BSTNode *cur = tree->root;
    for (;;) {
        int c = tree->cmp(data, cur->data);
        if (c < 0) {
            if (cur->left == NULL) { cur->left = newNode; return; }
            cur = cur->left;
        } else if (c > 0) {
            if (cur->right == NULL) { cur->right = newNode; return; }
            cur = cur->right;
        } else {
            free_data_if(tree->free_data, data);
            free(newNode);
            return;
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
    return search_rec(tree->root, probe, tree->cmp);
}

/* ── iterative search ────────────────────────────────────────────── */

void* bst_search_iter(const BSTree *tree, const void *probe) {
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
                           void (*free_data)(void *))
{
    if (node == NULL) return NULL;

    int c = cmp(probe, node->data);
    if (c < 0) {
        node->left = delete_rec(node->left, probe, cmp, free_data);
    } else if (c > 0) {
        node->right = delete_rec(node->right, probe, cmp, free_data);
    } else {
        /* found */
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
        node->right = delete_rec(node->right, tmp_data, cmp, free_data);
    }
    return node;
}

void bst_delete(BSTree *tree, const void *probe) {
    tree->root = delete_rec(tree->root, probe, tree->cmp, tree->free_data);
}

/* ── iterative delete ────────────────────────────────────────────── */

void bst_delete_iter(BSTree *tree, const void *probe) {
    BSTNode *parent = NULL;
    BSTNode *cur = tree->root;

    while (cur && tree->cmp(probe, cur->data) != 0) {
        parent = cur;
        cur = (tree->cmp(probe, cur->data) < 0) ? cur->left : cur->right;
    }
    if (cur == NULL) return;

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

void bst_inorder(const BSTree *tree, void (*visit)(const void *))  { inorder_rec(tree->root, visit); }
void bst_preorder(const BSTree *tree, void (*visit)(const void *)) { preorder_rec(tree->root, visit); }
void bst_postorder(const BSTree *tree, void (*visit)(const void *)){ postorder_rec(tree->root, visit); }

/* ── iterative traversals ────────────────────────────────────────── */

void bst_inorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree->root) return;
    Stack stack;
    stack_init(&stack, sizeof(const BSTNode *));
    const BSTNode *cur = tree->root;
    while (cur || !stack_is_empty(&stack)) {
        while (cur) { stack_push(&stack, &cur); cur = cur->left; }
        const BSTNode **pp = stack_pop(&stack);
        cur = *pp;
        free(pp);
        visit(cur->data);
        cur = cur->right;
    }
    stack_free(&stack);
}

void bst_preorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree->root) return;
    Stack stack;
    stack_init(&stack, sizeof(const BSTNode *));
    const BSTNode *r = tree->root;
    stack_push(&stack, &r);
    while (!stack_is_empty(&stack)) {
        const BSTNode **pp = stack_pop(&stack);
        const BSTNode *cur = *pp;
        free(pp);
        visit(cur->data);
        if (cur->right) stack_push(&stack, &cur->right);
        if (cur->left)  stack_push(&stack, &cur->left);
    }
    stack_free(&stack);
}

void bst_postorder_iter(const BSTree *tree, void (*visit)(const void *)) {
    if (!tree->root) return;
    Stack s1, s2;
    stack_init(&s1, sizeof(const BSTNode *));
    stack_init(&s2, sizeof(const BSTNode *));
    const BSTNode *r = tree->root;
    stack_push(&s1, &r);
    while (!stack_is_empty(&s1)) {
        const BSTNode **pp = stack_pop(&s1);
        const BSTNode *cur = *pp;
        free(pp);
        stack_push(&s2, &cur);
        if (cur->left)  stack_push(&s1, &cur->left);
        if (cur->right) stack_push(&s1, &cur->right);
    }
    while (!stack_is_empty(&s2)) {
        const BSTNode **pp = stack_pop(&s2);
        visit((*pp)->data);
        free(pp);
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
    print_rec(tree->root, 0, "Root: ", print_fn);
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
    free_rec(tree->root, tree->free_data);
    free(tree);
}
