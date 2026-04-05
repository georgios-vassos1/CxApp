#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// Polynomial rolling hash
static unsigned long ht_hash(const char *key, size_t size) {
    unsigned long hashval = 0;
    while (*key != '\0') {
        hashval = hashval * 31 + (unsigned char)(*key++);
    }
    return hashval % size;
}

static Record* create_record(const char *word, int num_reviews, float avg_rating) {
    Record *rec = malloc(sizeof(Record));
    if (rec == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    rec->word = strdup(word);
    if (rec->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(rec);
        exit(EXIT_FAILURE);
    }
    rec->num_reviews = num_reviews;
    rec->avg_rating = avg_rating;
    rec->next = NULL;
    return rec;
}

HashTable* ht_init(size_t size) {
    HashTable *ht = malloc(sizeof(HashTable));
    if (ht == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    ht->size = size;
    ht->count = 0;
    ht->table = malloc(size * sizeof(Bucket));
    if (ht->table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(ht);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < size; i++) {
        ht->table[i].head = NULL;
    }
    return ht;
}

static void rehash(HashTable *ht, size_t newSize) {
    HashTable *newHT = ht_init(newSize);
    for (size_t i = 0; i < ht->size; i++) {
        Record *rec = ht->table[i].head;
        while (rec != NULL) {
            size_t idx = ht_hash(rec->word, newSize);
            Record *next = rec->next;
            rec->next = newHT->table[idx].head;
            newHT->table[idx].head = rec;
            rec = next;
        }
    }
    free(ht->table);
    ht->table = newHT->table;
    ht->size = newSize;
    free(newHT);
}

void ht_insert(HashTable *ht, const char *word, int num_reviews, float rating) {
    size_t index = ht_hash(word, ht->size);
    Record *cur = ht->table[index].head;

    // Update existing record if word already present
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) {
            cur->avg_rating = (cur->avg_rating * (float)cur->num_reviews + rating)
                              / (float)(cur->num_reviews + num_reviews);
            cur->num_reviews += num_reviews;
            return;
        }
        cur = cur->next;
    }

    // Insert new record at head of bucket
    Record *rec = create_record(word, num_reviews, rating);
    rec->next = ht->table[index].head;
    ht->table[index].head = rec;
    ht->count++;

    double loadFactor = (double)ht->count / (double)ht->size;
    if (loadFactor >= HIGH_LOAD) {
        rehash(ht, ht->size * 2);
    }
}

void ht_delete(HashTable *ht, const char *word) {
    size_t index = ht_hash(word, ht->size);
    Record *prev = NULL;
    Record *cur = ht->table[index].head;
    while (cur != NULL && strcmp(cur->word, word) != 0) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) {
        fprintf(stderr, "Word not found\n");
        return;
    }
    if (prev == NULL) {
        ht->table[index].head = cur->next;
    } else {
        prev->next = cur->next;
    }
    free(cur->word);
    free(cur);
    ht->count--;

    double loadFactor = (double)ht->count / (double)ht->size;
    if (loadFactor <= LOW_LOAD && ht->size > INIT_HSIZE) {
        rehash(ht, ht->size / 2);
    }
}

Record* ht_search(HashTable *ht, const char *word) {
    size_t index = ht_hash(word, ht->size);
    Record *cur = ht->table[index].head;
    while (cur != NULL) {
        if (strcmp(cur->word, word) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

void ht_update(HashTable *ht, const char *word, int num_reviews, float avg_rating) {
    Record *rec = ht_search(ht, word);
    if (rec == NULL) {
        fprintf(stderr, "Word not found\n");
        return;
    }
    rec->num_reviews = num_reviews;
    rec->avg_rating = avg_rating;
}

static void print_record(const Record *rec) {
    printf("%s %d %.1f\n", rec->word, rec->num_reviews, rec->avg_rating);
}

void ht_print(const HashTable *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        const Record *rec = ht->table[i].head;
        while (rec != NULL) {
            print_record(rec);
            rec = rec->next;
        }
    }
}

void ht_free(HashTable *ht) {
    for (size_t i = 0; i < ht->size; i++) {
        Record *rec = ht->table[i].head;
        while (rec != NULL) {
            Record *next = rec->next;
            free(rec->word);
            free(rec);
            rec = next;
        }
    }
    free(ht->table);
    free(ht);
}
