/*
 * idioms.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "idioms.h"
#include <stdlib.h>
#include <limits.h>

const char *cferr_str(CfErr e)
{
    switch (e) {
        case CF_OK:           return "ok";
        case CF_ERR_NULL:     return "null pointer";
        case CF_ERR_OVERFLOW: return "arithmetic overflow";
        case CF_ERR_BOUNDS:   return "out of bounds";
        case CF_ERR_FORMAT:   return "bad format";
    }
    return "unknown error";
}

CfErr safe_add(size_t a, size_t b, size_t *result)
{ (void)a; (void)b; (void)result; return CF_ERR_OVERFLOW; }

CfErr safe_mul(size_t a, size_t b, size_t *result)
{ (void)a; (void)b; (void)result; return CF_ERR_OVERFLOW; }

int clamp_int(int val, int lo, int hi) { (void)val; (void)lo; return hi; }

int in_bounds(size_t idx, size_t n)    { (void)idx; (void)n; return 0; }

CfErr str_to_int(const char *s, int *out)
{ (void)s; (void)out; return CF_ERR_FORMAT; }

int arr_max(const int *arr, size_t n)
{ (void)arr; (void)n; return INT_MIN; }

const int *arr_max_ptr(const int *arr, size_t n)
{ (void)arr; (void)n; return NULL; }
