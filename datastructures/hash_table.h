#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#define INIT_HSIZE  16
#define HIGH_LOAD   0.75
#define LOW_LOAD    0.25

typedef struct HTEntry {
    void *data;
    struct HTEntry *next;
} HTEntry;

typedef struct HashTable {
    size_t size;
    size_t count;
    HTEntry **buckets;
    size_t (*hash)(const void *);
    int    (*cmp)(const void *, const void *);
    void   (*free_data)(void *);
} HashTable;

HashTable* ht_init(size_t initial_size,
                   size_t (*hash)(const void *),
                   int    (*cmp)(const void *, const void *),
                   void   (*free_data)(void *));

void  ht_insert(HashTable *ht, void *data);
void* ht_search(const HashTable *ht, const void *probe);
void  ht_delete(HashTable *ht, const void *probe);
void  ht_print(const HashTable *ht, void (*print_fn)(const void *));
void  ht_free(HashTable *ht);

#endif /* HASH_TABLE_H */
