#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// Hash function
unsigned long hash(char *key, int size) {
    unsigned long hashval = 0;
    while (*key != '\0') {
        hashval = hashval * 31 + (*key++);
    }
    return hashval % size;
}

unsigned long hashx(char *str, int size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % size;
}

// Function to create a new record
Record* createRecord(char *word, int num_reviews, int sum_ratings) {
    Record *newRecord = (Record*)malloc(sizeof(Record));
    if (newRecord == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newRecord->word = strdup(word);
    newRecord->num_reviews = num_reviews;
    newRecord->sum_ratings = sum_ratings;
    newRecord->next = NULL;
    return newRecord;
}

// Function to initialize the hash table
HashTable* initHashTable(int size) {
    HashTable *hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    hashTable->size = size;
    hashTable->count = 0;
    hashTable->table = (Bucket*)malloc(size * sizeof(Bucket));
    if (hashTable->table == NULL) {
        printf("Memory allocation failed\n");
        free(hashTable);
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        hashTable->table[i].head = NULL;
    }
    return hashTable;
}

// Function to insert a record into the hash table
void insertRecord(HashTable *hashTable, char *word, int num_reviews, int sum_ratings) {
    int index = hash(word, hashTable->size);
    Record *current = hashTable->table[index].head;

    // Check if the word already exists in the hash table
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            // Update the existing record
            current->num_reviews += num_reviews;
            // Update the sum of ratings with the mean of all ratings
            current->sum_ratings = (current->sum_ratings * (current->num_reviews - num_reviews) + sum_ratings) / current->num_reviews;
            return;
        }
        current = current->next;
    }

    // Insert a new record
    Record *newRecord = createRecord(word, num_reviews, sum_ratings);
    newRecord->next = hashTable->table[index].head;
    hashTable->table[index].head = newRecord;
    hashTable->count++;

    // Check if rehashing is required
    double loadFactor = (double)hashTable->count / hashTable->size;
    if (loadFactor >= HIGH_LOAD) {
        // Double the size of the hash table
        int newSize = hashTable->size * 2;
        HashTable *newHashTable = initHashTable(newSize);
        for (int i = 0; i < hashTable->size; i++) {
            Record *record = hashTable->table[i].head;
            while (record != NULL) {
                int newIndex = hash(record->word, newSize);
                Record *nextRecord = record->next;
                record->next = newHashTable->table[newIndex].head;
                newHashTable->table[newIndex].head = record;
                record = nextRecord;
            }
        }
        free(hashTable->table);
        hashTable->table = newHashTable->table;
        hashTable->size = newSize;
        free(newHashTable);
    }
}


// Function to delete a record from the hash table
void deleteRecord(HashTable *hashTable, char *word) {
    int index = hash(word, hashTable->size);
    Record *prev = NULL;
    Record *current = hashTable->table[index].head;
    while (current != NULL && strcmp(current->word, word) != 0) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Word not found\n");
        return;
    }
    if (prev == NULL) {
        hashTable->table[index].head = current->next;
    } else {
        prev->next = current->next;
    }
    free(current->word);
    free(current);
    hashTable->count--;

    // Check if rehashing is required
    double loadFactor = (double)hashTable->count / hashTable->size;
    if (loadFactor <= LOW_LOAD && hashTable->size > INIT_HSIZE) {
        // Halve the size of the hash table
        int newSize = hashTable->size / 2;
        HashTable *newHashTable = initHashTable(newSize);
        for (int i = 0; i < hashTable->size; i++) {
            Record *record = hashTable->table[i].head;
            while (record != NULL) {
                int newIndex = hash(record->word, newSize);
                Record *nextRecord = record->next;
                record->next = newHashTable->table[newIndex].head;
                newHashTable->table[newIndex].head = record;
                record = nextRecord;
            }
        }
        free(hashTable->table);
        hashTable->table = newHashTable->table;
        hashTable->size = newSize;
        free(newHashTable);
    }
}

// Function to search for a record in the hash table
Record* searchRecord(HashTable *hashTable, char *word) {
    int index = hash(word, hashTable->size);
    Record *current = hashTable->table[index].head;
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to print a record
void printRecord(Record *record) {
    printf("%s %d %.1f\n", record->word, record->num_reviews, record->sum_ratings);
}

// Function to print the hash table
void printHashTable(HashTable *hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        Record *record = hashTable->table[i].head;
        while (record != NULL) {
            printRecord(record);
            record = record->next;
        }
    }
}

// Function to free memory allocated for the hash table
void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        Record *record = hashTable->table[i].head;
        while (record != NULL) {
            Record *nextRecord = record->next;
            free(record->word);
            free(record);
            record = nextRecord;
        }
    }
    free(hashTable->table);
    free(hashTable);
}

