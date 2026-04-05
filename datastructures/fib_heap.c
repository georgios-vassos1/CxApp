#include <stdlib.h>
#include <math.h>
#include "fib_heap.h"
#include "cxds_internal.h"

struct FibNode {
    void    *data;
    FibNode *parent, *child;    /* parent and one child */
    FibNode *left, *right;      /* circular doubly-linked sibling list */
    int      degree;            /* number of children */
    int      mark;              /* lost a child since becoming a non-root child */
};

struct FibHeap {
    FibNode *min;               /* pointer to minimum node in root list */
    size_t   count;
    int    (*cmp)(const void *, const void *);
    void   (*free_data)(void *);
};

/* ------------------------------------------------------------------ */
/*  Internal helpers                                                   */
/* ------------------------------------------------------------------ */

static FibNode *make_node(void *data)
{
    FibNode *n = calloc(1, sizeof(*n));
    if (!n) return NULL;
    n->data  = data;
    n->left  = n;
    n->right = n;
    return n;
}

/* Splice two circular doubly-linked lists together.  Returns the head
   of the combined list (the one with the smaller of the two heads is
   arbitrary — caller decides which pointer to keep). */
static void splice_lists(FibNode *a, FibNode *b)
{
    if (!a || !b) return;
    FibNode *a_right = a->right;
    FibNode *b_left  = b->left;
    a->right       = b;
    b->left        = a;
    a_right->left  = b_left;
    b_left->right  = a_right;
}

/* Remove node from its circular sibling list. */
static void list_remove(FibNode *n)
{
    n->left->right = n->right;
    n->right->left = n->left;
    n->left  = n;
    n->right = n;
}

/* Link y as a child of x (both must be root-list nodes). */
static void link(FibNode *y, FibNode *x)
{
    list_remove(y);
    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left  = y;
        y->right = y;
    } else {
        splice_lists(x->child, y);
    }
    x->degree++;
    y->mark = 0;
}

/* Max possible degree for n nodes: floor(log_phi(n)).  46 covers the
   full size_t range on 64-bit systems. */
#define MAX_DEGREE 46

static void consolidate(FibHeap *heap)
{
    FibNode *A[MAX_DEGREE + 1] = {NULL};

    /* Collect root nodes into an array first, because the root list
       will be mutated during linking. */
    size_t root_count = 0;
    FibNode *x = heap->min;
    if (!x) return;
    FibNode *cur = x;
    do { root_count++; cur = cur->right; } while (cur != x);

    FibNode **roots = malloc(root_count * sizeof(*roots));
    if (!roots) return;                     /* OOM — leave heap as-is */
    cur = x;
    for (size_t i = 0; i < root_count; i++) {
        roots[i] = cur;
        cur = cur->right;
    }

    for (size_t i = 0; i < root_count; i++) {
        x = roots[i];
        int d = x->degree;
        while (A[d]) {
            FibNode *y = A[d];
            if (heap->cmp(y->data, x->data) < 0) {
                FibNode *tmp = x; x = y; y = tmp;
            }
            link(y, x);
            A[d] = NULL;
            d++;
        }
        A[d] = x;
    }
    free(roots);

    /* Rebuild the root list from the degree array. */
    heap->min = NULL;
    for (int i = 0; i <= MAX_DEGREE; i++) {
        if (!A[i]) continue;
        A[i]->left   = A[i];
        A[i]->right  = A[i];
        A[i]->parent = NULL;
        if (!heap->min) {
            heap->min = A[i];
        } else {
            splice_lists(heap->min, A[i]);
            if (heap->cmp(A[i]->data, heap->min->data) < 0)
                heap->min = A[i];
        }
    }
}

static void cut(FibHeap *heap, FibNode *node, FibNode *parent)
{
    if (node->right == node) {
        parent->child = NULL;
    } else {
        if (parent->child == node)
            parent->child = node->right;
        list_remove(node);
    }
    parent->degree--;

    node->parent = NULL;
    node->mark   = 0;
    node->left   = node;
    node->right  = node;
    splice_lists(heap->min, node);
}

static void cascading_cut(FibHeap *heap, FibNode *node)
{
    FibNode *parent = node->parent;
    if (!parent) return;
    if (!node->mark) {
        node->mark = 1;
    } else {
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }
}

/* Recursively free all nodes in a circular sibling list. */
static void free_nodes(FibNode *start, void (*free_data)(void *))
{
    if (!start) return;
    /* Break the circular link first. */
    FibNode *cur = start;
    do {
        FibNode *next = cur->right;
        free_nodes(cur->child, free_data);
        free_data_if(free_data, cur->data);
        free(cur);
        cur = next;
    } while (cur != start);
}

/* ------------------------------------------------------------------ */
/*  Public API                                                         */
/* ------------------------------------------------------------------ */

FibHeap *fh_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *))
{
    if (!cmp) return NULL;
    FibHeap *h = calloc(1, sizeof(*h));
    if (!h) return NULL;
    h->cmp       = cmp;
    h->free_data = free_data;
    return h;
}

FibNode *fh_insert(FibHeap *heap, void *data)
{
    if (!heap) return NULL;
    FibNode *n = make_node(data);
    if (!n) return NULL;

    if (!heap->min) {
        heap->min = n;
    } else {
        splice_lists(heap->min, n);
        if (heap->cmp(n->data, heap->min->data) < 0)
            heap->min = n;
    }
    heap->count++;
    return n;
}

void *fh_find_min(const FibHeap *heap)
{
    if (!heap || !heap->min) return NULL;
    return heap->min->data;
}

void *fh_extract_min(FibHeap *heap)
{
    if (!heap || !heap->min) return NULL;

    FibNode *z = heap->min;

    /* Add all children of z to the root list. */
    if (z->child) {
        FibNode *child = z->child;
        /* Clear parent pointers. */
        FibNode *cur = child;
        do {
            cur->parent = NULL;
            cur = cur->right;
        } while (cur != child);

        splice_lists(z, child);
    }

    /* Remove z from root list. */
    if (z->right == z) {
        heap->min = NULL;
    } else {
        heap->min = z->right;
        list_remove(z);
        consolidate(heap);
    }

    void *data = z->data;
    free(z);
    heap->count--;
    return data;
}

int fh_decrease_key(FibHeap *heap, FibNode *node, void *new_data)
{
    if (!heap || !node) return -1;
    if (heap->cmp(new_data, node->data) > 0) return -1;  /* not a decrease */

    node->data = new_data;
    FibNode *parent = node->parent;
    if (parent && heap->cmp(node->data, parent->data) < 0) {
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }
    if (heap->cmp(node->data, heap->min->data) < 0)
        heap->min = node;

    return 0;
}

int fh_delete(FibHeap *heap, FibNode *node)
{
    if (!heap || !node) return -1;

    /* Cut node to root and force it to become the minimum. */
    FibNode *parent = node->parent;
    if (parent) {
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }
    heap->min = node;

    /* Now extract it. */
    void *data = fh_extract_min(heap);
    free_data_if(heap->free_data, data);
    return 0;
}

FibHeap *fh_merge(FibHeap *a, FibHeap *b)
{
    if (!a && !b) return NULL;
    if (!a) return b;
    if (!b) return a;

    /* Merge into a, free b's shell. */
    if (a->min && b->min) {
        splice_lists(a->min, b->min);
        if (a->cmp(b->min->data, a->min->data) < 0)
            a->min = b->min;
    } else if (!a->min) {
        a->min = b->min;
    }

    a->count += b->count;
    /* a inherits its own cmp / free_data; b's are discarded. */
    free(b);
    return a;
}

size_t fh_size(const FibHeap *heap)
{
    if (!heap) return 0;
    return heap->count;
}

int fh_is_empty(const FibHeap *heap)
{
    if (!heap) return 1;
    return heap->count == 0;
}

void fh_free(FibHeap *heap)
{
    if (!heap) return;
    free_nodes(heap->min, heap->free_data);
    free(heap);
}
