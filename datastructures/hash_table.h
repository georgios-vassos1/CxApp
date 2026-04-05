#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

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

int    ht_insert(HashTable *ht, void *data);
void*  ht_search(const HashTable *ht, const void *probe);
int    ht_delete(HashTable *ht, const void *probe);
void   ht_print(const HashTable *ht, void (*print_fn)(const void *));

size_t ht_size(const HashTable *ht);
void** ht_keys(const HashTable *ht, size_t *out_count);
void   ht_foreach(const HashTable *ht, void (*visitor)(void *data));

void   ht_free(HashTable *ht);

#ifdef __cplusplus
}
#endif

#endif /* HASH_TABLE_H */
