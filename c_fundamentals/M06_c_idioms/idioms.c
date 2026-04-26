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

/* TODO: if result==NULL return CF_ERR_NULL; if SIZE_MAX-a < b return CF_ERR_OVERFLOW; *result=a+b; return CF_OK */
CfErr safe_add(size_t a, size_t b, size_t *result)
{ (void)a; (void)b; (void)result; return CF_ERR_OVERFLOW; }

/* TODO: if result==NULL return CF_ERR_NULL; if a!=0 && SIZE_MAX/a < b return CF_ERR_OVERFLOW; *result=a*b; return CF_OK */
CfErr safe_mul(size_t a, size_t b, size_t *result)
{ (void)a; (void)b; (void)result; return CF_ERR_OVERFLOW; }

/* TODO: val < lo ? lo : val > hi ? hi : val */
int clamp_int(int val, int lo, int hi) { (void)val; (void)lo; return hi; }

/* TODO: return idx < n */
int in_bounds(size_t idx, size_t n)    { (void)idx; (void)n; return 0; }

/* TODO: if s==NULL||out==NULL return CF_ERR_NULL; skip optional '-'; parse digits accumulating with overflow check; return CF_ERR_FORMAT if no digits or non-digit found */
CfErr str_to_int(const char *s, int *out)
{ (void)s; (void)out; return CF_ERR_FORMAT; }

/* TODO: linear scan keeping running max; return INT_MIN if n==0 */
int arr_max(const int *arr, size_t n)
{ (void)arr; (void)n; return INT_MIN; }

/* TODO: return pointer to max element (same scan as arr_max); return NULL if arr==NULL||n==0 */
const int *arr_max_ptr(const int *arr, size_t n)
{ (void)arr; (void)n; return NULL; }
