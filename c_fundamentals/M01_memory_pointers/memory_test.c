/*
 * memory_test.c — Unit tests for Module 1: Memory & Pointers.
 *
 * TDD RED PHASE
 * -------------
 * These tests define the required behaviour of every function in
 * memory.h.  All non-trivial tests MUST fail with the current stubs.
 * Implement memory.c until `make test` reports 0 failures.
 *
 * Test groups
 * -----------
 * DynArray
 *   Creation, push, get, size, capacity growth, free (no leak).
 *
 * Pointer arithmetic
 *   ptr_find: hit, miss, first-occurrence.
 *   ptr_reverse: empty, single, even-length, odd-length.
 *   ptr_distance: zero, positive, negative.
 *
 * Ownership / duplication
 *   int_arr_dup: new pointer, correct values, empty input.
 */

#include "memory.h"
#include "../test_framework.h"

#include <stdlib.h>
#include <stddef.h>

/* ================================================================== */
/*  DynArray tests                                                     */
/* ================================================================== */

/*
 * A freshly created DynArray must be non-NULL, have size 0,
 * and have a non-zero initial capacity ready to receive elements.
 */
static void test_DynArray_CreateIsEmpty(void)
{
    DynArray *da = dynarray_create();
    EXPECT_TRUE(da != NULL);
    EXPECT_EQ(dynarray_size(da), (size_t)0);
    EXPECT_TRUE(dynarray_capacity(da) > 0);
    dynarray_free(da);
}

/*
 * After one push, size becomes 1 and get(0) returns the pushed value.
 */
static void test_DynArray_PushOneElement(void)
{
    DynArray *da = dynarray_create();
    dynarray_push(da, 42);
    EXPECT_EQ(dynarray_size(da), (size_t)1);
    EXPECT_EQ(dynarray_get(da, 0), 42);
    dynarray_free(da);
}

/*
 * Pushing n elements must preserve insertion order.
 * get(i) must return the i-th pushed value.
 */
static void test_DynArray_PreservesInsertionOrder(void)
{
    enum { N = 8 };
    DynArray *da = dynarray_create();
    for (int i = 0; i < N; i++)
        dynarray_push(da, i * 10);

    EXPECT_EQ(dynarray_size(da), (size_t)N);
    for (int i = 0; i < N; i++)
        EXPECT_EQ(dynarray_get(da, (size_t)i), i * 10);

    dynarray_free(da);
}

/*
 * Pushing more elements than the initial capacity triggers realloc.
 * Values must survive the growth: all gets must still be correct.
 * Initial capacity is 4, so pushing 32 elements forces several doublings.
 */
static void test_DynArray_GrowsBeyondInitialCapacity(void)
{
    enum { N = 32 };
    DynArray *da = dynarray_create();
    for (int i = 0; i < N; i++)
        dynarray_push(da, i);

    EXPECT_EQ(dynarray_size(da), (size_t)N);
    EXPECT_TRUE(dynarray_capacity(da) >= (size_t)N);

    int all_correct = 1;
    for (int i = 0; i < N; i++) {
        if (dynarray_get(da, (size_t)i) != i) {
            all_correct = 0;
            break;
        }
    }
    EXPECT_TRUE(all_correct);
    dynarray_free(da);
}

/*
 * Capacity must exactly double each time the array is full.
 * Start: capacity = 4.  After 5th push: capacity = 8.
 *                        After 9th push: capacity = 16.
 */
static void test_DynArray_CapacityDoublesOnGrowth(void)
{
    DynArray *da = dynarray_create();
    EXPECT_EQ(dynarray_capacity(da), (size_t)4);

    for (int i = 0; i < 4; i++) dynarray_push(da, i);
    EXPECT_EQ(dynarray_capacity(da), (size_t)4);   /* still fits */

    dynarray_push(da, 99);                          /* triggers first grow */
    EXPECT_EQ(dynarray_capacity(da), (size_t)8);

    for (int i = 0; i < 3; i++) dynarray_push(da, i);  /* fill to 8 */
    EXPECT_EQ(dynarray_capacity(da), (size_t)8);

    dynarray_push(da, 99);                          /* triggers second grow */
    EXPECT_EQ(dynarray_capacity(da), (size_t)16);

    dynarray_free(da);
}

/* ================================================================== */
/*  ptr_find tests                                                     */
/* ================================================================== */

/*
 * Target present: returned pointer must point to the correct element.
 * Verify by checking both the pointer address and the dereferenced value.
 */
static void test_PtrFind_ReturnsPointerToMatch(void)
{
    int arr[] = { 10, 20, 30, 40, 50 };
    int *p = ptr_find(arr, 5, 30);
    EXPECT_TRUE(p != NULL);
    if (p != NULL) {
        EXPECT_TRUE(p == &arr[2]);
        EXPECT_EQ(*p, 30);
    }
}

/*
 * Target absent: must return NULL.
 */
static void test_PtrFind_ReturnsNullWhenNotFound(void)
{
    int arr[] = { 1, 2, 3 };
    EXPECT_TRUE(ptr_find(arr, 3, 99) == NULL);
}

/*
 * Duplicate values: must return the FIRST occurrence (lowest index).
 */
static void test_PtrFind_ReturnsFirstOccurrence(void)
{
    int arr[] = { 5, 9, 5, 5 };
    int *p = ptr_find(arr, 4, 5);
    EXPECT_TRUE(p == &arr[0]);
}

/*
 * n = 0: empty range, must return NULL without crashing.
 */
static void test_PtrFind_EmptyArray(void)
{
    int dummy = 7;
    EXPECT_TRUE(ptr_find(&dummy, 0, 7) == NULL);
}

/* ================================================================== */
/*  ptr_reverse tests                                                  */
/* ================================================================== */

/*
 * n = 0: must not crash.
 */
static void test_PtrReverse_EmptyArray(void)
{
    int dummy = 1;
    ptr_reverse(&dummy, 0);
    EXPECT_EQ(dummy, 1);
}

/*
 * n = 1: single element is unchanged.
 */
static void test_PtrReverse_SingleElement(void)
{
    int arr[] = { 42 };
    ptr_reverse(arr, 1);
    EXPECT_EQ(arr[0], 42);
}

/*
 * Even length: elements swap pairwise.
 */
static void test_PtrReverse_EvenLength(void)
{
    int arr[]      = { 1, 2, 3, 4 };
    int expected[] = { 4, 3, 2, 1 };
    ptr_reverse(arr, 4);
    EXPECT_INT_ARRAY_EQ(arr, expected, 4);
}

/*
 * Odd length: middle element stays, outer pairs swap.
 */
static void test_PtrReverse_OddLength(void)
{
    int arr[]      = { 1, 2, 3, 4, 5 };
    int expected[] = { 5, 4, 3, 2, 1 };
    ptr_reverse(arr, 5);
    EXPECT_INT_ARRAY_EQ(arr, expected, 5);
}

/* ================================================================== */
/*  ptr_distance tests                                                 */
/* ================================================================== */

/*
 * Same pointer: distance is 0.
 */
static void test_PtrDistance_SamePointer(void)
{
    int arr[] = { 1, 2, 3 };
    EXPECT_EQ(ptr_distance(arr, arr), (ptrdiff_t)0);
}

/*
 * Adjacent elements: distance is 1 (one int step).
 */
static void test_PtrDistance_AdjacentElements(void)
{
    int arr[] = { 1, 2, 3 };
    EXPECT_EQ(ptr_distance(&arr[0], &arr[1]), (ptrdiff_t)1);
}

/*
 * Span of the full array: distance equals n - 1.
 */
static void test_PtrDistance_FullArray(void)
{
    int arr[] = { 10, 20, 30, 40, 50 };
    EXPECT_EQ(ptr_distance(&arr[0], &arr[4]), (ptrdiff_t)4);
}

/*
 * Negative direction: from > to gives a negative result.
 */
static void test_PtrDistance_NegativeDirection(void)
{
    int arr[] = { 1, 2, 3 };
    EXPECT_EQ(ptr_distance(&arr[2], &arr[0]), (ptrdiff_t)-2);
}

/* ================================================================== */
/*  int_arr_dup tests                                                  */
/* ================================================================== */

/*
 * Returned pointer must be different from the original (heap copy).
 */
static void test_IntArrDup_ReturnsNewPointer(void)
{
    int arr[] = { 1, 2, 3 };
    int *copy = int_arr_dup(arr, 3);
    EXPECT_TRUE(copy != NULL);
    EXPECT_TRUE(copy != arr);   /* different allocation */
    free(copy);
}

/*
 * All values in the copy must match the original.
 */
static void test_IntArrDup_CopiesValues(void)
{
    int arr[]      = { 10, 20, 30, 40 };
    int expected[] = { 10, 20, 30, 40 };
    int *copy = int_arr_dup(arr, 4);
    EXPECT_TRUE(copy != NULL);
    if (copy != NULL) {
        EXPECT_INT_ARRAY_EQ(copy, expected, 4);
        free(copy);
    }
}

/*
 * Mutating the copy must not affect the original.
 * This confirms the copy is truly independent (no aliasing).
 */
static void test_IntArrDup_IsIndependent(void)
{
    int arr[] = { 1, 2, 3 };
    int *copy = int_arr_dup(arr, 3);
    EXPECT_TRUE(copy != NULL);
    if (copy != NULL) {
        copy[0] = 999;
        EXPECT_EQ(arr[0], 1);   /* original unchanged */
        free(copy);
    }
}

/*
 * n = 0: must return NULL (nothing to copy).
 */
static void test_IntArrDup_EmptyReturnsNull(void)
{
    int arr[] = { 1, 2, 3 };
    EXPECT_TRUE(int_arr_dup(arr, 0) == NULL);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M01 — Memory & Pointers\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("DynArray\n");
    printf("--------\n");
    RUN_TEST(test_DynArray_CreateIsEmpty);
    RUN_TEST(test_DynArray_PushOneElement);
    RUN_TEST(test_DynArray_PreservesInsertionOrder);
    RUN_TEST(test_DynArray_GrowsBeyondInitialCapacity);
    RUN_TEST(test_DynArray_CapacityDoublesOnGrowth);

    printf("\nptr_find\n");
    printf("--------\n");
    RUN_TEST(test_PtrFind_ReturnsPointerToMatch);
    RUN_TEST(test_PtrFind_ReturnsNullWhenNotFound);
    RUN_TEST(test_PtrFind_ReturnsFirstOccurrence);
    RUN_TEST(test_PtrFind_EmptyArray);

    printf("\nptr_reverse\n");
    printf("-----------\n");
    RUN_TEST(test_PtrReverse_EmptyArray);
    RUN_TEST(test_PtrReverse_SingleElement);
    RUN_TEST(test_PtrReverse_EvenLength);
    RUN_TEST(test_PtrReverse_OddLength);

    printf("\nptr_distance\n");
    printf("------------\n");
    RUN_TEST(test_PtrDistance_SamePointer);
    RUN_TEST(test_PtrDistance_AdjacentElements);
    RUN_TEST(test_PtrDistance_FullArray);
    RUN_TEST(test_PtrDistance_NegativeDirection);

    printf("\nint_arr_dup\n");
    printf("-----------\n");
    RUN_TEST(test_IntArrDup_ReturnsNewPointer);
    RUN_TEST(test_IntArrDup_CopiesValues);
    RUN_TEST(test_IntArrDup_IsIndependent);
    RUN_TEST(test_IntArrDup_EmptyReturnsNull);

    return TEST_SUMMARY();
}
