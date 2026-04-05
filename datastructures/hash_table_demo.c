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

    ht_insert(ht, make_word("example", 1, 2.0f));
    ht_insert(ht, make_word("test",    1, 5.0f));
    ht_insert(ht, make_word("list",    1, 4.0f));

    /* inserting with existing key replaces the old entry */
    ht_insert(ht, make_word("example", 2, 3.0f));

    printf("After inserts:\n");
    ht_print(ht, print_word);

    /* search */
    WordRecord probe = { .word = "list" };
    WordRecord *found = ht_search(ht, &probe);
    if (found)
        printf("\nFound: %s %d %.1f\n", found->word, found->num_reviews, found->avg_rating);

    /* delete + re-insert */
    WordRecord del = { .word = "test" };
    ht_delete(ht, &del);
    ht_insert(ht, make_word("list", 2, 4.5f));

    printf("\nAfter delete(test) + replace(list):\n");
    ht_print(ht, print_word);

    ht_free(ht);
    return 0;
}
