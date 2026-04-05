#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"


int main() {
    // Initialize the hash table with initial size INIT_HSIZE
    HashTable *hashTable = initHashTable(INIT_HSIZE);

    // Example usage:
    insertRecord(hashTable, "example", 1, 2);
    insertRecord(hashTable, "test", 1, 5);
    insertRecord(hashTable, "list", 1, 4);

    // Insert a record with an existing word
    insertRecord(hashTable, "example", 1, 4);

    // Print the hash table
    printHashTable(hashTable);

    // Delete a record
    deleteRecord(hashTable, "test");
    insertRecord(hashTable, "list", 1, 5);

    // Print the hash table after deleting a record
    printHashTable(hashTable);

    // Free memory allocated for the hash table
    freeHashTable(hashTable);

    return 0;
}
