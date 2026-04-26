/*
 * bits.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "bits.h"

/* TODO: x | (1u << pos) */
uint32_t bit_set   (uint32_t x, int pos) { (void)x; (void)pos; return 0; }
/* TODO: x & ~(1u << pos) */
uint32_t bit_clear (uint32_t x, int pos) { (void)x; (void)pos; return 0; }
/* TODO: x ^ (1u << pos) */
uint32_t bit_toggle(uint32_t x, int pos) { (void)x; (void)pos; return 0; }
/* TODO: (x >> pos) & 1 */
int      bit_get   (uint32_t x, int pos) { (void)x; (void)pos; return 0; }

/* TODO: Brian Kernighan — while(x) { x &= x-1; count++; } */
int      bit_count    (uint32_t x) { (void)x; return 0; }
/* TODO: x > 0 && (x & (x-1)) == 0 */
int      bit_is_pow2  (uint32_t x) { (void)x; return 0; }
/* TODO: if pow2 return x; else shift x right until 0, counting shifts, return 1<<shifts */
uint32_t bit_next_pow2(uint32_t x) { (void)x; return 0; }

/* TODO: (x >> start) & ((1u << len) - 1) */
uint32_t bit_extract(uint32_t x, int start, int len)
{ (void)x; (void)start; (void)len; return 0; }

/* TODO: (field & mask) == mask */
int flags_all_set(uint8_t field, uint8_t mask) { (void)field; (void)mask; return 0; }
/* TODO: (field & mask) != 0 */
int flags_any_set(uint8_t field, uint8_t mask) { (void)field; (void)mask; return 0; }
