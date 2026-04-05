#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slist.h"

/* =========================================================================
 * Struct definitions (opaque to consumers)
 * ========================================================================= */

struct SNode {
    review_t     *review;
    struct SNode *next;
};

struct SList {
    SNode *head;
    int    size;
};

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static void free_review(review_t *r)
{
    if (!r) return;
    free(r->movie_name);
    free(r->reviewer_name);
    free(r->review_text);
    free(r);
}

/* =========================================================================
 * Public API
 * ========================================================================= */

SList *slist_create(void)
{
    SList *list = malloc(sizeof(SList));
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

void slist_destroy(SList *list)
{
    if (!list) return;
    SNode *curr = list->head;
    while (curr) {
        SNode *nxt = curr->next;
        free_review(curr->review);
        free(curr);
        curr = nxt;
    }
    free(list);
}

int slist_size(const SList *list)
{
    return list->size;
}

int slist_insert(SList *list, review_t *review)
{
    SNode *node = malloc(sizeof(SNode));
    if (!node) return -1;
    node->review = review;
    node->next   = list->head;
    list->head   = node;
    list->size++;
    return 0;
}

int slist_delete_reviewer(SList *list, const char *reviewer)
{
    int    removed = 0;
    SNode *prev    = NULL;
    SNode *curr    = list->head;

    while (curr) {
        if (strcmp(curr->review->reviewer_name, reviewer) == 0) {
            SNode *nxt = curr->next;
            if (prev) prev->next = nxt;
            else      list->head = nxt;
            free_review(curr->review);
            free(curr);
            list->size--;
            removed++;
            curr = nxt;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    return removed;
}

void slist_foreach(const SList *list,
                   void (*cb)(const review_t *review, void *arg), void *arg)
{
    for (SNode *curr = list->head; curr; curr = curr->next)
        cb(curr->review, arg);
}

void slist_print_movie(const SList *list, const char *movie)
{
    int   found = 0;
    int   all   = (!movie || strcmp(movie, "*") == 0);
    SNode *curr = list->head;

    while (curr) {
        if (all || strcmp(curr->review->movie_name, movie) == 0) {
            printf("Movie   : %s\n", curr->review->movie_name);
            printf("Reviewer: %s\n", curr->review->reviewer_name);
            printf("Score   : %.1f\n", curr->review->review_score);
            printf("Text    : %s\n\n", curr->review->review_text);
            found = 1;
        }
        curr = curr->next;
    }
    if (!found)
        printf("No reviews found.\n");
}
