/*
 * funcptrs_test.c — Unit tests for Module 4: Function Pointers.
 * TDD RED PHASE — implement funcptrs.c until all tests pass.
 */

#include "funcptrs.h"
#include "../test_framework.h"
#include <math.h>    /* fabs, M_PI */
#include <string.h>  /* strcmp */

#define EPSILON 1e-9
#define EXPECT_DOUBLE_NEAR(a, b) EXPECT_TRUE(fabs((a) - (b)) < 1e-6)

/* ================================================================== */
/*  Helpers — simple functions to pass as callbacks                   */
/* ================================================================== */

static int double_it(int x) { return x * 2; }
static int negate(int x)    { return -x; }
static int is_even(int x)   { return x % 2 == 0; }
static int is_positive(int x){ return x > 0; }
static int add(int a, int b) { return a + b; }
static int mul(int a, int b) { return a * b; }
static int max2(int a, int b){ return a > b ? a : b; }
static int cmp_asc(int a, int b) { return (a > b) - (a < b); }
static int cmp_desc(int a, int b){ return (a < b) - (a > b); }

/* ================================================================== */
/*  arr_map                                                            */
/* ================================================================== */

static void test_ArrMap_DoubleAllElements(void)
{
    int arr[]      = { 1, 2, 3, 4 };
    int expected[] = { 2, 4, 6, 8 };
    arr_map(arr, 4, double_it);
    EXPECT_INT_ARRAY_EQ(arr, expected, 4);
}

static void test_ArrMap_NegateAllElements(void)
{
    int arr[]      = { 1, -2, 3 };
    int expected[] = { -1, 2, -3 };
    arr_map(arr, 3, negate);
    EXPECT_INT_ARRAY_EQ(arr, expected, 3);
}

static void test_ArrMap_EmptyArray(void)
{
    int dummy = 5;
    arr_map(&dummy, 0, double_it);
    EXPECT_EQ(dummy, 5);   /* untouched */
}

/* ================================================================== */
/*  arr_count_if                                                       */
/* ================================================================== */

static void test_ArrCountIf_CountEvens(void)
{
    int arr[] = { 1, 2, 3, 4, 5, 6 };
    EXPECT_EQ(arr_count_if(arr, 6, is_even), (size_t)3);
}

static void test_ArrCountIf_NoneMatch(void)
{
    int arr[] = { 1, 3, 5 };
    EXPECT_EQ(arr_count_if(arr, 3, is_even), (size_t)0);
}

static void test_ArrCountIf_AllMatch(void)
{
    int arr[] = { 2, 4, 6 };
    EXPECT_EQ(arr_count_if(arr, 3, is_even), (size_t)3);
}

static void test_ArrCountIf_CountPositives(void)
{
    int arr[] = { -3, -1, 0, 2, 4 };
    EXPECT_EQ(arr_count_if(arr, 5, is_positive), (size_t)2);
}

/* ================================================================== */
/*  arr_reduce                                                         */
/* ================================================================== */

static void test_ArrReduce_Sum(void)
{
    int arr[] = { 1, 2, 3, 4, 5 };
    /* 0+1+2+3+4+5 = 15 */
    EXPECT_EQ(arr_reduce(arr, 5, 0, add), 15);
}

static void test_ArrReduce_Product(void)
{
    int arr[] = { 1, 2, 3, 4 };
    /* 1*1*2*3*4 = 24 */
    EXPECT_EQ(arr_reduce(arr, 4, 1, mul), 24);
}

static void test_ArrReduce_Max(void)
{
    int arr[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
    EXPECT_EQ(arr_reduce(arr, 8, arr[0], max2), 9);
}

static void test_ArrReduce_EmptyReturnsInit(void)
{
    int dummy = 0;
    EXPECT_EQ(arr_reduce(&dummy, 0, 42, add), 42);
}

/* ================================================================== */
/*  arr_sort                                                           */
/* ================================================================== */

static void test_ArrSort_Ascending(void)
{
    int arr[]      = { 5, 2, 8, 1, 9 };
    int expected[] = { 1, 2, 5, 8, 9 };
    arr_sort(arr, 5, cmp_asc);
    EXPECT_INT_ARRAY_EQ(arr, expected, 5);
}

static void test_ArrSort_Descending(void)
{
    int arr[]      = { 5, 2, 8, 1, 9 };
    int expected[] = { 9, 8, 5, 2, 1 };
    arr_sort(arr, 5, cmp_desc);
    EXPECT_INT_ARRAY_EQ(arr, expected, 5);
}

/* ================================================================== */
/*  arr_bsearch                                                        */
/* ================================================================== */

static void test_ArrBsearch_Found(void)
{
    int arr[] = { 1, 3, 5, 7, 9 };
    EXPECT_EQ(arr_bsearch(arr, 5, 5), 2);
}

static void test_ArrBsearch_NotFound(void)
{
    int arr[] = { 1, 3, 5, 7, 9 };
    EXPECT_EQ(arr_bsearch(arr, 5, 4), -1);
}

static void test_ArrBsearch_FirstElement(void)
{
    int arr[] = { 2, 4, 6, 8 };
    EXPECT_EQ(arr_bsearch(arr, 4, 2), 0);
}

static void test_ArrBsearch_LastElement(void)
{
    int arr[] = { 2, 4, 6, 8 };
    EXPECT_EQ(arr_bsearch(arr, 4, 8), 3);
}

/* ================================================================== */
/*  vtable dispatch                                                    */
/* ================================================================== */

static void test_Shape_CircleArea(void)
{
    Circle c;
    circle_init(&c, 1.0);
    /* Area = π × r² = π */
    EXPECT_DOUBLE_NEAR(shape_area((Shape *)&c), M_PI);
}

static void test_Shape_CirclePerimeter(void)
{
    Circle c;
    circle_init(&c, 1.0);
    /* Perimeter = 2πr = 2π */
    EXPECT_DOUBLE_NEAR(shape_perimeter((Shape *)&c), 2.0 * M_PI);
}

static void test_Shape_CircleName(void)
{
    Circle c;
    circle_init(&c, 1.0);
    EXPECT_TRUE(strcmp(shape_name((Shape *)&c), "Circle") == 0);
}

static void test_Shape_RectArea(void)
{
    ShapeRect r;
    shaperect_init(&r, 3.0, 4.0);
    EXPECT_DOUBLE_NEAR(shape_area((Shape *)&r), 12.0);
}

static void test_Shape_RectPerimeter(void)
{
    ShapeRect r;
    shaperect_init(&r, 3.0, 4.0);
    EXPECT_DOUBLE_NEAR(shape_perimeter((Shape *)&r), 14.0);
}

static void test_Shape_RectName(void)
{
    ShapeRect r;
    shaperect_init(&r, 3.0, 4.0);
    EXPECT_TRUE(strcmp(shape_name((Shape *)&r), "Rectangle") == 0);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M04 — Function Pointers\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("arr_map\n");
    printf("-------\n");
    RUN_TEST(test_ArrMap_DoubleAllElements);
    RUN_TEST(test_ArrMap_NegateAllElements);
    RUN_TEST(test_ArrMap_EmptyArray);

    printf("\narr_count_if\n");
    printf("------------\n");
    RUN_TEST(test_ArrCountIf_CountEvens);
    RUN_TEST(test_ArrCountIf_NoneMatch);
    RUN_TEST(test_ArrCountIf_AllMatch);
    RUN_TEST(test_ArrCountIf_CountPositives);

    printf("\narr_reduce\n");
    printf("----------\n");
    RUN_TEST(test_ArrReduce_Sum);
    RUN_TEST(test_ArrReduce_Product);
    RUN_TEST(test_ArrReduce_Max);
    RUN_TEST(test_ArrReduce_EmptyReturnsInit);

    printf("\narr_sort\n");
    printf("--------\n");
    RUN_TEST(test_ArrSort_Ascending);
    RUN_TEST(test_ArrSort_Descending);

    printf("\narr_bsearch\n");
    printf("-----------\n");
    RUN_TEST(test_ArrBsearch_Found);
    RUN_TEST(test_ArrBsearch_NotFound);
    RUN_TEST(test_ArrBsearch_FirstElement);
    RUN_TEST(test_ArrBsearch_LastElement);

    printf("\nShape vtable\n");
    printf("------------\n");
    RUN_TEST(test_Shape_CircleArea);
    RUN_TEST(test_Shape_CirclePerimeter);
    RUN_TEST(test_Shape_CircleName);
    RUN_TEST(test_Shape_RectArea);
    RUN_TEST(test_Shape_RectPerimeter);
    RUN_TEST(test_Shape_RectName);

    return TEST_SUMMARY();
}
