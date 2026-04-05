#include <string.h>
#include "ds_helpers.h"

/* ── integer key ─────────────────────────────────────────────────── */

int ds_cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

size_t ds_hash_int(const void *data) {
    unsigned int v = (unsigned int)*(const int *)data;
    /* Knuth multiplicative hash */
    return (size_t)(v * 2654435761u);
}

/* ── string key (null-terminated) ────────────────────────────────── */

int ds_cmp_str(const void *a, const void *b) {
    const char *sa = *(const char *const *)a;
    const char *sb = *(const char *const *)b;
    return strcmp(sa, sb);
}

size_t ds_hash_str(const void *data) {
    const char *s = *(const char *const *)data;
    size_t h = 0;
    while (*s)
        h = h * 31u + (unsigned char)(*s++);
    return h;
}

/* ── double key ──────────────────────────────────────────────────── */

int ds_cmp_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}
