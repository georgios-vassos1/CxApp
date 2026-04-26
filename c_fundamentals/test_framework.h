/*
 * test_framework.h — Minimal C11 unit-test harness for the ADS course.
 *
 * Design goals
 * ------------
 *   - Zero external dependencies (pure C11 + stdio).
 *   - One assertion = one pass/fail counter increment.
 *   - Descriptive failure messages with file + line.
 *   - RUN_TEST prints [ RUN ] / [ PASS ] / [ FAIL ] per test function.
 *   - TEST_SUMMARY() returns 0 (all pass) or 1 (any fail) for main().
 *
 * Usage
 * -----
 *   #include "test_framework.h"
 *
 *   static void test_FooBar(void) {
 *       int result = foo_bar(2, 3);
 *       EXPECT_EQ(result, 5);
 *   }
 *
 *   int main(void) {
 *       RUN_TEST(test_FooBar);
 *       return TEST_SUMMARY();
 *   }
 */

#ifndef ADS_TEST_FRAMEWORK_H
#define ADS_TEST_FRAMEWORK_H

#include <stdio.h>
#include <stddef.h>

/* Global pass/fail counters — static so each translation unit is self-contained.
 * Every test binary is a single .c file, so there is no double-definition risk. */
static int _tf_passed = 0;
static int _tf_failed = 0;

/* ------------------------------------------------------------------ */
/*  Assertion macros                                                   */
/* ------------------------------------------------------------------ */

/*
 * EXPECT_TRUE / EXPECT_FALSE
 * Check a boolean condition. Does NOT abort the test on failure;
 * the test function continues so all assertions are evaluated.
 */
#define EXPECT_TRUE(expr)                                                    \
    do {                                                                     \
        if (!(expr)) {                                                       \
            fprintf(stderr, "  FAIL  %s:%d — expected true: %s\n",          \
                    __FILE__, __LINE__, #expr);                              \
            _tf_failed++;                                                    \
        } else {                                                             \
            _tf_passed++;                                                    \
        }                                                                    \
    } while (0)

#define EXPECT_FALSE(expr)                                                   \
    do {                                                                     \
        if ((expr)) {                                                        \
            fprintf(stderr, "  FAIL  %s:%d — expected false: %s\n",         \
                    __FILE__, __LINE__, #expr);                              \
            _tf_failed++;                                                    \
        } else {                                                             \
            _tf_passed++;                                                    \
        }                                                                    \
    } while (0)

/*
 * EXPECT_EQ / EXPECT_NEQ
 * Scalar equality / inequality using the == and != operators.
 */
#define EXPECT_EQ(a, b)                                                      \
    do {                                                                     \
        if ((a) != (b)) {                                                    \
            fprintf(stderr, "  FAIL  %s:%d — expected %s == %s\n",          \
                    __FILE__, __LINE__, #a, #b);                             \
            _tf_failed++;                                                    \
        } else {                                                             \
            _tf_passed++;                                                    \
        }                                                                    \
    } while (0)

#define EXPECT_NEQ(a, b)                                                     \
    do {                                                                     \
        if ((a) == (b)) {                                                    \
            fprintf(stderr, "  FAIL  %s:%d — expected %s != %s\n",          \
                    __FILE__, __LINE__, #a, #b);                             \
            _tf_failed++;                                                    \
        } else {                                                             \
            _tf_passed++;                                                    \
        }                                                                    \
    } while (0)

/*
 * EXPECT_INT_ARRAY_EQ / EXPECT_UINT_ARRAY_EQ
 * Element-wise comparison of two arrays of length n.
 * Reports the index and values of the first mismatch, then stops.
 * Counts as one assertion (one pass/fail increment).
 */
#define EXPECT_INT_ARRAY_EQ(got, want, n)                                    \
    do {                                                                     \
        int _ok = 1;                                                         \
        for (size_t _i = 0; _i < (size_t)(n); _i++) {                       \
            if ((got)[_i] != (want)[_i]) {                                   \
                fprintf(stderr,                                              \
                    "  FAIL  %s:%d — " #got "[%zu] = %d, want %d\n",        \
                    __FILE__, __LINE__, _i, (got)[_i], (want)[_i]);          \
                _ok = 0;                                                     \
                break;                                                       \
            }                                                                \
        }                                                                    \
        if (_ok) _tf_passed++; else _tf_failed++;                           \
    } while (0)

#define EXPECT_UINT_ARRAY_EQ(got, want, n)                                   \
    do {                                                                     \
        int _ok = 1;                                                         \
        for (size_t _i = 0; _i < (size_t)(n); _i++) {                       \
            if ((got)[_i] != (want)[_i]) {                                   \
                fprintf(stderr,                                              \
                    "  FAIL  %s:%d — " #got "[%zu] = %u, want %u\n",        \
                    __FILE__, __LINE__, _i, (got)[_i], (want)[_i]);          \
                _ok = 0;                                                     \
                break;                                                       \
            }                                                                \
        }                                                                    \
        if (_ok) _tf_passed++; else _tf_failed++;                           \
    } while (0)

/* ------------------------------------------------------------------ */
/*  Test runner                                                        */
/* ------------------------------------------------------------------ */

/*
 * RUN_TEST(fn)
 * Calls fn(), then prints [ PASS ] or [ FAIL ] based on whether
 * any assertion inside fn() incremented _tf_failed.
 */
#define RUN_TEST(fn)                                                         \
    do {                                                                     \
        int _before = _tf_failed;                                            \
        printf("[ RUN  ] %s\n", #fn);                                        \
        fn();                                                                \
        if (_tf_failed == _before)                                           \
            printf("[ PASS ] %s\n", #fn);                                    \
        else                                                                 \
            printf("[ FAIL ] %s\n", #fn);                                    \
    } while (0)

/*
 * TEST_SUMMARY()
 * Prints totals and returns 0 (all green) or 1 (any red).
 * Use as the return value of main().
 */
#define TEST_SUMMARY()                                                       \
    (                                                                        \
        printf("\n========================================\n"),               \
        printf("  %d passed,  %d failed\n", _tf_passed, _tf_failed),        \
        printf("========================================\n"),                \
        (_tf_failed > 0) ? 1 : 0                                            \
    )

#endif /* ADS_TEST_FRAMEWORK_H */
