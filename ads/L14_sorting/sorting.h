/*
 * sorting.h — Linear-time sorting algorithms (MIT 6.851, Lecture 14).
 *
 * Why linear time is achievable
 * --------------------------------
 * The Ω(n log n) lower bound applies only to comparison-based sorts:
 * any algorithm that distinguishes elements solely via pairwise
 * comparisons must make at least ⌈log₂(n!)⌉ ≈ n log n comparisons.
 *
 * When keys are integers drawn from a bounded universe [0, U), we can
 * exploit their binary structure rather than comparing them.  The two
 * algorithms below do exactly that:
 *
 *   counting_sort   O(n + k)     One pass per key range [0, k].
 *   radix_sort      O(4n)≡O(n)  Four passes of counting sort, one per
 *                               byte of a 32-bit key (base-256 LSD).
 *
 * Both algorithms are STABLE: equal keys preserve their original
 * relative order.  Stability is essential for radix_sort's correctness:
 * each pass builds on the order established by the previous one.
 *
 * Stability proof sketch (counting_sort)
 * ----------------------------------------
 * We scan the input right-to-left and place each element at the
 * position indicated by a suffix-sum of the count array.  Because we
 * decrement the position counter after each placement, earlier
 * occurrences of a key land at lower indices than later ones —
 * preserving original order.
 */

#ifndef ADS_SORTING_H
#define ADS_SORTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* ------------------------------------------------------------------ */
/*  counting_sort                                                      */
/* ------------------------------------------------------------------ */

/*
 * Sort arr[0..n-1] in non-decreasing order in-place.
 *
 * Preconditions
 *   - arr != NULL when n > 0.
 *   - 0 <= arr[i] <= max_val for all i.
 *   - max_val >= 0.
 *
 * Complexity
 *   Time:  O(n + max_val)
 *   Space: O(max_val)  auxiliary (count array + output buffer)
 *
 * Stability: yes — equal elements keep their original relative order.
 */
void counting_sort(int *arr, size_t n, int max_val);

/* ------------------------------------------------------------------ */
/*  radix_sort                                                         */
/* ------------------------------------------------------------------ */

/*
 * Sort arr[0..n-1] in non-decreasing order in-place.
 *
 * Uses LSD (least-significant-digit) radix sort with base 256.
 * Performs four passes of counting_sort, one per byte of each 32-bit
 * key, from the least to the most significant byte.
 *
 * Preconditions
 *   - arr != NULL when n > 0.
 *   - Keys are unsigned 32-bit integers (any value is valid).
 *
 * Complexity
 *   Time:  O(4 · (n + 256)) = O(n)
 *   Space: O(n + 256)  auxiliary (output buffer + count array)
 *
 * Stability: yes (each pass is a stable counting sort).
 */
void radix_sort(unsigned int *arr, size_t n);

/* ------------------------------------------------------------------ */
/*  Verification predicates                                            */
/*  Used by tests and benchmarks — not part of the sorting API.       */
/* ------------------------------------------------------------------ */

/* Returns 1 if arr[0..n-1] is non-decreasingly ordered, 0 otherwise. */
int is_sorted_int(const int *arr, size_t n);
int is_sorted_uint(const unsigned int *arr, size_t n);

#ifdef __cplusplus
}
#endif

#endif /* ADS_SORTING_H */
