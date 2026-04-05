#ifndef HTABLE_H
#define HTABLE_H

/*
 * Hash table: dynamic array of DList* (doubly linked circular lists).
 *
 * Each bucket is an independently sorted DList.  The table rehashes
 * automatically:
 *   - doubles when  load >= HIGH_LOAD  (after an insert)
 *   - halves  when  load <= LOW_LOAD   (after a delete, only if new
 *                                       size would still be >= INIT_HSIZE)
 *
 * All three constants can be overridden at compile time with -D flags.
 *
 * The HashTable struct is opaque — external code iterates via
 * htable_foreach() and accesses metadata through accessor functions.
 */

#include "dlist.h"

#ifndef INIT_HSIZE
#define INIT_HSIZE 1
#endif

#ifndef HIGH_LOAD
#define HIGH_LOAD 4.0
#endif

#ifndef LOW_LOAD
#define LOW_LOAD 1.0
#endif

/* Opaque type — full definition lives in htable.c */
typedef struct HashTable HashTable;

/* ---- hash function ---- */

/* djb2 hash, result in [0, size). */
unsigned long htable_hash(const char *key, int size);

/* ---- lifecycle ---- */

HashTable *htable_create (int size);             /* NULL on alloc failure */
void       htable_destroy(HashTable *ht);

/* ---- accessors ---- */

int    htable_bucket_count(const HashTable *ht); /* number of buckets */
int    htable_count       (const HashTable *ht); /* unique words stored */
double htable_load        (const HashTable *ht);

/* ---- query ---- */

/* Returns NULL if word is not in the table. */
DNode *htable_search(const HashTable *ht, const char *word);

/*
 * htable_foreach: calls cb(node, arg) for every record across all buckets.
 * Iteration order is bucket-index then lexicographic within each bucket.
 * The callback must NOT mutate the table structure (insert/delete).
 */
void htable_foreach(const HashTable *ht,
                    void (*cb)(const DNode *node, void *arg), void *arg);

/* ---- mutation ---- */

/*
 * htable_insert: accumulates (num_reviews, sum_ratings) for word.
 * Rehashes up if the load factor crosses HIGH_LOAD.
 * Returns 0 on success, -1 on allocation failure.
 */
int  htable_insert(HashTable *ht, const char *word,
                   int num_reviews, float sum_ratings);

/*
 * htable_delete: removes word entirely.
 * Rehashes down if the load factor drops to or below LOW_LOAD.
 */
void htable_delete(HashTable *ht, const char *word);

/* ---- debug ---- */

void htable_print(const HashTable *ht);

#endif /* HTABLE_H */
