#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "cxds_internal.h"

/* ── struct definitions (private) ────────────────────────────────── */

typedef struct HTEntry {
    void *data;
    struct HTEntry *next;
} HTEntry;

struct HashTable {
    size_t size;
    size_t count;
    HTEntry **buckets;
    size_t (*hash)(const void *);
    int    (*cmp)(const void *, const void *);
    void   (*free_data)(void *);
};

static const double HIGH_LOAD = 0.75;
static const double LOW_LOAD  = 0.125;   /* wide hysteresis gap vs HIGH_LOAD */

HashTable* ht_init(size_t initial_size,
                   size_t (*hash)(const void *),
                   int    (*cmp)(const void *, const void *),
                   void   (*free_data)(void *))
{
    if (initial_size == 0) return NULL;
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->buckets = calloc(initial_size, sizeof(HTEntry *));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    ht->size      = initial_size;
    ht->count     = 0;
    ht->hash      = hash;
    ht->cmp       = cmp;
    ht->free_data = free_data;
    return ht;
}

/* Returns 0 on success, -1 on allocation failure (old buckets kept). */
static int rehash(HashTable *ht, size_t new_size) {
    HTEntry **new_buckets = calloc(new_size, sizeof(HTEntry *));
    if (!new_buckets) return -1;
    for (size_t i = 0; i < ht->size; i++) {
        HTEntry *e = ht->buckets[i];
        while (e) {
            HTEntry *next = e->next;
            size_t idx = ht->hash(e->data) % new_size;
            e->next = new_buckets[idx];
            new_buckets[idx] = e;
            e = next;
        }
    }
    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->size = new_size;
    return 0;
}

int ht_insert(HashTable *ht, void *data) {
    if (!ht) return -1;
    size_t idx = ht->hash(data) % ht->size;
    HTEntry *cur = ht->buckets[idx];

    /* replace if key already exists */
    while (cur) {
        if (ht->cmp(cur->data, data) == 0) {
            free_data_if(ht->free_data, cur->data);
            cur->data = data;
            return 0;
        }
        cur = cur->next;
    }

    /* new entry */
    HTEntry *e = malloc(sizeof(HTEntry));
    if (!e) return -1;
    e->data = data;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
    ht->count++;

    double lf = (double)ht->count / (double)ht->size;
    if (lf >= HIGH_LOAD && ht->size <= SIZE_MAX / 2)
        rehash(ht, ht->size * 2);  /* best-effort grow; insert already succeeded */

    return 0;
}

void* ht_search(const HashTable *ht, const void *probe) {
    if (!ht) return NULL;
    size_t idx = ht->hash(probe) % ht->size;
    HTEntry *cur = ht->buckets[idx];
    while (cur) {
        if (ht->cmp(cur->data, probe) == 0)
            return cur->data;
        cur = cur->next;
    }
    return NULL;
}

int ht_delete(HashTable *ht, const void *probe) {
    if (!ht) return -1;
    size_t idx = ht->hash(probe) % ht->size;
    HTEntry *prev = NULL;
    HTEntry *cur = ht->buckets[idx];

    while (cur && ht->cmp(cur->data, probe) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return -1;

    if (prev)
        prev->next = cur->next;
    else
        ht->buckets[idx] = cur->next;

    free_data_if(ht->free_data, cur->data);
    free(cur);
    ht->count--;

    double lf = (double)ht->count / (double)ht->size;
    if (lf <= LOW_LOAD && ht->size > INIT_HSIZE)
        rehash(ht, ht->size / 2);  /* best-effort shrink */

    return 0;
}

void ht_print(const HashTable *ht, void (*print_fn)(const void *)) {
    if (!ht) return;
    for (size_t i = 0; i < ht->size; i++) {
        const HTEntry *e = ht->buckets[i];
        while (e) {
            print_fn(e->data);
            e = e->next;
        }
    }
}

/* ── accessors ───────────────────────────────────────────────────── */

size_t ht_size(const HashTable *ht) {
    return ht ? ht->count : 0;
}

void** ht_keys(const HashTable *ht, size_t *out_count) {
    if (!ht || !out_count) return NULL;
    *out_count = ht->count;
    if (ht->count == 0)
        return calloc(1, sizeof(void *));   /* non-NULL ⇒ success (empty) */
    void **arr = malloc(ht->count * sizeof(void *));
    if (!arr) {
        *out_count = 0;
        return NULL;                        /* NULL ⇒ alloc failure */
    }
    size_t k = 0;
    for (size_t i = 0; i < ht->size; i++) {
        HTEntry *e = ht->buckets[i];
        while (e) {
            arr[k++] = e->data;
            e = e->next;
        }
    }
    *out_count = k;
    return arr;
}

void ht_foreach(HashTable *ht, void (*visitor)(void *data)) {
    if (!ht) return;
    for (size_t i = 0; i < ht->size; i++) {
        HTEntry *e = ht->buckets[i];
        while (e) {
            visitor(e->data);
            e = e->next;
        }
    }
}

/* ── free ────────────────────────────────────────────────────────── */

void ht_free(HashTable *ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->size; i++) {
        HTEntry *e = ht->buckets[i];
        while (e) {
            HTEntry *next = e->next;
            free_data_if(ht->free_data, e->data);
            free(e);
            e = next;
        }
    }
    free(ht->buckets);
    free(ht);
}
