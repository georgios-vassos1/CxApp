/*
 * sorting_test.c — Unit tests for counting_sort and radix_sort.
 *
 * TDD RED PHASE
 * -------------
 * These tests are written BEFORE the implementation exists.
 * Non-trivial tests MUST fail with the current stubs in sorting.c.
 * Do not modify tests to make them pass — implement sorting.c instead.
 *
 * Test categories
 * ---------------
 * counting_sort:
 *   Boundary   — empty array, single element, two elements
 *   Structure  — already sorted, reverse sorted, all equal, binary values
 *   Content    — duplicates, mixed values, range extremes
 *   Scale      — 50 000 random integers compared against qsort reference
 *
 * radix_sort:
 *   Boundary   — empty array, single element, two elements
 *   Structure  — already sorted, reverse sorted, all equal
 *   Bit layout — powers of two, near-UINT_MAX values, mixed byte widths
 *   Content    — duplicates
 *   Scale      — 50 000 random unsigned ints compared against qsort reference
 *
 * Why compare against qsort?
 * --------------------------
 * qsort is comparison-based and well-tested by the C runtime.  Using it
 * as a reference oracle verifies both that our output is sorted AND that
 * it is a permutation of the input (same multiset of values).
 *
 * Build and run
 * -------------
 *   make test           (from ads/)
 *   ./L14_sorting/sorting_test
 */

#include "sorting.h"
#include "../test_framework.h"

#include <stdlib.h>
#include <string.h>

/* ================================================================== */
/*  Test helpers                                                       */
/* ================================================================== */

/*
 * Deterministic LCG pseudo-random number generator.
 * Using our own PRNG (rather than rand()) gives reproducible sequences
 * regardless of platform libc implementation.
 *
 * Parameters from Numerical Recipes (Knuth vol. 2).
 */
static unsigned int _seed = 0;

static void seed_prng(unsigned int s) { _seed = s; }

static unsigned int next_rand(void)
{
    _seed = _seed * 1664525u + 1013904223u;
    return _seed;
}

/* Fill arr[0..n-1] with random ints in [0, max_val]. */
static void fill_random_int(int *arr, size_t n, int max_val)
{
    unsigned int range = (unsigned int)max_val + 1u;
    for (size_t i = 0; i < n; i++)
        arr[i] = (int)(next_rand() % range);
}

/* Fill arr[0..n-1] with random unsigned ints (full 32-bit range). */
static void fill_random_uint(unsigned int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++)
        arr[i] = next_rand();
}

/* Comparators for qsort reference oracle. */
static int cmp_int(const void *a, const void *b)
{
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

static int cmp_uint(const void *a, const void *b)
{
    unsigned int va = *(const unsigned int *)a;
    unsigned int vb = *(const unsigned int *)b;
    return (va > vb) - (va < vb);
}

/* ================================================================== */
/*  counting_sort tests                                                */
/* ================================================================== */

/*
 * n = 0: function must return without crashing or touching memory.
 * Passing a non-NULL pointer for safety; n = 0 means nothing is read.
 * This test trivially passes even with a stub — that is correct: doing
 * nothing is the right behaviour for an empty input.
 */
static void test_CountingSort_EmptyArray(void)
{
    int dummy = 99;
    counting_sort(&dummy, 0, 0);
    EXPECT_EQ(dummy, 99);   /* dummy must be untouched */
}

/*
 * n = 1: a single element is trivially sorted.
 * Also passes with stub — correct behaviour.
 */
static void test_CountingSort_SingleElement(void)
{
    int arr[]      = { 4 };
    int expected[] = { 4 };
    counting_sort(arr, 1, 4);
    EXPECT_INT_ARRAY_EQ(arr, expected, 1);
}

/*
 * Two elements, already in order.
 * Passes with stub — but tests that the function does not break
 * already-correct input.
 */
static void test_CountingSort_TwoElements_AlreadySorted(void)
{
    int arr[]      = { 1, 3 };
    int expected[] = { 1, 3 };
    counting_sort(arr, 2, 3);
    EXPECT_INT_ARRAY_EQ(arr, expected, 2);
}

/*
 * Two elements, reversed.
 * MUST FAIL with stub — this is the simplest non-trivial case.
 */
static void test_CountingSort_TwoElements_ReverseSorted(void)
{
    int arr[]      = { 3, 1 };
    int expected[] = { 1, 3 };
    counting_sort(arr, 2, 3);
    EXPECT_INT_ARRAY_EQ(arr, expected, 2);
}

/*
 * Six elements in reverse order.
 * Expected: [0,1,2,3,4,5]  (each value appears exactly once, max_val=5).
 * MUST FAIL with stub.
 */
static void test_CountingSort_ReverseSorted(void)
{
    int arr[]      = { 5, 4, 3, 2, 1, 0 };
    int expected[] = { 0, 1, 2, 3, 4, 5 };
    counting_sort(arr, 6, 5);
    EXPECT_INT_ARRAY_EQ(arr, expected, 6);
}

/*
 * All elements equal.
 * Passes with stub — all-equal arrays are already sorted.
 * Ensures the function does not corrupt uniform input.
 */
static void test_CountingSort_AllEqual(void)
{
    int arr[]      = { 7, 7, 7, 7, 7 };
    int expected[] = { 7, 7, 7, 7, 7 };
    counting_sort(arr, 5, 7);
    EXPECT_INT_ARRAY_EQ(arr, expected, 5);
}

/*
 * Only 0s and 1s — exercises the two-bucket boundary case.
 * Expected: all zeros before all ones.
 * MUST FAIL with stub (mixed order becomes sorted order).
 */
static void test_CountingSort_BinaryValues(void)
{
    int arr[]      = { 1, 0, 1, 0, 0, 1, 1, 0 };
    int expected[] = { 0, 0, 0, 0, 1, 1, 1, 1 };
    counting_sort(arr, 8, 1);
    EXPECT_INT_ARRAY_EQ(arr, expected, 8);
}

/*
 * Sequence with many duplicates and varied values.
 * Reference sorted manually: {1,1,2,3,3,4,5,5,5,6,9}.
 * MUST FAIL with stub.
 */
static void test_CountingSort_Duplicates(void)
{
    int arr[]      = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    int expected[] = { 1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9 };
    counting_sort(arr, 11, 9);
    EXPECT_INT_ARRAY_EQ(arr, expected, 11);
}

/*
 * All elements equal to max_val — single-bucket edge case.
 * Passes with stub — unchanged array is correct.
 */
static void test_CountingSort_AllAtMaxVal(void)
{
    int arr[]      = { 9, 9, 9, 9 };
    int expected[] = { 9, 9, 9, 9 };
    counting_sort(arr, 4, 9);
    EXPECT_INT_ARRAY_EQ(arr, expected, 4);
}

/*
 * Only the two extreme values of the range (0 and max_val).
 * Expected: all zeros first, then all max_vals.
 * MUST FAIL with stub (interleaved input becomes grouped output).
 */
static void test_CountingSort_RangeExtremes(void)
{
    int arr[]      = { 0, 100, 0, 100, 0 };
    int expected[] = { 0, 0, 0, 100, 100 };
    counting_sort(arr, 5, 100);
    EXPECT_INT_ARRAY_EQ(arr, expected, 5);
}

/*
 * Scale test: 50 000 random integers in [0, 255].
 * Verifies that the output:
 *   (a) is non-decreasingly ordered (is_sorted_int), AND
 *   (b) is a permutation of the input (matches qsort reference).
 * MUST FAIL with stub.
 */
static void test_CountingSort_LargeRandom(void)
{
    enum { N = 50000, MAX_VAL = 255 };

    int *arr      = malloc(N * sizeof(int));
    int *expected = malloc(N * sizeof(int));

    seed_prng(42);
    fill_random_int(arr, N, MAX_VAL);
    memcpy(expected, arr, N * sizeof(int));
    qsort(expected, N, sizeof(int), cmp_int);

    counting_sort(arr, N, MAX_VAL);

    EXPECT_TRUE(is_sorted_int(arr, N));
    EXPECT_INT_ARRAY_EQ(arr, expected, N);

    free(arr);
    free(expected);
}

/* ================================================================== */
/*  radix_sort tests                                                   */
/* ================================================================== */

/*
 * n = 0: must not crash.
 * Trivially passes with stub — correct.
 */
static void test_RadixSort_EmptyArray(void)
{
    unsigned int dummy = 99u;
    radix_sort(&dummy, 0);
    EXPECT_EQ(dummy, 99u);
}

/*
 * n = 1: trivially sorted.
 * Passes with stub — correct.
 */
static void test_RadixSort_SingleElement(void)
{
    unsigned int arr[]      = { 42u };
    unsigned int expected[] = { 42u };
    radix_sort(arr, 1);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 1);
}

/*
 * Two elements, already sorted.
 * Passes with stub — correct.
 */
static void test_RadixSort_TwoElements_AlreadySorted(void)
{
    unsigned int arr[]      = { 10u, 20u };
    unsigned int expected[] = { 10u, 20u };
    radix_sort(arr, 2);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 2);
}

/*
 * Two elements, reversed.
 * MUST FAIL with stub.
 */
static void test_RadixSort_TwoElements_ReverseSorted(void)
{
    unsigned int arr[]      = { 20u, 10u };
    unsigned int expected[] = { 10u, 20u };
    radix_sort(arr, 2);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 2);
}

/*
 * Five elements in reverse order.
 * MUST FAIL with stub.
 */
static void test_RadixSort_ReverseSorted(void)
{
    unsigned int arr[]      = { 5u, 4u, 3u, 2u, 1u };
    unsigned int expected[] = { 1u, 2u, 3u, 4u, 5u };
    radix_sort(arr, 5);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 5);
}

/*
 * All elements equal.
 * Passes with stub — correct.
 */
static void test_RadixSort_AllEqual(void)
{
    unsigned int arr[]      = { 77u, 77u, 77u, 77u };
    unsigned int expected[] = { 77u, 77u, 77u, 77u };
    radix_sort(arr, 4);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 4);
}

/*
 * Repeated values interspersed — tests that duplicates are handled.
 * Hand-sorted reference: {1,1,200,300,400,500,600,900}.
 * MUST FAIL with stub.
 */
static void test_RadixSort_Duplicates(void)
{
    unsigned int arr[]      = { 300u, 1u, 400u, 1u, 500u, 900u, 200u, 600u };
    unsigned int expected[] = { 1u, 1u, 200u, 300u, 400u, 500u, 600u, 900u };
    radix_sort(arr, 8);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 8);
}

/*
 * Powers of two that span all four bytes of a 32-bit value.
 * Exercises all four radix passes independently.
 * Expected (ascending): {0, 1, 256, 65536, 16777216}.
 * MUST FAIL with stub.
 */
static void test_RadixSort_PowersOfTwo(void)
{
    unsigned int arr[]      = { 1u << 24, 1u << 16, 1u << 8, 1u, 0u };
    unsigned int expected[] = { 0u, 1u, 1u << 8, 1u << 16, 1u << 24 };
    radix_sort(arr, 5);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 5);
}

/*
 * Values near UINT_MAX — the most-significant byte must be handled.
 * Expected: {0x00000000, 0x00000001, 0xFFFFFFFE, 0xFFFFFFFF}.
 * MUST FAIL with stub.
 */
static void test_RadixSort_NearMaxUint(void)
{
    unsigned int arr[]      = { 0xFFFFFFFFu, 0xFFFFFFFEu, 0x00000001u, 0x00000000u };
    unsigned int expected[] = { 0x00000000u, 0x00000001u, 0xFFFFFFFEu, 0xFFFFFFFFu };
    radix_sort(arr, 4);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 4);
}

/*
 * Mixed byte widths: one-byte values interleaved with four-byte values.
 * Expected (ascending): {1, 3, 5, 0x0000FF00, 0x00FF0000, 0xFF000000}.
 * MUST FAIL with stub.
 */
static void test_RadixSort_MixedByteWidths(void)
{
    unsigned int arr[]      = { 0xFF000000u, 5u, 0x00FF0000u, 3u, 0x0000FF00u, 1u };
    unsigned int expected[] = { 1u, 3u, 5u, 0x0000FF00u, 0x00FF0000u, 0xFF000000u };
    radix_sort(arr, 6);
    EXPECT_UINT_ARRAY_EQ(arr, expected, 6);
}

/*
 * Scale test: 50 000 random unsigned ints (full 32-bit range).
 * Output must be sorted and match the qsort reference (same multiset).
 * MUST FAIL with stub.
 */
static void test_RadixSort_LargeRandom(void)
{
    enum { N = 50000 };

    unsigned int *arr      = malloc(N * sizeof(unsigned int));
    unsigned int *expected = malloc(N * sizeof(unsigned int));

    seed_prng(99);
    fill_random_uint(arr, N);
    memcpy(expected, arr, N * sizeof(unsigned int));
    qsort(expected, N, sizeof(unsigned int), cmp_uint);

    radix_sort(arr, N);

    EXPECT_TRUE(is_sorted_uint(arr, N));
    EXPECT_UINT_ARRAY_EQ(arr, expected, N);

    free(arr);
    free(expected);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("MIT 6.851  L14 — Sorting in Linear Time\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("counting_sort\n");
    printf("-------------\n");
    RUN_TEST(test_CountingSort_EmptyArray);
    RUN_TEST(test_CountingSort_SingleElement);
    RUN_TEST(test_CountingSort_TwoElements_AlreadySorted);
    RUN_TEST(test_CountingSort_TwoElements_ReverseSorted);
    RUN_TEST(test_CountingSort_ReverseSorted);
    RUN_TEST(test_CountingSort_AllEqual);
    RUN_TEST(test_CountingSort_BinaryValues);
    RUN_TEST(test_CountingSort_Duplicates);
    RUN_TEST(test_CountingSort_AllAtMaxVal);
    RUN_TEST(test_CountingSort_RangeExtremes);
    RUN_TEST(test_CountingSort_LargeRandom);

    printf("\nradix_sort\n");
    printf("----------\n");
    RUN_TEST(test_RadixSort_EmptyArray);
    RUN_TEST(test_RadixSort_SingleElement);
    RUN_TEST(test_RadixSort_TwoElements_AlreadySorted);
    RUN_TEST(test_RadixSort_TwoElements_ReverseSorted);
    RUN_TEST(test_RadixSort_ReverseSorted);
    RUN_TEST(test_RadixSort_AllEqual);
    RUN_TEST(test_RadixSort_Duplicates);
    RUN_TEST(test_RadixSort_PowersOfTwo);
    RUN_TEST(test_RadixSort_NearMaxUint);
    RUN_TEST(test_RadixSort_MixedByteWidths);
    RUN_TEST(test_RadixSort_LargeRandom);

    return TEST_SUMMARY();
}
