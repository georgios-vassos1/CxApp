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

DynArray *dynarray_create(void)
{
    return NULL;   /* stub */
}

void dynarray_push(DynArray *da, int val)
{
    (void)da;
    (void)val;     /* stub */
}

int dynarray_get(const DynArray *da, size_t idx)
{
    (void)da;
    (void)idx;
    return 0;      /* stub */
}

size_t dynarray_size(const DynArray *da)
{
    (void)da;
    return 0;      /* stub */
}

size_t dynarray_capacity(const DynArray *da)
{
    (void)da;
    return 0;      /* stub */
}

void dynarray_free(DynArray *da)
{
    (void)da;      /* stub */
}

/* ------------------------------------------------------------------ */
/*  Pointer arithmetic stubs                                           */
/* ------------------------------------------------------------------ */

int *ptr_find(int *arr, size_t n, int target)
{
    (void)arr;
    (void)n;
    (void)target;
    return NULL;   /* stub */
}

void ptr_reverse(int *arr, size_t n)
{
    (void)arr;
    (void)n;       /* stub */
}

ptrdiff_t ptr_distance(const int *from, const int *to)
{
    (void)from;
    (void)to;
    return 0;      /* stub */
}

/* ------------------------------------------------------------------ */
/*  Ownership / duplication stub                                       */
/* ------------------------------------------------------------------ */

int *int_arr_dup(const int *arr, size_t n)
{
    (void)arr;
    (void)n;
    return NULL;   /* stub */
}
