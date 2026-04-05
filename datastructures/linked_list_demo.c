#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

typedef struct {
    char word[64];
    int  num_reviews;
    int  sum_ratings;
} Entry;

static Entry* make_entry(const char *word, int nr, int sr) {
    Entry *e = malloc(sizeof(Entry));
    if (!e) { perror("malloc"); exit(EXIT_FAILURE); }
    snprintf(e->word, sizeof(e->word), "%s", word);
    e->num_reviews = nr;
    e->sum_ratings = sr;
    return e;
}

static int cmp_entry(const void *a, const void *b) {
    return strcmp(((const Entry *)a)->word, ((const Entry *)b)->word);
}

static void print_entry(const void *d) {
    const Entry *e = d;
    printf("Word: %s, Num_reviews: %d, Sum_ratings: %d\n",
           e->word, e->num_reviews, e->sum_ratings);
}

int main(void) {
    List *list = list_create(free);
    if (!list) { fprintf(stderr, "list_create failed\n"); return 1; }

    if (list_insert_end(list, make_entry("example", 5, 20)) != 0) goto fail;
    if (list_insert_end(list, make_entry("test",    3, 15)) != 0) goto fail;
    if (list_insert_end(list, make_entry("list",    8, 30)) != 0) goto fail;

    /* insert_front */
    if (list_insert_front(list, make_entry("front", 1, 5)) != 0) goto fail;

    printf("List (size=%zu):\n", list_size(list));
    list_display(list, print_entry);

    /* search */
    Entry probe = { .word = "test" };
    Entry *found = list_search(list, &probe, cmp_entry);
    if (found)
        printf("\nFound: %s reviews=%d\n", found->word, found->num_reviews);

    /* get by index */
    Entry *second = list_get(list, 1);
    if (second)
        printf("Index 1: %s\n", second->word);

    /* delete */
    Entry del = { .word = "test" };
    if (list_delete(list, &del, cmp_entry) == 0)
        printf("\nAfter deleting 'test' (size=%zu):\n", list_size(list));
    list_display(list, print_entry);

    list_free(list);
    return 0;

fail:
    fprintf(stderr, "insert failed\n");
    list_free(list);
    return 1;
}
