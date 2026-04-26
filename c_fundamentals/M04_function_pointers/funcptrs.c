/*
 * funcptrs.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "funcptrs.h"
#include <stdlib.h>
#include <math.h>    /* M_PI for circle area/perimeter */

/* ------------------------------------------------------------------ */
/*  Higher-order array operations stubs                                */
/* ------------------------------------------------------------------ */

void arr_map(int *arr, size_t n, int (*fn)(int))
{ (void)arr; (void)n; (void)fn; }

size_t arr_count_if(const int *arr, size_t n, int (*pred)(int))
{ (void)arr; (void)n; (void)pred; return 0; }

int arr_reduce(const int *arr, size_t n, int init, int (*fn)(int, int))
{ (void)arr; (void)n; (void)fn; return init; }

/* ------------------------------------------------------------------ */
/*  Standard-library callbacks stubs                                   */
/* ------------------------------------------------------------------ */

void arr_sort(int *arr, size_t n, int (*cmp)(int, int))
{ (void)arr; (void)n; (void)cmp; }

int arr_bsearch(const int *arr, size_t n, int target)
{ (void)arr; (void)n; (void)target; return -1; }

/* ------------------------------------------------------------------ */
/*  vtable dispatch stubs                                              */
/* ------------------------------------------------------------------ */

double      shape_area     (const Shape *s) { (void)s; return 0.0; }
double      shape_perimeter(const Shape *s) { (void)s; return 0.0; }
const char *shape_name     (const Shape *s) { (void)s; return ""; }

/* Circle vtable (filled in during GREEN phase) */
static double      circle_area     (const void *self) { (void)self; return 0.0; }
static double      circle_perimeter(const void *self) { (void)self; return 0.0; }
static const char *circle_name     (void)              { return "Circle"; }

static const ShapeOps circle_ops = {
    circle_area, circle_perimeter, circle_name
};

void circle_init(Circle *c, double radius)
{ (void)c; (void)radius; }

/* ShapeRect vtable */
static double      shaperect_area     (const void *self) { (void)self; return 0.0; }
static double      shaperect_perimeter(const void *self) { (void)self; return 0.0; }
static const char *shaperect_name     (void)              { return "Rectangle"; }

static const ShapeOps shaperect_ops = {
    shaperect_area, shaperect_perimeter, shaperect_name
};

void shaperect_init(ShapeRect *r, double width, double height)
{ (void)r; (void)width; (void)height; }

/* Suppress unused-variable warnings on the vtable structs in RED phase. */
static const ShapeOps *_unused_c = &circle_ops;
static const ShapeOps *_unused_r = &shaperect_ops;
