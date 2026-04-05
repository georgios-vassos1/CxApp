#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Opaque types — callers use pointers only. */
typedef struct FibNode FibNode;
typedef struct FibHeap FibHeap;

FibHeap* fh_create(int (*cmp)(const void *, const void *),
                   void (*free_data)(void *));

FibNode* fh_insert(FibHeap *heap, void *data);       /* O(1) amortized */
void*    fh_find_min(const FibHeap *heap);            /* O(1) */
void*    fh_extract_min(FibHeap *heap);               /* O(log n) amortized */

int      fh_decrease_key(FibHeap *heap, FibNode *node,
                         void *new_data);             /* O(1) amortized */
int      fh_delete(FibHeap *heap, FibNode *node);     /* O(log n) amortized */

FibHeap* fh_merge(FibHeap *a, FibHeap *b);            /* O(1) */

size_t   fh_size(const FibHeap *heap);
int      fh_is_empty(const FibHeap *heap);
void     fh_free(FibHeap *heap);

#ifdef __cplusplus
}
#endif

#endif /* FIB_HEAP_H */
