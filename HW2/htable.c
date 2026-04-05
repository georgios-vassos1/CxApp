#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htable.h"

/* =========================================================================
 * Struct definition (opaque to consumers)
 * ========================================================================= */

struct HashTable {
    int    size;       /* number of buckets                  */
    int    count;      /* total unique words across buckets  */
    DList **buckets;
};

/* =========================================================================
 * Hash function
 * ========================================================================= */

unsigned long htable_hash(const char *key, int size)
{
    unsigned long h = 5381;
    unsigned char c;
    while ((c = (unsigned char)*key++))
        h = ((h << 5) + h) + c;   /* djb2: h * 33 + c */
    return h % (unsigned long)size;
}

/* =========================================================================
 * Internal helpers
 * ========================================================================= */

static DList **alloc_buckets(int size)
{
    DList **b = malloc((size_t)size * sizeof(DList *));
    if (!b) return NULL;
    for (int i = 0; i < size; i++) {
        b[i] = dlist_create();
        if (!b[i]) {
            for (int j = 0; j < i; j++) dlist_destroy(b[j]);
            free(b);
            return NULL;
        }
    }
    return b;
}

/* Context passed through the dlist_foreach callback during rehash. */
typedef struct {
    DList **new_buckets;
    int     new_size;
    int     err;           /* set to -1 if any insertion fails */
} RehashCtx;

/* Callback: re-insert one node's data into the new bucket array. */
static void rehash_node(const DNode *node, void *arg)
{
    RehashCtx *ctx = arg;
    if (ctx->err) return;   /* short-circuit after first failure */

    unsigned long idx = htable_hash(dnode_word(node), ctx->new_size);
    if (dlist_insert_sorted(ctx->new_buckets[idx],
                            dnode_word(node),
                            dnode_num_reviews(node),
                            dnode_sum_ratings(node)) != 0)
        ctx->err = -1;
}

/*
 * Rebuild the hash table with a new bucket count.
 * Returns 0 on success, -1 on allocation failure (table unchanged).
 */
static int htable_rehash(HashTable *ht, int new_size)
{
    DList **new_buckets = alloc_buckets(new_size);
    if (!new_buckets) return -1;

    RehashCtx ctx = { new_buckets, new_size, 0 };

    for (int i = 0; i < ht->size && !ctx.err; i++)
        dlist_foreach(ht->buckets[i], rehash_node, &ctx);

    if (ctx.err) {
        /* Roll back: destroy new buckets, leave table untouched. */
        for (int i = 0; i < new_size; i++) dlist_destroy(new_buckets[i]);
        free(new_buckets);
        return -1;
    }

    /* Success: destroy old buckets and swap in the new ones. */
    for (int i = 0; i < ht->size; i++)
        dlist_destroy(ht->buckets[i]);
    free(ht->buckets);

    ht->buckets = new_buckets;
    ht->size    = new_size;
    return 0;
}

/* =========================================================================
 * Public API — accessors
 * ========================================================================= */

int htable_bucket_count(const HashTable *ht) { return ht->size;  }
int htable_count       (const HashTable *ht) { return ht->count; }

double htable_load(const HashTable *ht)
{
    return (double)ht->count / ht->size;
}

/* =========================================================================
 * Public API — lifecycle
 * ========================================================================= */

HashTable *htable_create(int size)
{
    if (size < INIT_HSIZE) size = INIT_HSIZE;
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size    = size;
    ht->count   = 0;
    ht->buckets = alloc_buckets(size);
    if (!ht->buckets) { free(ht); return NULL; }
    return ht;
}

void htable_destroy(HashTable *ht)
{
    if (!ht) return;
    for (int i = 0; i < ht->size; i++)
        dlist_destroy(ht->buckets[i]);
    free(ht->buckets);
    free(ht);
}

/* =========================================================================
 * Public API — query / iteration
 * ========================================================================= */

DNode *htable_search(const HashTable *ht, const char *word)
{
    unsigned long idx = htable_hash(word, ht->size);
    return dlist_search(ht->buckets[idx], word);
}

void htable_foreach(const HashTable *ht,
                    void (*cb)(const DNode *node, void *arg), void *arg)
{
    for (int i = 0; i < ht->size; i++)
        dlist_foreach(ht->buckets[i], cb, arg);
}

/* =========================================================================
 * Public API — mutation
 * ========================================================================= */

int htable_insert(HashTable *ht, const char *word,
                  int num_reviews, float sum_ratings)
{
    unsigned long idx      = htable_hash(word, ht->size);
    int           old_size = dlist_size(ht->buckets[idx]);

    if (dlist_insert_sorted(ht->buckets[idx], word,
                            num_reviews, sum_ratings) != 0)
        return -1;

    if (dlist_size(ht->buckets[idx]) > old_size)
        ht->count++;

    if (htable_load(ht) >= HIGH_LOAD)
        htable_rehash(ht, ht->size * 2);   /* best-effort; table still valid on failure */

    return 0;
}

void htable_delete(HashTable *ht, const char *word)
{
    unsigned long idx = htable_hash(word, ht->size);
    if (!dlist_delete(ht->buckets[idx], word))
        return;

    ht->count--;

    if (ht->size / 2 >= INIT_HSIZE && htable_load(ht) <= LOW_LOAD)
        htable_rehash(ht, ht->size / 2);   /* best-effort */
}

/* =========================================================================
 * Public API — debug
 * ========================================================================= */

void htable_print(const HashTable *ht)
{
    int max_bucket = 0;
    for (int i = 0; i < ht->size; i++) {
        int sz = dlist_size(ht->buckets[i]);
        if (sz > max_bucket) max_bucket = sz;
    }

    printf("HashTable | size: %d | count: %d | load: %.4f | max bucket: %d\n",
           ht->size, ht->count, htable_load(ht), max_bucket);

    for (int i = 0; i < ht->size; i++) {
        if (dlist_size(ht->buckets[i]) == 0) continue;
        printf("  [%d] (%d entries):\n", i, dlist_size(ht->buckets[i]));
        dlist_print(ht->buckets[i]);
    }
}
