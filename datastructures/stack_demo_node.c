#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "bsTree.h"

/* A small struct to push onto the stack, demonstrating that the
   generic BST node now holds void *data. */
typedef struct { int id; } Item;

static int cmp_item(const void *a, const void *b) {
    int ka = ((const Item *)a)->id;
    int kb = ((const Item *)b)->id;
    return (ka > kb) - (ka < kb);
}

static Item* make_item(int id) {
    Item *it = malloc(sizeof(Item));
    if (!it) { perror("malloc"); exit(EXIT_FAILURE); }
    it->id = id;
    return it;
}

int main(void) {
    /* Build a small BST just to get some BSTNode pointers */
    BSTree *tree = bst_create(cmp_item, free);
    if (!tree) { fprintf(stderr, "bst_create failed\n"); return 1; }
    if (bst_insert(tree, make_item(1)) != 0) goto fail;
    if (bst_insert(tree, make_item(2)) != 0) goto fail;
    if (bst_insert(tree, make_item(3)) != 0) goto fail;

    /* Push BSTNode pointers onto the stack */
    Stack stack;
    if (stack_init(&stack, sizeof(BSTNode *)) != 0) {
        fprintf(stderr, "stack_init failed\n");
        bst_free(tree);
        return 1;
    }

    /* Walk the tree manually to get node pointers for the stack demo */
    BSTNode *n1 = bst_root(tree);                       /* key 1 */
    BSTNode *n2 = bst_node_right(n1);                   /* key 2 */
    BSTNode *n3 = bst_node_right(n2);                   /* key 3 */

    if (stack_push(&stack, &n1) != 0) goto fail_both;
    if (stack_push(&stack, &n2) != 0) goto fail_both;
    if (stack_push(&stack, &n3) != 0) goto fail_both;

    BSTNode *popped;
    if (stack_pop(&stack, &popped) == 0)
        printf("Popped: %d\n", ((Item *)bst_node_data(popped))->id);

    if (stack_pop(&stack, &popped) == 0)
        printf("Popped: %d\n", ((Item *)bst_node_data(popped))->id);

    BSTNode *top;
    if (stack_peek(&stack, &top) == 0)
        printf("Top element: %d\n", ((Item *)bst_node_data(top))->id);

    stack_free(&stack);
    bst_free(tree);
    return 0;

fail_both:
    stack_free(&stack);
fail:
    fprintf(stderr, "operation failed\n");
    bst_free(tree);
    return 1;
}
