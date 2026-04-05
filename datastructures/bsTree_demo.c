#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsTree.h"

typedef struct {
    int    key;
    char   name[32];
    double value;
} Record;

static Record* make_record(int key, const char *name, double value) {
    Record *r = malloc(sizeof(Record));
    if (!r) { perror("malloc"); exit(EXIT_FAILURE); }
    r->key = key;
    snprintf(r->name, sizeof(r->name), "%s", name);
    r->value = value;
    return r;
}

static int cmp_record(const void *a, const void *b) {
    int ka = ((const Record *)a)->key;
    int kb = ((const Record *)b)->key;
    return (ka > kb) - (ka < kb);
}

static void print_record(const void *d) {
    const Record *r = d;
    printf("[%d] %s %.2f", r->key, r->name, r->value);
}

static void visit_record(const void *d) {
    const Record *r = d;
    printf("%d ", r->key);
}

int main(void) {
    BSTree *tree = bst_create(cmp_record, free);
    if (!tree) { fprintf(stderr, "bst_create failed\n"); return 1; }

    if (bst_insert_iter(tree, make_record(4, "delta",   4.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(2, "bravo",   2.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(6, "foxtrot", 6.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(1, "alpha",   1.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(3, "charlie", 3.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(5, "echo",    5.0)) != 0) goto fail;
    if (bst_insert_iter(tree, make_record(7, "golf",    7.0)) != 0) goto fail;

    printf("Inorder traversal of the BST:\n");
    bst_inorder_iter(tree, visit_record);
    printf("\n\n");

    printf("Binary Search Tree (size=%zu):\n", bst_size(tree));
    bst_print(tree, print_record);

    /* search */
    Record probe = { .key = 5 };
    Record *found = bst_search(tree, &probe);
    if (found)
        printf("\nFound key 5: %s %.2f\n", found->name, found->value);

    /* min / max */
    Record *mn = bst_min(tree);
    Record *mx = bst_max(tree);
    if (mn) printf("Min key: %d\n", mn->key);
    if (mx) printf("Max key: %d\n", mx->key);

    /* to_array */
    size_t arr_count;
    void **arr = bst_to_array(tree, &arr_count);
    if (arr) {
        printf("Array (%zu): ", arr_count);
        for (size_t i = 0; i < arr_count; i++)
            printf("%d ", ((Record *)arr[i])->key);
        printf("\n");
        free(arr);
    }

    /* delete */
    Record del = { .key = 2 };
    if (bst_delete(tree, &del) == 0)
        printf("\nDeleted key 2 (size=%zu):\n", bst_size(tree));
    bst_print(tree, print_record);

    bst_free(tree);
    return 0;

fail:
    fprintf(stderr, "insert failed\n");
    bst_free(tree);
    return 1;
}
