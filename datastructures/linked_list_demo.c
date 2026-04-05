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

static void print_entry(const void *d) {
    const Entry *e = d;
    printf("Word: %s, Num_reviews: %d, Sum_ratings: %d\n",
           e->word, e->num_reviews, e->sum_ratings);
}

int main(void) {
    List *list = list_create(free);

    list_insert_end(list, make_entry("example", 5, 20));
    list_insert_end(list, make_entry("test",    3, 15));
    list_insert_end(list, make_entry("list",    8, 30));

    list_display(list, print_entry);

    list_free(list);
    return 0;
}
