#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#define INIT_HSIZE 1
#define HIGH_LOAD 4.0
#define LOW_LOAD 1.0

// Structure for each record
typedef struct Record {
    char *word;
    int num_reviews;
    float sum_ratings;
    struct Record *next;
} Record;

// Structure for each bucket in the hash table
typedef struct Bucket {
    struct Record *head;
} Bucket;

// Structure for the hash table
typedef struct HashTable {
    int size;
    int count;
    struct Bucket *table;
} HashTable;

unsigned long hash(char *key, int size);
unsigned long hashx(char *str, int size);
Record* createRecord(char *word, int num_reviews, int sum_ratings);
HashTable* initHashTable(int size);
void insertRecord(HashTable *ht, char *word, int num_reviews, int sum_ratings);
Record* searchRecord(HashTable *hashTable, char *word);
void updateRecord(HashTable *hashTable, char *word, int num_reviews, int sum_ratings);
void deleteRecord(HashTable *hashTable, char *word);
void freeHashTable(HashTable *hashTable);
void printHashTable(HashTable *hashTable);

#endif // HASH_TABLE_H
