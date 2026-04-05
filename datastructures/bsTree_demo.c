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

    bst_insert_iter(tree, make_record(4, "delta",   4.0));
    bst_insert_iter(tree, make_record(2, "bravo",   2.0));
    bst_insert_iter(tree, make_record(6, "foxtrot", 6.0));
    bst_insert_iter(tree, make_record(1, "alpha",   1.0));
    bst_insert_iter(tree, make_record(3, "charlie", 3.0));
    bst_insert_iter(tree, make_record(5, "echo",    5.0));
    bst_insert_iter(tree, make_record(7, "golf",    7.0));

    printf("Inorder traversal of the BST:\n");
    bst_inorder_iter(tree, visit_record);
    printf("\n\n");

    printf("Binary Search Tree:\n");
    bst_print(tree, print_record);

    /* search */
    Record probe = { .key = 5 };
    Record *found = bst_search(tree, &probe);
    if (found)
        printf("\nFound key 5: %s %.2f\n", found->name, found->value);

    /* delete */
    Record del = { .key = 2 };
    bst_delete(tree, &del);
    printf("\nBinary Search Tree after deleting key 2:\n");
    bst_print(tree, print_record);

    bst_free(tree);
    return 0;
}
