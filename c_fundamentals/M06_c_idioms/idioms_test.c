/*
 * idioms_test.c — Unit tests for Module 6: C Idioms.
 * TDD RED PHASE — implement idioms.c until all tests pass.
 */

#include "idioms.h"
#include "../test_framework.h"
#include <stdint.h>   /* SIZE_MAX */
#include <limits.h>   /* INT_MIN, INT_MAX */

/* ================================================================== */
/*  cferr_str                                                          */
/* ================================================================== */

static void test_CferrStr_NeverReturnsNull(void)
{
    EXPECT_TRUE(cferr_str(CF_OK)           != NULL);
    EXPECT_TRUE(cferr_str(CF_ERR_NULL)     != NULL);
    EXPECT_TRUE(cferr_str(CF_ERR_OVERFLOW) != NULL);
    EXPECT_TRUE(cferr_str(CF_ERR_BOUNDS)   != NULL);
    EXPECT_TRUE(cferr_str(CF_ERR_FORMAT)   != NULL);
}

/* ================================================================== */
/*  safe_add                                                           */
/* ================================================================== */

static void test_SafeAdd_Normal(void)
{
    size_t result = 0;
    EXPECT_EQ(safe_add(3, 4, &result), CF_OK);
    EXPECT_EQ(result, (size_t)7);
}

static void test_SafeAdd_Zero(void)
{
    size_t result = 0;
    EXPECT_EQ(safe_add(0, 0, &result), CF_OK);
    EXPECT_EQ(result, (size_t)0);
}

static void test_SafeAdd_Overflow(void)
{
    size_t result = 0;
    EXPECT_EQ(safe_add(SIZE_MAX, 1, &result), CF_ERR_OVERFLOW);
}

static void test_SafeAdd_NullResult(void)
{
    EXPECT_EQ(safe_add(1, 2, NULL), CF_ERR_NULL);
}

/* ================================================================== */
/*  safe_mul                                                           */
/* ================================================================== */

static void test_SafeMul_Normal(void)
{
    size_t result = 0;
    EXPECT_EQ(safe_mul(6, 7, &result), CF_OK);
    EXPECT_EQ(result, (size_t)42);
}

static void test_SafeMul_ByZero(void)
{
    size_t result = 99;
    EXPECT_EQ(safe_mul(0, SIZE_MAX, &result), CF_OK);
    EXPECT_EQ(result, (size_t)0);
}

static void test_SafeMul_Overflow(void)
{
    size_t result = 0;
    EXPECT_EQ(safe_mul(SIZE_MAX, 2, &result), CF_ERR_OVERFLOW);
}

static void test_SafeMul_NullResult(void)
{
    EXPECT_EQ(safe_mul(2, 3, NULL), CF_ERR_NULL);
}

/* ================================================================== */
/*  clamp_int                                                          */
/* ================================================================== */

static void test_ClampInt_BelowLo(void)   { EXPECT_EQ(clamp_int(-5, 0, 10), 0);  }
static void test_ClampInt_AboveHi(void)   { EXPECT_EQ(clamp_int(20, 0, 10), 10); }
static void test_ClampInt_WithinRange(void){ EXPECT_EQ(clamp_int(5,  0, 10), 5);  }
static void test_ClampInt_AtLo(void)      { EXPECT_EQ(clamp_int(0,  0, 10), 0);  }
static void test_ClampInt_AtHi(void)      { EXPECT_EQ(clamp_int(10, 0, 10), 10); }

/* ================================================================== */
/*  in_bounds                                                          */
/* ================================================================== */

static void test_InBounds_ValidIndex(void)
{
    EXPECT_TRUE(in_bounds(0, 5));
    EXPECT_TRUE(in_bounds(4, 5));
}

static void test_InBounds_EqualToNIsInvalid(void)
{
    EXPECT_FALSE(in_bounds(5, 5));
}

static void test_InBounds_ZeroLengthArray(void)
{
    EXPECT_FALSE(in_bounds(0, 0));
}

/* ================================================================== */
/*  str_to_int                                                         */
/* ================================================================== */

static void test_StrToInt_ValidPositive(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int("42", &out), CF_OK);
    EXPECT_EQ(out, 42);
}

static void test_StrToInt_ValidNegative(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int("-7", &out), CF_OK);
    EXPECT_EQ(out, -7);
}

static void test_StrToInt_Zero(void)
{
    int out = 99;
    EXPECT_EQ(str_to_int("0", &out), CF_OK);
    EXPECT_EQ(out, 0);
}

static void test_StrToInt_NullString(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int(NULL, &out), CF_ERR_NULL);
}

static void test_StrToInt_NullOut(void)
{
    EXPECT_EQ(str_to_int("42", NULL), CF_ERR_NULL);
}

static void test_StrToInt_BadFormat(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int("12x4", &out), CF_ERR_FORMAT);
}

static void test_StrToInt_EmptyString(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int("", &out), CF_ERR_FORMAT);
}

static void test_StrToInt_Overflow(void)
{
    int out = 0;
    EXPECT_EQ(str_to_int("99999999999", &out), CF_ERR_OVERFLOW);
}

/* ================================================================== */
/*  arr_max / arr_max_ptr                                              */
/* ================================================================== */

static void test_ArrMax_BasicCase(void)
{
    int arr[] = { 3, 1, 4, 1, 5, 9, 2, 6 };
    EXPECT_EQ(arr_max(arr, 8), 9);
}

static void test_ArrMax_SingleElement(void)
{
    int arr[] = { 42 };
    EXPECT_EQ(arr_max(arr, 1), 42);
}

static void test_ArrMax_AllNegative(void)
{
    int arr[] = { -5, -3, -8, -1 };
    EXPECT_EQ(arr_max(arr, 4), -1);
}

static void test_ArrMax_EmptyReturnsIntMin(void)
{
    int dummy = 0;
    EXPECT_EQ(arr_max(&dummy, 0), INT_MIN);
}

static void test_ArrMaxPtr_ReturnsPointerToMax(void)
{
    int arr[] = { 3, 9, 1, 5 };
    const int *p = arr_max_ptr(arr, 4);
    EXPECT_TRUE(p != NULL);
    if (p != NULL) {
        EXPECT_TRUE(p == &arr[1]);
        EXPECT_EQ(*p, 9);
    }
}

static void test_ArrMaxPtr_EmptyReturnsNull(void)
{
    int dummy = 0;
    EXPECT_TRUE(arr_max_ptr(&dummy, 0) == NULL);
}

static void test_ArrMaxPtr_IsConstCorrect(void)
{
    /* This test simply verifies that the returned pointer is const-qualified
     * at the type level — enforced by the compiler, not a runtime check.
     * If this compiles without warning, const correctness is maintained. */
    int arr[] = { 1, 2, 3 };
    const int *p = arr_max_ptr(arr, 3);
    EXPECT_TRUE(p != NULL);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M06 — C Idioms\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("cferr_str\n");
    printf("---------\n");
    RUN_TEST(test_CferrStr_NeverReturnsNull);

    printf("\nsafe_add\n");
    printf("--------\n");
    RUN_TEST(test_SafeAdd_Normal);
    RUN_TEST(test_SafeAdd_Zero);
    RUN_TEST(test_SafeAdd_Overflow);
    RUN_TEST(test_SafeAdd_NullResult);

    printf("\nsafe_mul\n");
    printf("--------\n");
    RUN_TEST(test_SafeMul_Normal);
    RUN_TEST(test_SafeMul_ByZero);
    RUN_TEST(test_SafeMul_Overflow);
    RUN_TEST(test_SafeMul_NullResult);

    printf("\nclamp_int\n");
    printf("---------\n");
    RUN_TEST(test_ClampInt_BelowLo);
    RUN_TEST(test_ClampInt_AboveHi);
    RUN_TEST(test_ClampInt_WithinRange);
    RUN_TEST(test_ClampInt_AtLo);
    RUN_TEST(test_ClampInt_AtHi);

    printf("\nin_bounds\n");
    printf("---------\n");
    RUN_TEST(test_InBounds_ValidIndex);
    RUN_TEST(test_InBounds_EqualToNIsInvalid);
    RUN_TEST(test_InBounds_ZeroLengthArray);

    printf("\nstr_to_int\n");
    printf("----------\n");
    RUN_TEST(test_StrToInt_ValidPositive);
    RUN_TEST(test_StrToInt_ValidNegative);
    RUN_TEST(test_StrToInt_Zero);
    RUN_TEST(test_StrToInt_NullString);
    RUN_TEST(test_StrToInt_NullOut);
    RUN_TEST(test_StrToInt_BadFormat);
    RUN_TEST(test_StrToInt_EmptyString);
    RUN_TEST(test_StrToInt_Overflow);

    printf("\narr_max / arr_max_ptr\n");
    printf("---------------------\n");
    RUN_TEST(test_ArrMax_BasicCase);
    RUN_TEST(test_ArrMax_SingleElement);
    RUN_TEST(test_ArrMax_AllNegative);
    RUN_TEST(test_ArrMax_EmptyReturnsIntMin);
    RUN_TEST(test_ArrMaxPtr_ReturnsPointerToMax);
    RUN_TEST(test_ArrMaxPtr_EmptyReturnsNull);
    RUN_TEST(test_ArrMaxPtr_IsConstCorrect);

    return TEST_SUMMARY();
}
