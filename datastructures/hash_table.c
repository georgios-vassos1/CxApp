#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"

static void free_data_if(void (*free_data)(void *), void *data) {
    if (free_data) free_data(data);
}

HashTable* ht_init(size_t initial_size,
                   size_t (*hash)(const void *),
                   int    (*cmp)(const void *, const void *),
                   void   (*free_data)(void *))
{
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    ht->size      = initial_size;
    ht->count     = 0;
    ht->hash      = hash;
    ht->cmp       = cmp;
    ht->free_data = free_data;
    ht->buckets   = calloc(initial_size, sizeof(HTEntry *));
    if (!ht->buckets) {
        fprintf(stderr, "Memory allocation failed\n");
        free(ht);
        exit(EXIT_FAILURE);
    }
    return ht;
}

static void rehash(HashTable *ht, size_t new_size) {
    HTEntry **new_buckets = calloc(new_size, sizeof(HTEntry *));
    if (!new_buckets) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
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
}

void ht_insert(HashTable *ht, void *data) {
    size_t idx = ht->hash(data) % ht->size;
    HTEntry *cur = ht->buckets[idx];

    /* replace if key already exists */
    while (cur) {
        if (ht->cmp(cur->data, data) == 0) {
            free_data_if(ht->free_data, cur->data);
            cur->data = data;
            return;
        }
        cur = cur->next;
    }

    /* new entry */
    HTEntry *e = malloc(sizeof(HTEntry));
    if (!e) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    e->data = data;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
    ht->count++;

    double lf = (double)ht->count / (double)ht->size;
    if (lf >= HIGH_LOAD)
        rehash(ht, ht->size * 2);
}

void* ht_search(const HashTable *ht, const void *probe) {
    size_t idx = ht->hash(probe) % ht->size;
    HTEntry *cur = ht->buckets[idx];
    while (cur) {
        if (ht->cmp(cur->data, probe) == 0)
            return cur->data;
        cur = cur->next;
    }
    return NULL;
}

void ht_delete(HashTable *ht, const void *probe) {
    size_t idx = ht->hash(probe) % ht->size;
    HTEntry *prev = NULL;
    HTEntry *cur = ht->buckets[idx];

    while (cur && ht->cmp(cur->data, probe) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (!cur) return;

    if (prev)
        prev->next = cur->next;
    else
        ht->buckets[idx] = cur->next;

    free_data_if(ht->free_data, cur->data);
    free(cur);
    ht->count--;

    double lf = (double)ht->count / (double)ht->size;
    if (lf <= LOW_LOAD && ht->size > INIT_HSIZE)
        rehash(ht, ht->size / 2);
}

void ht_print(const HashTable *ht, void (*print_fn)(const void *)) {
    for (size_t i = 0; i < ht->size; i++) {
        const HTEntry *e = ht->buckets[i];
        while (e) {
            print_fn(e->data);
            e = e->next;
        }
    }
}

void ht_free(HashTable *ht) {
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
