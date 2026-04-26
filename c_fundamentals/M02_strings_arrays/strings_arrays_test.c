/*
 * strings_arrays_test.c — Unit tests for Module 2: Strings & Arrays.
 * TDD RED PHASE — implement strings_arrays.c until all tests pass.
 */

#include "strings_arrays.h"
#include "../test_framework.h"
#include <stdlib.h>
#include <string.h>

/* ================================================================== */
/*  str_len                                                            */
/* ================================================================== */

static void test_StrLen_EmptyString(void)
{
    EXPECT_EQ(str_len(""), (size_t)0);
}

static void test_StrLen_SingleChar(void)
{
    EXPECT_EQ(str_len("A"), (size_t)1);
}

static void test_StrLen_LongerString(void)
{
    EXPECT_EQ(str_len("hello"), (size_t)5);
}

/* ================================================================== */
/*  str_ncopy                                                          */
/* ================================================================== */

static void test_StrNcopy_FitsExactly(void)
{
    char buf[6];
    size_t written = str_ncopy(buf, sizeof(buf), "hello");
    EXPECT_EQ(written, (size_t)5);
    EXPECT_TRUE(strcmp(buf, "hello") == 0);
}

static void test_StrNcopy_Truncates(void)
{
    char buf[4];
    str_ncopy(buf, sizeof(buf), "hello");
    /* Must be null-terminated and contain the first 3 chars. */
    EXPECT_EQ(buf[3], '\0');
    EXPECT_TRUE(strncmp(buf, "hel", 3) == 0);
}

static void test_StrNcopy_EmptyDst(void)
{
    char buf[1] = { 'X' };
    str_ncopy(buf, 1, "hello");
    EXPECT_EQ(buf[0], '\0');   /* always null-terminates */
}

/* ================================================================== */
/*  str_ncat                                                           */
/* ================================================================== */

static void test_StrNcat_AppendsToEmpty(void)
{
    char buf[16] = "";
    size_t len = str_ncat(buf, sizeof(buf), "world");
    EXPECT_EQ(len, (size_t)5);
    EXPECT_TRUE(strcmp(buf, "world") == 0);
}

static void test_StrNcat_AppendsToExisting(void)
{
    char buf[16] = "hello ";
    size_t len = str_ncat(buf, sizeof(buf), "world");
    EXPECT_EQ(len, (size_t)11);
    EXPECT_TRUE(strcmp(buf, "hello world") == 0);
}

static void test_StrNcat_TruncatesWhenFull(void)
{
    char buf[8] = "hello";   /* 5 chars + '\0', 2 bytes spare */
    str_ncat(buf, sizeof(buf), "world");
    EXPECT_EQ(buf[7], '\0');  /* always null-terminated */
}

/* ================================================================== */
/*  str_find                                                           */
/* ================================================================== */

static void test_StrFind_CharPresent(void)
{
    const char *s = "hello";
    char *p = str_find(s, 'l');
    EXPECT_TRUE(p != NULL);
    if (p != NULL) {
        EXPECT_EQ(*p, 'l');
        EXPECT_TRUE(p == s + 2);
    }
}

static void test_StrFind_CharAbsent(void)
{
    EXPECT_TRUE(str_find("hello", 'z') == NULL);
}

static void test_StrFind_FindsNullTerminator(void)
{
    const char *s = "hi";
    char *p = str_find(s, '\0');
    EXPECT_TRUE(p != NULL);
    EXPECT_TRUE(p == s + 2);
}

/* ================================================================== */
/*  str_rev                                                            */
/* ================================================================== */

static void test_StrRev_EmptyString(void)
{
    char s[] = "";
    str_rev(s);
    EXPECT_TRUE(strcmp(s, "") == 0);
}

static void test_StrRev_SingleChar(void)
{
    char s[] = "a";
    str_rev(s);
    EXPECT_TRUE(strcmp(s, "a") == 0);
}

static void test_StrRev_EvenLength(void)
{
    char s[] = "abcd";
    str_rev(s);
    EXPECT_TRUE(strcmp(s, "dcba") == 0);
}

static void test_StrRev_OddLength(void)
{
    char s[] = "hello";
    str_rev(s);
    EXPECT_TRUE(strcmp(s, "olleh") == 0);
}

/* ================================================================== */
/*  str_is_palindrome                                                  */
/* ================================================================== */

static void test_StrIsPalindrome_EmptyIsTrue(void)
{
    EXPECT_TRUE(str_is_palindrome(""));
}

static void test_StrIsPalindrome_SingleIsTrue(void)
{
    EXPECT_TRUE(str_is_palindrome("a"));
}

static void test_StrIsPalindrome_PalindromeOdd(void)
{
    EXPECT_TRUE(str_is_palindrome("racecar"));
}

static void test_StrIsPalindrome_PalindromeEven(void)
{
    EXPECT_TRUE(str_is_palindrome("abba"));
}

static void test_StrIsPalindrome_NotPalindrome(void)
{
    EXPECT_FALSE(str_is_palindrome("hello"));
}

/* ================================================================== */
/*  matrix                                                             */
/* ================================================================== */

static void test_Matrix_CreateAndGet(void)
{
    int **m = matrix_create(3, 4);
    EXPECT_TRUE(m != NULL);
    /* All cells must be zero-initialised. */
    int all_zero = 1;
    for (size_t r = 0; r < 3; r++)
        for (size_t c = 0; c < 4; c++)
            if (matrix_get(m, r, c) != 0) { all_zero = 0; break; }
    EXPECT_TRUE(all_zero);
    matrix_free(m, 3);
}

static void test_Matrix_SetAndGet(void)
{
    int **m = matrix_create(3, 3);
    EXPECT_TRUE(m != NULL);
    matrix_set(m, 1, 2, 42);
    EXPECT_EQ(matrix_get(m, 1, 2), 42);
    EXPECT_EQ(matrix_get(m, 0, 0), 0);   /* neighbours untouched */
    matrix_free(m, 3);
}

static void test_Matrix_IndependentRows(void)
{
    /* Writing to one row must not affect another. */
    int **m = matrix_create(2, 4);
    EXPECT_TRUE(m != NULL);
    for (size_t c = 0; c < 4; c++) matrix_set(m, 0, c, (int)c);
    for (size_t c = 0; c < 4; c++) matrix_set(m, 1, c, (int)(c + 10));
    EXPECT_EQ(matrix_get(m, 0, 0), 0);
    EXPECT_EQ(matrix_get(m, 1, 0), 10);
    matrix_free(m, 2);
}

/* ================================================================== */
/*  strarray                                                           */
/* ================================================================== */

static void test_StrArray_CreateCopiesValues(void)
{
    const char *src[] = { "alpha", "beta", "gamma" };
    char **arr = strarray_create(src, 3);
    EXPECT_TRUE(arr != NULL);
    if (arr != NULL) {
        EXPECT_TRUE(strcmp(arr[0], "alpha") == 0);
        EXPECT_TRUE(strcmp(arr[1], "beta")  == 0);
        EXPECT_TRUE(strcmp(arr[2], "gamma") == 0);
        strarray_free(arr, 3);
    }
}

static void test_StrArray_IsIndependent(void)
{
    char orig0[] = "hello";
    char orig1[] = "world";
    const char *src[] = { orig0, orig1 };
    char **arr = strarray_create(src, 2);
    EXPECT_TRUE(arr != NULL);
    if (arr != NULL) {
        orig0[0] = 'X';
        EXPECT_TRUE(strcmp(arr[0], "hello") == 0);
        strarray_free(arr, 2);
    }
}

static void test_StrArray_EmptyReturnsNull(void)
{
    const char *src[] = { "x" };
    EXPECT_TRUE(strarray_create(src, 0) == NULL);
}

/* ================================================================== */
/*  Main                                                               */
/* ================================================================== */

int main(void)
{
    printf("C Fundamentals  M02 — Strings & Arrays\n");
    printf("TDD Red Phase: non-trivial tests are expected to FAIL\n");
    printf("========================================\n\n");

    printf("str_len\n");
    printf("-------\n");
    RUN_TEST(test_StrLen_EmptyString);
    RUN_TEST(test_StrLen_SingleChar);
    RUN_TEST(test_StrLen_LongerString);

    printf("\nstr_ncopy\n");
    printf("---------\n");
    RUN_TEST(test_StrNcopy_FitsExactly);
    RUN_TEST(test_StrNcopy_Truncates);
    RUN_TEST(test_StrNcopy_EmptyDst);

    printf("\nstr_ncat\n");
    printf("--------\n");
    RUN_TEST(test_StrNcat_AppendsToEmpty);
    RUN_TEST(test_StrNcat_AppendsToExisting);
    RUN_TEST(test_StrNcat_TruncatesWhenFull);

    printf("\nstr_find\n");
    printf("--------\n");
    RUN_TEST(test_StrFind_CharPresent);
    RUN_TEST(test_StrFind_CharAbsent);
    RUN_TEST(test_StrFind_FindsNullTerminator);

    printf("\nstr_rev\n");
    printf("-------\n");
    RUN_TEST(test_StrRev_EmptyString);
    RUN_TEST(test_StrRev_SingleChar);
    RUN_TEST(test_StrRev_EvenLength);
    RUN_TEST(test_StrRev_OddLength);

    printf("\nstr_is_palindrome\n");
    printf("-----------------\n");
    RUN_TEST(test_StrIsPalindrome_EmptyIsTrue);
    RUN_TEST(test_StrIsPalindrome_SingleIsTrue);
    RUN_TEST(test_StrIsPalindrome_PalindromeOdd);
    RUN_TEST(test_StrIsPalindrome_PalindromeEven);
    RUN_TEST(test_StrIsPalindrome_NotPalindrome);

    printf("\nmatrix\n");
    printf("------\n");
    RUN_TEST(test_Matrix_CreateAndGet);
    RUN_TEST(test_Matrix_SetAndGet);
    RUN_TEST(test_Matrix_IndependentRows);

    printf("\nstrarray\n");
    printf("--------\n");
    RUN_TEST(test_StrArray_CreateCopiesValues);
    RUN_TEST(test_StrArray_IsIndependent);
    RUN_TEST(test_StrArray_EmptyReturnsNull);

    return TEST_SUMMARY();
}
