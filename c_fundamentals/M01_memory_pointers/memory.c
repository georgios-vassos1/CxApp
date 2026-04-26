/*
 * memory.c — TDD RED PHASE: stub implementations.
 *
 * All functions are intentionally unimplemented (stubs).
 * The test suite in memory_test.c defines what each function must do.
 * Your job: replace each stub body with a correct implementation and
 * run `make test` until all tests pass.
 *
 * Allowed headers: <stdlib.h>, <string.h>, <stddef.h>.
 * No other includes needed.
 */

#include "memory.h"
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  DynArray internals — visible only in this translation unit        */
/* ------------------------------------------------------------------ */

struct DynArray {
    int    *data;
    size_t  size;
    size_t  capacity;
};

/* ------------------------------------------------------------------ */
/*  DynArray stubs                                                     */
/* ------------------------------------------------------------------ */

/* TODO: malloc the struct, malloc data buffer (capacity=4), set size=0 */
DynArray *dynarray_create(void)
{
    DynArray *da = malloc(sizeof(DynArray));

    da->data = malloc(4 * sizeof(int));
    da->size = 0;
    da->capacity = 4;

    return da;
}

/* TODO: if size==capacity double it with realloc; store val; size++ */
void dynarray_push(DynArray *da, int val)
{
    if (da->size == da->capacity) {
        /* realloc returns NULL on failure, leaving da->data untouched;
         * assigning directly to da->data would overwrite the only pointer
         * to the old buffer, making it unfreeable (memory leak). */
        int *tmp = realloc(da->data, 2 * da->capacity * sizeof(int));
        if (tmp) {
            da->data = tmp;
            da->capacity *= 2;
        }
    }
    da->data[da->size++] = val;
}

/* TODO: return da->data[idx] */
int dynarray_get(const DynArray *da, size_t idx)
{
    return da->data[idx];
}

/* TODO: return da->size */
size_t dynarray_size(const DynArray *da)
{
    return da->size;
}

/* TODO: return da->capacity */
size_t dynarray_capacity(const DynArray *da)
{
    return da->capacity;
}

/* TODO: free(da->data) first, then free(da) — order matters */
void dynarray_free(DynArray *da)
{
    /* free da->data first: freeing da first would destroy the only
     * pointer to the data buffer, making it impossible to free and
     * leaking that memory. */
    free(da->data);
    free(da);
}

/* ------------------------------------------------------------------ */
/*  Pointer arithmetic                                                 */
/* ------------------------------------------------------------------ */

/* TODO: walk p from arr to arr+n; return p when *p==target, else NULL */
int *ptr_find(int *arr, size_t n, int target)
{
    if (!arr) return NULL;
    for (size_t i = 0; i < n; i++) {
        if (arr[i] == target) {
            return &arr[i];
        }
    }
    return NULL;
}

/* TODO: lo=arr, hi=arr+n-1; swap *lo-*hi and walk inward until lo>=hi */
void ptr_reverse(int *arr, size_t n)
{
    int *lo = arr;
    int *hi = arr + n - 1;
    while (lo < hi) {
        int tmp = *lo;
        *lo++ = *hi;
        *hi-- = tmp;
    }
}

/* TODO: return to - from */
ptrdiff_t ptr_distance(const int *from, const int *to)
{
    return to - from;
}

/* ------------------------------------------------------------------ */
/*  Ownership / duplication                                            */
/* ------------------------------------------------------------------ */

/* TODO: if n==0 return NULL; malloc(n*sizeof(int)), memcpy, return ptr */
int *int_arr_dup(const int *arr, size_t n)
{
    if (n == 0) return NULL;

    int *dup = malloc(n * sizeof(int));
    memcpy(dup, arr, n * sizeof(int));

    return dup;
}
