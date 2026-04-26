/*
 * idioms.h — Module 6: C Idioms (C Fundamentals).
 *
 * Covers the patterns that separate fluent C from beginner C:
 *
 *   1. Error codes — returning structured failure information instead
 *      of using errno or magic sentinel values.
 *
 *   2. Overflow-safe arithmetic — size_t arithmetic wraps silently;
 *      detecting overflow before it happens is essential in any code
 *      that allocates memory based on user input.
 *
 *   3. Bounds & clamping — common input-validation patterns.
 *
 *   4. Safe string parsing — converting strings to numbers without
 *      invoking undefined behaviour on bad input.
 *
 *   5. const correctness — using const to document ownership and
 *      prevent accidental mutation; returning const pointers.
 *
 * The restrict keyword is covered in the module exercises (see .c stub)
 * but not in the API here, as it is a hint to the compiler rather than
 * an observable behaviour change.
 */

#ifndef C_FUND_IDIOMS_H
#define C_FUND_IDIOMS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t   */
#include <stdint.h>   /* SIZE_MAX */
#include <limits.h>   /* INT_MIN, INT_MAX */

/* ------------------------------------------------------------------ */
/*  1. Error codes                                                     */
/* ------------------------------------------------------------------ */

typedef enum {
    CF_OK = 0,       /* success                        */
    CF_ERR_NULL,     /* a required pointer was NULL    */
    CF_ERR_OVERFLOW, /* arithmetic would overflow      */
    CF_ERR_BOUNDS,   /* index or value out of range    */
    CF_ERR_FORMAT    /* input string has wrong format  */
} CfErr;

/* Return a human-readable string for error code e. Never returns NULL. */
const char *cferr_str(CfErr e);

/* ------------------------------------------------------------------ */
/*  2. Overflow-safe arithmetic                                        */
/* ------------------------------------------------------------------ */

/*
 * Set *result = a + b.
 * Return CF_ERR_OVERFLOW if the addition would exceed SIZE_MAX.
 * Return CF_ERR_NULL if result is NULL.
 */
CfErr safe_add(size_t a, size_t b, size_t *result);

/*
 * Set *result = a * b.
 * Return CF_ERR_OVERFLOW if the multiplication would exceed SIZE_MAX.
 * Return CF_ERR_NULL if result is NULL.
 */
CfErr safe_mul(size_t a, size_t b, size_t *result);

/* ------------------------------------------------------------------ */
/*  3. Bounds & clamping                                               */
/* ------------------------------------------------------------------ */

/* Return val clamped to [lo, hi].  Requires lo <= hi. */
int clamp_int(int val, int lo, int hi);

/*
 * Return 1 if idx is a valid 0-based index into an array of length n
 * (i.e. idx < n), 0 otherwise.
 */
int in_bounds(size_t idx, size_t n);

/* ------------------------------------------------------------------ */
/*  4. Safe string-to-integer conversion                              */
/* ------------------------------------------------------------------ */

/*
 * Parse the null-terminated string s as a base-10 signed integer
 * and store the result in *out.
 *
 * Returns:
 *   CF_OK          — success; *out is set.
 *   CF_ERR_NULL    — s or out is NULL.
 *   CF_ERR_FORMAT  — s is empty or contains non-digit characters
 *                    (a leading '-' is allowed for negative numbers).
 *   CF_ERR_OVERFLOW — value does not fit in int.
 */
CfErr str_to_int(const char *s, int *out);

/* ------------------------------------------------------------------ */
/*  5. const correctness                                               */
/* ------------------------------------------------------------------ */

/*
 * Return the maximum value in arr[0..n-1].
 * Return INT_MIN if n == 0.
 * arr is read-only — const prevents accidental mutation.
 */
int arr_max(const int *arr, size_t n);

/*
 * Return a pointer to the maximum element in arr[0..n-1].
 * The returned pointer is const — callers may read but not write via it.
 * Return NULL if n == 0.
 */
const int *arr_max_ptr(const int *arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_IDIOMS_H */
