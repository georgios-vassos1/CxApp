#ifndef DS_HELPERS_H
#define DS_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*
 * Pre-built callbacks for the two most common key types.
 *
 * All callbacks assume **pointer-to-value** semantics: the void *data
 * stored in a container node points to a heap-allocated int, double,
 * or (char *) (i.e. a pointer to a string pointer).
 *
 * This is the natural pattern when wrapping from Python/R where you
 * malloc a single value and hand it to the container.
 */

/* ── integer key ─────────────────────────────────────────────────── */

int    ds_cmp_int(const void *a, const void *b);
size_t ds_hash_int(const void *data);

/* ── string key (null-terminated) ────────────────────────────────── */

int    ds_cmp_str(const void *a, const void *b);
size_t ds_hash_str(const void *data);

/* ── double key ──────────────────────────────────────────────────── */

int    ds_cmp_double(const void *a, const void *b);

#ifdef __cplusplus
}
#endif

#endif /* DS_HELPERS_H */
