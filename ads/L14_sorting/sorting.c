/*
 * sorting.c — TDD RED PHASE: stub implementations.
 *
 * counting_sort and radix_sort are intentionally left as no-ops.
 * Every test that exercises a non-trivial input WILL FAIL.
 * That is the point: the failing tests define what must be implemented.
 *
 * is_sorted_int and is_sorted_uint ARE implemented here.
 * They are verification predicates used by the test harness, not the
 * subject of the TDD cycle, so implementing them up front is correct.
 *
 * GREEN PHASE: replace the stub bodies with real implementations and
 * re-run `make test` until all tests pass.
 */

#include "sorting.h"
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Stubs (RED phase)                                                  */
/* ------------------------------------------------------------------ */

void counting_sort(int *arr, size_t n, int max_val)
{
    if (n == 0) return;

    int *count = calloc((size_t)max_val + 1, sizeof(int));
    for (size_t i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    for (size_t i = 1; i < (size_t)max_val + 1; i++) {
        count[i] += count[i - 1];
    }

    int *output = malloc(n * sizeof(int));
    for (size_t i = n; i > 0; i--) {
        output[--count[arr[i - 1]]] = arr[i - 1];
    }

    memcpy(arr, output, n * sizeof(int));
    free(count);
    free(output);
}

void radix_sort(unsigned int *arr, size_t n)
{
    /* Not yet implemented — leaves arr unchanged. */
    (void)arr;
    (void)n;
}

/* ------------------------------------------------------------------ */
/*  Verification predicates (implemented from the start)              */
/* ------------------------------------------------------------------ */

int is_sorted_int(const int *arr, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1])
            return 0;
    }
    return 1;
}

int is_sorted_uint(const unsigned int *arr, size_t n)
{
    for (size_t i = 1; i < n; i++) {
        if (arr[i] < arr[i - 1])
            return 0;
    }
    return 1;
}
