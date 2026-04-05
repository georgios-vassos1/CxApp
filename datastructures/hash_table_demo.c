#include <stdio.h>
#include "hash_table.h"

int main(void) {
    HashTable *ht = ht_init(INIT_HSIZE);

    ht_insert(ht, "example", 1, 2);
    ht_insert(ht, "test", 1, 5);
    ht_insert(ht, "list", 1, 4);

    // Insert with an existing word (updates running average)
    ht_insert(ht, "example", 1, 4);

    ht_print(ht);

    ht_delete(ht, "test");
    ht_insert(ht, "list", 1, 5);

    ht_print(ht);

    ht_free(ht);

    return 0;
}
