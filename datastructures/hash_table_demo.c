#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

typedef struct {
    char  word[64];
    int   num_reviews;
    float avg_rating;
} WordRecord;

static WordRecord* make_word(const char *word, int nr, float ar) {
    WordRecord *w = malloc(sizeof(WordRecord));
    if (!w) { perror("malloc"); exit(EXIT_FAILURE); }
    snprintf(w->word, sizeof(w->word), "%s", word);
    w->num_reviews = nr;
    w->avg_rating  = ar;
    return w;
}

static size_t hash_word(const void *d) {
    const char *s = ((const WordRecord *)d)->word;
    size_t h = 0;
    while (*s) h = h * 31u + (unsigned char)(*s++);
    return h;
}

static int cmp_word(const void *a, const void *b) {
    return strcmp(((const WordRecord *)a)->word,
                  ((const WordRecord *)b)->word);
}

static void print_word(const void *d) {
    const WordRecord *w = d;
    printf("%s %d %.1f\n", w->word, w->num_reviews, w->avg_rating);
}

int main(void) {
    HashTable *ht = ht_init(INIT_HSIZE, hash_word, cmp_word, free);
    if (!ht) { fprintf(stderr, "ht_init failed\n"); return 1; }

    if (ht_insert(ht, make_word("example", 1, 2.0f)) != 0) goto fail;
    if (ht_insert(ht, make_word("test",    1, 5.0f)) != 0) goto fail;
    if (ht_insert(ht, make_word("list",    1, 4.0f)) != 0) goto fail;

    /* inserting with existing key replaces the old entry */
    if (ht_insert(ht, make_word("example", 2, 3.0f)) != 0) goto fail;

    printf("After inserts (size=%zu):\n", ht_size(ht));
    ht_print(ht, print_word);

    /* search */
    WordRecord probe = { .word = "list" };
    WordRecord *found = ht_search(ht, &probe);
    if (found)
        printf("\nFound: %s %d %.1f\n", found->word, found->num_reviews, found->avg_rating);

    /* ht_keys */
    size_t nkeys;
    void **keys = ht_keys(ht, &nkeys);
    if (keys) {
        printf("\nKeys (%zu): ", nkeys);
        for (size_t i = 0; i < nkeys; i++)
            printf("%s ", ((WordRecord *)keys[i])->word);
        printf("\n");
        free(keys);
    }

    /* delete + re-insert */
    WordRecord del = { .word = "test" };
    if (ht_delete(ht, &del) == 0)
        printf("\nDeleted 'test'\n");
    if (ht_insert(ht, make_word("list", 2, 4.5f)) != 0) goto fail;

    printf("\nAfter delete(test) + replace(list) (size=%zu):\n", ht_size(ht));
    ht_print(ht, print_word);

    ht_free(ht);
    return 0;

fail:
    fprintf(stderr, "insert failed\n");
    ht_free(ht);
    return 1;
}
