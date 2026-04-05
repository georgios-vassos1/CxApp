#include <stdio.h>
#include <stdlib.h>
#include "fib_heap.h"

/* Wrap int in heap-allocated storage so fh can hold void*. */
static int *make_int(int v)
{
    int *p = malloc(sizeof(*p));
    if (!p) { perror("malloc"); exit(EXIT_FAILURE); }
    *p = v;
    return p;
}

static int cmp_int(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

int main(void)
{
    /* --- basic insert / find-min / extract-min --- */
    printf("=== Insert & Extract-Min ===\n");
    FibHeap *h = fh_create(cmp_int, free);
    if (!h) { fprintf(stderr, "fh_create failed\n"); return 1; }

    fh_insert(h, make_int(10));
    fh_insert(h, make_int(3));
    fh_insert(h, make_int(15));
    fh_insert(h, make_int(7));
    fh_insert(h, make_int(1));

    printf("size = %zu, empty = %d\n", fh_size(h), fh_is_empty(h));
    printf("find_min = %d\n", *(int *)fh_find_min(h));

    printf("extract order:");
    while (!fh_is_empty(h)) {
        int *v = fh_extract_min(h);
        printf(" %d", *v);
        free(v);
    }
    printf("\n");
    printf("size after drain = %zu\n\n", fh_size(h));
    fh_free(h);

    /* --- decrease-key --- */
    printf("=== Decrease-Key ===\n");
    h = fh_create(cmp_int, free);

    FibNode *n20 = fh_insert(h, make_int(20));
    fh_insert(h, make_int(5));
    FibNode *n30 = fh_insert(h, make_int(30));
    fh_insert(h, make_int(12));

    printf("min before decrease = %d\n", *(int *)fh_find_min(h));

    /* Decrease 20 -> 2 (becomes new min) */
    int *new2 = make_int(2);
    if (fh_decrease_key(h, n20, new2) == 0)
        printf("decreased 20 -> 2, new min = %d\n", *(int *)fh_find_min(h));

    /* Decrease 30 -> 4 */
    int *new4 = make_int(4);
    if (fh_decrease_key(h, n30, new4) == 0)
        printf("decreased 30 -> 4, min still = %d\n", *(int *)fh_find_min(h));

    printf("extract order:");
    while (!fh_is_empty(h)) {
        int *v = fh_extract_min(h);
        printf(" %d", *v);
        free(v);
    }
    printf("\n\n");
    fh_free(h);

    /* --- merge --- */
    printf("=== Merge ===\n");
    FibHeap *h1 = fh_create(cmp_int, free);
    FibHeap *h2 = fh_create(cmp_int, free);

    fh_insert(h1, make_int(8));
    fh_insert(h1, make_int(2));
    fh_insert(h1, make_int(14));

    fh_insert(h2, make_int(6));
    fh_insert(h2, make_int(1));
    fh_insert(h2, make_int(11));

    FibHeap *merged = fh_merge(h1, h2);
    /* h1 and h2 are now invalid; merged owns everything */
    printf("merged size = %zu, min = %d\n", fh_size(merged), *(int *)fh_find_min(merged));
    printf("extract order:");
    while (!fh_is_empty(merged)) {
        int *v = fh_extract_min(merged);
        printf(" %d", *v);
        free(v);
    }
    printf("\n\n");
    fh_free(merged);

    /* --- delete --- */
    printf("=== Delete ===\n");
    h = fh_create(cmp_int, free);

    fh_insert(h, make_int(10));
    FibNode *n50 = fh_insert(h, make_int(50));
    fh_insert(h, make_int(25));
    fh_insert(h, make_int(3));

    printf("before delete: size = %zu, min = %d\n", fh_size(h), *(int *)fh_find_min(h));
    if (fh_delete(h, n50) == 0)
        printf("deleted 50:   size = %zu, min = %d\n", fh_size(h), *(int *)fh_find_min(h));

    printf("extract order:");
    while (!fh_is_empty(h)) {
        int *v = fh_extract_min(h);
        printf(" %d", *v);
        free(v);
    }
    printf("\n");
    fh_free(h);

    return 0;
}
