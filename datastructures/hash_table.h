#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#define INIT_HSIZE  16
#define HIGH_LOAD   0.75
#define LOW_LOAD    0.25

typedef struct Record {
    char *word;
    int num_reviews;
    float avg_rating;
    struct Record *next;
} Record;

typedef struct Bucket {
    struct Record *head;
} Bucket;

typedef struct HashTable {
    size_t size;
    size_t count;
    struct Bucket *table;
} HashTable;

HashTable* ht_init(size_t size);
void    ht_insert(HashTable *ht, const char *word, int num_reviews, float rating);
Record* ht_search(HashTable *ht, const char *word);
void    ht_update(HashTable *ht, const char *word, int num_reviews, float avg_rating);
void    ht_delete(HashTable *ht, const char *word);
void    ht_free(HashTable *ht);
void    ht_print(const HashTable *ht);

#endif // HASH_TABLE_H
