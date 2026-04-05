#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

/* =========================================================================
 * Struct definitions (opaque to consumers)
 * ========================================================================= */

struct DNode {
    char         *word;
    int           num_reviews;
    float         sum_ratings;
    struct DNode *prev;
    struct DNode *next;
};

struct DList {
    DNode *sentinel;   /* permanent dummy head/tail node */
    int    size;       /* number of real (non-sentinel) nodes */
};

/* =========================================================================
 * Node accessors
 * ========================================================================= */

const char *dnode_word       (const DNode *n) { return n->word;        }
int         dnode_num_reviews(const DNode *n) { return n->num_reviews; }
float       dnode_sum_ratings(const DNode *n) { return n->sum_ratings; }

void dnode_accumulate(DNode *n, int delta_reviews, float delta_ratings)
{
    n->num_reviews += delta_reviews;
    n->sum_ratings += delta_ratings;
}

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static DNode *make_node(const char *word, int num_reviews, float sum_ratings)
{
    DNode *node = malloc(sizeof(DNode));
    if (!node) return NULL;
    node->word = strdup(word);
    if (!node->word) { free(node); return NULL; }
    node->num_reviews = num_reviews;
    node->sum_ratings = sum_ratings;
    node->prev = node->next = NULL;
    return node;
}

/* Link 'node' immediately before 'successor'. */
static void link_before(DNode *successor, DNode *node)
{
    node->next             = successor;
    node->prev             = successor->prev;
    successor->prev->next  = node;
    successor->prev        = node;
}

/* Unlink 'node' from its neighbours (does NOT free it). */
static void dnode_unlink(DNode *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/* =========================================================================
 * Public API
 * ========================================================================= */

DList *dlist_create(void)
{
    DList *list = malloc(sizeof(DList));
    if (!list) return NULL;

    DNode *s = malloc(sizeof(DNode));
    if (!s) { free(list); return NULL; }

    s->word        = NULL;
    s->num_reviews = 0;
    s->sum_ratings = 0.0f;
    s->prev        = s;
    s->next        = s;

    list->sentinel = s;
    list->size     = 0;
    return list;
}

void dlist_destroy(DList *list)
{
    if (!list) return;
    DNode *curr = list->sentinel->next;
    while (curr != list->sentinel) {
        DNode *nxt = curr->next;
        free(curr->word);
        free(curr);
        curr = nxt;
    }
    free(list->sentinel);
    free(list);
}

int dlist_size(const DList *list)
{
    return list->size;
}

DNode *dlist_search(const DList *list, const char *word)
{
    DNode *curr = list->sentinel->next;
    while (curr != list->sentinel) {
        int cmp = strcmp(curr->word, word);
        if (cmp == 0) return curr;
        if (cmp  > 0) break;
        curr = curr->next;
    }
    return NULL;
}

void dlist_foreach(const DList *list,
                   void (*cb)(const DNode *node, void *arg), void *arg)
{
    DNode *curr = list->sentinel->next;
    while (curr != list->sentinel) {
        DNode *nxt = curr->next;   /* safe if cb causes indirect effects */
        cb(curr, arg);
        curr = nxt;
    }
}

int dlist_insert_sorted(DList *list, const char *word,
                        int num_reviews, float sum_ratings)
{
    DNode *curr = list->sentinel->next;
    while (curr != list->sentinel) {
        int cmp = strcmp(curr->word, word);
        if (cmp == 0) {
            curr->num_reviews += num_reviews;
            curr->sum_ratings += sum_ratings;
            return 0;
        }
        if (cmp > 0) break;
        curr = curr->next;
    }

    DNode *node = make_node(word, num_reviews, sum_ratings);
    if (!node) return -1;

    link_before(curr, node);
    list->size++;
    return 0;
}

int dlist_delete(DList *list, const char *word)
{
    DNode *node = dlist_search(list, word);
    if (!node) return 0;
    dnode_unlink(node);
    free(node->word);
    free(node);
    list->size--;
    return 1;
}

void dlist_print(const DList *list)
{
    DNode *curr = list->sentinel->next;
    while (curr != list->sentinel) {
        float avg = (curr->num_reviews > 0)
                    ? curr->sum_ratings / (float)curr->num_reviews
                    : 0.0f;
        printf("    %-20s  reviews: %4d  sum: %7.2f  avg: %.4f\n",
               curr->word, curr->num_reviews, curr->sum_ratings, avg);
        curr = curr->next;
    }
}
