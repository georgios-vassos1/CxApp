#ifndef SLIST_H
#define SLIST_H

/*
 * Singly linked list of reviews.
 *
 * Each node owns its review_t (allocated on the heap).  Destroying the
 * list frees all nodes and their reviews.
 *
 * Both SList and SNode are opaque — external code iterates via
 * slist_foreach() and accesses review data through the review_t pointer
 * provided by the callback.
 */

#include "utils.h"   /* review_t */

/* Opaque types — full definitions live in slist.c */
typedef struct SNode SNode;
typedef struct SList SList;

/* ---- lifecycle ---- */

SList *slist_create (void);                    /* NULL on alloc failure */
void   slist_destroy(SList *list);

/* ---- accessors ---- */

int slist_size(const SList *list);

/* ---- mutation ---- */

/*
 * slist_insert: prepends review to the list.  The list takes ownership.
 * Returns 0 on success, -1 on allocation failure (review NOT freed).
 */
int slist_insert(SList *list, review_t *review);

/*
 * slist_delete_reviewer: removes all reviews by reviewer.
 * Returns the number of reviews removed.
 */
int slist_delete_reviewer(SList *list, const char *reviewer);

/* ---- iteration ---- */

/*
 * slist_foreach: calls cb(review, arg) for every review in the list.
 * The callback receives a *const* pointer — it must NOT free the review.
 */
void slist_foreach(const SList *list,
                   void (*cb)(const review_t *review, void *arg), void *arg);

/* ---- query / print ---- */

/*
 * slist_print_movie: prints all reviews whose movie_name matches movie.
 * Pass "*" or NULL to print every review.
 */
void slist_print_movie(const SList *list, const char *movie);

#endif /* SLIST_H */
