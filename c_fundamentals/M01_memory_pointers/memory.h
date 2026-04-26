/*
 * memory.h — Module 1: Memory & Pointers (C Fundamentals).
 *
 * Three groups of exercises, each isolating a core C memory concept:
 *
 * 1. DynArray — dynamic array of ints
 *    Practises: malloc, calloc, realloc, free, struct ownership,
 *               the grow-on-demand pattern.
 *
 * 2. Pointer arithmetic
 *    Practises: pointers as array iterators, pointer comparison,
 *               ptrdiff_t, in-place mutation via pointers.
 *
 * 3. Ownership / duplication
 *    Practises: heap-allocating a copy, transferring ownership to
 *               the caller, the "caller must free" contract.
 */

#ifndef C_FUND_MEMORY_H
#define C_FUND_MEMORY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t, ptrdiff_t */

/* ------------------------------------------------------------------ */
/*  1. DynArray — growable int array                                  */
/* ------------------------------------------------------------------ */

/*
 * Opaque handle — callers never see the internals.
 * Internals: a heap-allocated int buffer, a size, and a capacity.
 * Growth strategy: double capacity when full, starting at 4.
 */
typedef struct DynArray DynArray;

/* Allocate and return a new, empty DynArray. Returns NULL on failure. */
DynArray *dynarray_create(void);

/*
 * Append val to the end of da.
 * Doubles capacity via realloc if size == capacity.
 */
void dynarray_push(DynArray *da, int val);

/*
 * Return the element at position idx (0-based).
 * Behaviour is undefined if idx >= dynarray_size(da).
 */
int dynarray_get(const DynArray *da, size_t idx);

/* Number of elements currently stored. */
size_t dynarray_size(const DynArray *da);

/* Current allocated capacity (number of slots, not bytes). */
size_t dynarray_capacity(const DynArray *da);

/* Free the internal buffer and the DynArray struct itself. */
void dynarray_free(DynArray *da);

/* ------------------------------------------------------------------ */
/*  2. Pointer arithmetic                                              */
/* ------------------------------------------------------------------ */

/*
 * Return a pointer to the first element in arr[0..n-1] equal to target.
 * Return NULL if target is not found.
 *
 * Implement using pointer arithmetic (no subscript operator).
 */
int *ptr_find(int *arr, size_t n, int target);

/*
 * Reverse arr[0..n-1] in place.
 * Implement by walking two pointers toward each other and swapping.
 */
void ptr_reverse(int *arr, size_t n);

/*
 * Return the signed number of int-sized steps from `from` to `to`.
 * Equivalent to (to - from) in pointer arithmetic.
 * Both pointers must point into the same array (or one past its end).
 */
ptrdiff_t ptr_distance(const int *from, const int *to);

/* ------------------------------------------------------------------ */
/*  3. Ownership / duplication                                         */
/* ------------------------------------------------------------------ */

/*
 * Allocate and return a heap copy of arr[0..n-1].
 * The caller owns the returned pointer and must free() it.
 * Returns NULL if n == 0 or on allocation failure.
 */
int *int_arr_dup(const int *arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_MEMORY_H */
