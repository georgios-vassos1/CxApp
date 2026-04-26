/*
 * strings_arrays.h — Module 2: Strings & Arrays (C Fundamentals).
 *
 * Two groups of exercises:
 *
 * 1. Strings — manual re-implementations of common <string.h> operations.
 *    Goal: understand what the standard library actually does under the hood,
 *    practise pointer-as-iterator, and handle the null-terminator correctly.
 *
 * 2. Multi-dimensional structures — 2D arrays and arrays of strings.
 *    Goal: understand that "2D array" in C is usually int**, not int[][],
 *    and practise the alloc-loop-free ownership pattern.
 *
 * None of these functions call the standard <string.h> equivalents.
 * Implement them from first principles using pointer arithmetic.
 */

#ifndef C_FUND_STRINGS_ARRAYS_H
#define C_FUND_STRINGS_ARRAYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t */

/* ------------------------------------------------------------------ */
/*  1. String operations                                               */
/* ------------------------------------------------------------------ */

/*
 * Return the number of characters in s before the null terminator.
 * Equivalent to strlen — implement without calling it.
 */
size_t str_len(const char *s);

/*
 * Copy at most dst_size-1 characters from src into dst,
 * always null-terminating dst (if dst_size > 0).
 * Returns the number of characters written, excluding '\0'.
 * Equivalent to strlcpy.
 */
size_t str_ncopy(char *dst, size_t dst_size, const char *src);

/*
 * Append src to the end of dst, writing at most dst_size bytes total
 * (including the existing content and the null terminator).
 * Always null-terminates.
 * Returns the total length of the resulting string.
 */
size_t str_ncat(char *dst, size_t dst_size, const char *src);

/*
 * Return a pointer to the first occurrence of character c in string s.
 * Return NULL if c is not found.
 * Equivalent to strchr — implement without calling it.
 */
char *str_find(const char *s, char c);

/*
 * Reverse string s in place.
 * The null terminator stays at the end.
 */
void str_rev(char *s);

/*
 * Return 1 if s reads the same forwards and backwards, 0 otherwise.
 * Case-sensitive. Empty string and single character are palindromes.
 */
int str_is_palindrome(const char *s);

/* ------------------------------------------------------------------ */
/*  2a. 2D matrix (array of pointers)                                 */
/* ------------------------------------------------------------------ */

/*
 * Allocate a rows × cols matrix of ints, all initialised to 0.
 * Returns an int** (array of `rows` pointers, each pointing to a
 * heap-allocated array of `cols` ints).
 * Returns NULL on allocation failure.
 * Caller must free with matrix_free(m, rows).
 */
int **matrix_create(size_t rows, size_t cols);

/* Set m[row][col] = val. */
void matrix_set(int **m, size_t row, size_t col, int val);

/* Return m[row][col]. */
int  matrix_get(int **m, size_t row, size_t col);

/*
 * Free every row, then the array of row pointers.
 * Must be called with the same `rows` passed to matrix_create.
 */
void matrix_free(int **m, size_t rows);

/* ------------------------------------------------------------------ */
/*  2b. Array of heap-copied strings                                   */
/* ------------------------------------------------------------------ */

/*
 * Allocate an array of count strings, each a heap copy of src[i].
 * Returns a char** pointing to `count` independent copies.
 * Returns NULL if count == 0 or on allocation failure.
 * Caller must free with strarray_free(arr, count).
 */
char **strarray_create(const char * const *src, size_t count);

/*
 * Free each string in arr[0..count-1], then free arr itself.
 */
void strarray_free(char **arr, size_t count);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_STRINGS_ARRAYS_H */
