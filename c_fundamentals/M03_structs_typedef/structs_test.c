/*
 * structs_test.c — Unit tests for Module 3: Structs & typedef.
 * TDD RED PHASE — implement structs.c until all tests pass.
 */

#include "structs.h"
#include "../test_framework.h"
#include <math.h>    /* fabs */

#define EPSILON 1e-9
#define EXPECT_DOUBLE_EQ(a, b) EXPECT_TRUE(fabs((a) - (b)) < EPSILON)

/* ================================================================== */
/*  Rect tests                                                         */
/* ================================================================== */

static void test_RectArea_ZeroDimension(void)
{
    Rect r = { .origin = {0,0}, .width = 0.0, .height = 5.0 };
    EXPECT_DOUBLE_EQ(rect_area(r), 0.0);
}

static void test_RectArea_Basic(void)
{
    Rect r = { .origin = {0,0}, .width = 4.0, .height = 3.0 };
    EXPECT_DOUBLE_EQ(rect_area(r), 12.0);
}

static void test_RectPerimeter_Basic(void)
{
    Rect r = { .origin = {0,0}, .width = 4.0, .height = 3.0 };
    EXPECT_DOUBLE_EQ(rect_perimeter(r), 14.0);
}

static void test_RectContains_InsidePoint(void)
{
    Rect r  = { .origin = {1.0, 1.0}, .width = 4.0, .height = 4.0 };
    Point p = { .x = 3.0, .y = 3.0 };
    EXPECT_TRUE(rect_contains(r, p));
}

static void test_RectContains_OutsidePoint(void)
{
    Rect r  = { .origin = {1.0, 1.0}, .width = 4.0, .height = 4.0 };
    Point p = { .x = 0.0, .y = 0.0 };
    EXPECT_FALSE(rect_contains(r, p));
}

static void test_RectContains_OnBoundaryIsOutside(void)
{
    /* Boundary points are NOT strictly inside. */
    Rect r  = { .origin = {0.0, 0.0}, .width = 5.0, .height = 5.0 };
    Point p = { .x = 0.0, .y = 2.5 };   /* on left edge */
    EXPECT_FALSE(rect_contains(r, p));
}

static void test_RectIntersects_Overlapping(void)
{
    Rect a = { .origin = {0,0}, .width = 4.0, .height = 4.0 };
    Rect b = { .origin = {2,2}, .width = 4.0, .height = 4.0 };
    EXPECT_TRUE(rect_intersects(a, b));
}

static void test_RectIntersects_Separate(void)
{
    Rect a = { .origin = {0,0}, .width = 2.0, .height = 2.0 };
    Rect b = { .origin = {5,5}, .width = 2.0, .height = 2.0 };
    EXPECT_FALSE(rect_intersects(a, b));
}

static void test_RectIntersects_TouchingEdgeIsNotOverlap(void)
{
    Rect a = { .origin = {0,0}, .width = 2.0, .height = 2.0 };
    Rect b = { .origin = {2,0}, .width = 2.0, .height = 2.0 };
    EXPECT_FALSE(rect_intersects(a, b));
}

/* ================================================================== */
/*  Queue tests                                                        */
/* ================================================================== */

static void test_Queue_CreateIsEmpty(void)
{
    Queue *q = queue_create();
    EXPECT_TRUE(q != NULL);
    EXPECT_TRUE(queue_is_empty(q));
    EXPECT_EQ(queue_size(q), (size_t)0);
    queue_free(q);
}

static void test_Queue_EnqueueIncreasesSize(void)
{
    Queue *q = queue_create();
    queue_enqueue(q, 10);
    EXPECT_EQ(queue_size(q), (size_t)1);
    EXPECT_FALSE(queue_is_empty(q));
    queue_free(q);
}

static void test_Queue_FIFOOrder(void)
{
    /* First in, first out — dequeue must return elements in insertion order. */
    Queue *q = queue_create();
    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    queue_enqueue(q, 3);

    int out = 0;
    EXPECT_TRUE(queue_dequeue(q, &out)); EXPECT_EQ(out, 1);
    EXPECT_TRUE(queue_dequeue(q, &out)); EXPECT_EQ(out, 2);
    EXPECT_TRUE(queue_dequeue(q, &out)); EXPECT_EQ(out, 3);
    EXPECT_TRUE(queue_is_empty(q));
    queue_free(q);
}

static void test_Queue_DequeueEmptyReturnsFalse(void)
{
    Queue *q = queue_create();
    int out = 99;
    EXPECT_FALSE(queue_dequeue(q, &out));
    EXPECT_EQ(out, 99);   /* out must be untouched */
    queue_free(q);
}

static void test_Queue_PeekDoesNotRemove(void)
{
    Queue *q = queue_create();
    queue_enqueue(q, 42);
    int out = 0;
    EXPECT_TRUE(queue_peek(q, &out));
    EXPECT_EQ(out, 42);
    EXPECT_EQ(queue_size(q), (size_t)1);   /* still there */
    queue_free(q);
}

static void test_Queue_InterleavedEnqueueDequeue(void)
{
    Queue *q = queue_create();
    queue_enqueue(q, 1);
    queue_enqueue(q, 2);
    int out = 0;
    queue_dequeue(q, &out);   /* removes 1 */
    queue_enqueue(q, 3);
    queue_dequeue(q, &out);   EXPECT_EQ(out, 2);
    queue_dequeue(q, &out);   EXPECT_EQ(out, 3);
    EXPECT_TRUE(queue_is_empty(q));
    queue_free(q);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M03 — Structs & typedef\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("Rect\n");
    printf("----\n");
    RUN_TEST(test_RectArea_ZeroDimension);
    RUN_TEST(test_RectArea_Basic);
    RUN_TEST(test_RectPerimeter_Basic);
    RUN_TEST(test_RectContains_InsidePoint);
    RUN_TEST(test_RectContains_OutsidePoint);
    RUN_TEST(test_RectContains_OnBoundaryIsOutside);
    RUN_TEST(test_RectIntersects_Overlapping);
    RUN_TEST(test_RectIntersects_Separate);
    RUN_TEST(test_RectIntersects_TouchingEdgeIsNotOverlap);

    printf("\nQueue\n");
    printf("-----\n");
    RUN_TEST(test_Queue_CreateIsEmpty);
    RUN_TEST(test_Queue_EnqueueIncreasesSize);
    RUN_TEST(test_Queue_FIFOOrder);
    RUN_TEST(test_Queue_DequeueEmptyReturnsFalse);
    RUN_TEST(test_Queue_PeekDoesNotRemove);
    RUN_TEST(test_Queue_InterleavedEnqueueDequeue);

    return TEST_SUMMARY();
}
