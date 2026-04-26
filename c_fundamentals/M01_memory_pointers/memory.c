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
    return NULL;
}

/* TODO: if size==capacity double it with realloc; store val; size++ */
void dynarray_push(DynArray *da, int val)
{
    (void)da;
    (void)val;
}

/* TODO: return da->data[idx] */
int dynarray_get(const DynArray *da, size_t idx)
{
    (void)da;
    (void)idx;
    return 0;
}

/* TODO: return da->size */
size_t dynarray_size(const DynArray *da)
{
    (void)da;
    return 0;
}

/* TODO: return da->capacity */
size_t dynarray_capacity(const DynArray *da)
{
    (void)da;
    return 0;
}

/* TODO: free(da->data) first, then free(da) — order matters */
void dynarray_free(DynArray *da)
{
    (void)da;
}

/* ------------------------------------------------------------------ */
/*  Pointer arithmetic                                                 */
/* ------------------------------------------------------------------ */

/* TODO: walk p from arr to arr+n; return p when *p==target, else NULL */
int *ptr_find(int *arr, size_t n, int target)
{
    (void)arr;
    (void)n;
    (void)target;
    return NULL;
}

/* TODO: lo=arr, hi=arr+n-1; swap *lo/*hi and walk inward until lo>=hi */
void ptr_reverse(int *arr, size_t n)
{
    (void)arr;
    (void)n;
}

/* TODO: return to - from */
ptrdiff_t ptr_distance(const int *from, const int *to)
{
    (void)from;
    (void)to;
    return 0;
}

/* ------------------------------------------------------------------ */
/*  Ownership / duplication                                            */
/* ------------------------------------------------------------------ */

/* TODO: if n==0 return NULL; malloc(n*sizeof(int)), memcpy, return ptr */
int *int_arr_dup(const int *arr, size_t n)
{
    (void)arr;
    (void)n;
    return NULL;
}
