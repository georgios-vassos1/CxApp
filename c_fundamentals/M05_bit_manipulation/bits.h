/*
 * bits.h — Module 5: Bit Manipulation (C Fundamentals).
 *
 * Essential prerequisite for ADS lectures 11 and 12 (van Emde Boas
 * trees and Fusion trees), which rely heavily on word-level operations.
 *
 * Groups:
 *   1. Single-bit operations  — set, clear, toggle, get.
 *   2. Aggregate operations   — popcount, power-of-two tests, rounding.
 *   3. Bit-field extraction   — isolate an arbitrary range of bits.
 *   4. Flags register pattern — compact boolean fields in a uint8_t.
 *
 * All functions operate on unsigned integers to avoid undefined
 * behaviour from shifting into or out of the sign bit.
 */

#ifndef C_FUND_BITS_H
#define C_FUND_BITS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>   /* uint8_t, uint32_t */

/* ------------------------------------------------------------------ */
/*  1. Single-bit operations  (pos = 0 is the least-significant bit)  */
/* ------------------------------------------------------------------ */

/* Return x with bit pos set to 1. */
uint32_t bit_set(uint32_t x, int pos);

/* Return x with bit pos set to 0. */
uint32_t bit_clear(uint32_t x, int pos);

/* Return x with bit pos flipped. */
uint32_t bit_toggle(uint32_t x, int pos);

/* Return the value of bit pos in x (0 or 1). */
int bit_get(uint32_t x, int pos);

/* ------------------------------------------------------------------ */
/*  2. Aggregate operations                                            */
/* ------------------------------------------------------------------ */

/* Return the number of set bits in x (Hamming weight / popcount).
 * Implement without __builtin_popcount. */
int bit_count(uint32_t x);

/* Return 1 if x > 0 and x is an exact power of two, 0 otherwise.
 * Implement with a single bitwise expression — no loop. */
int bit_is_pow2(uint32_t x);

/*
 * Return the smallest power of two >= x.
 * Special cases: bit_next_pow2(0) == 1,
 *                bit_next_pow2(x) == x if x is already a power of two.
 */
uint32_t bit_next_pow2(uint32_t x);

/* ------------------------------------------------------------------ */
/*  3. Bit-field extraction                                            */
/* ------------------------------------------------------------------ */

/*
 * Extract len bits starting at bit position start (0 = LSB) and
 * return them right-aligned in a uint32_t.
 *
 * Example: bit_extract(0b11010110, start=2, len=4) == 0b0101  (== 5)
 *
 * Preconditions: 1 <= len <= 32, start + len <= 32.
 */
uint32_t bit_extract(uint32_t x, int start, int len);

/* ------------------------------------------------------------------ */
/*  4. Flags register pattern                                          */
/* ------------------------------------------------------------------ */

/* Typical Unix-style permission flags packed into a byte. */
#define FLAG_READ    ((uint8_t)(1u << 0))
#define FLAG_WRITE   ((uint8_t)(1u << 1))
#define FLAG_EXEC    ((uint8_t)(1u << 2))
#define FLAG_HIDDEN  ((uint8_t)(1u << 3))

/* Return 1 if ALL bits set in mask are also set in field. */
int flags_all_set(uint8_t field, uint8_t mask);

/* Return 1 if ANY bit set in mask is also set in field. */
int flags_any_set(uint8_t field, uint8_t mask);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_BITS_H */
