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

/* TODO: arr[i] = fn(arr[i]) for each i */
void arr_map(int *arr, size_t n, int (*fn)(int))
{ (void)arr; (void)n; (void)fn; }

/* TODO: count elements where pred(arr[i]) != 0 */
size_t arr_count_if(const int *arr, size_t n, int (*pred)(int))
{ (void)arr; (void)n; (void)pred; return 0; }

/* TODO: acc=init; for each i: acc=fn(acc,arr[i]); return acc */
int arr_reduce(const int *arr, size_t n, int init, int (*fn)(int, int))
{ (void)arr; (void)n; (void)fn; return init; }

/* ------------------------------------------------------------------ */
/*  Standard-library callbacks stubs                                   */
/* ------------------------------------------------------------------ */

/* TODO: wrap qsort; adapt cmp(int,int) to qsort's (const void*,const void*) signature */
void arr_sort(int *arr, size_t n, int (*cmp)(int, int))
{ (void)arr; (void)n; (void)cmp; }

/* TODO: wrap bsearch; cast result pointer back to index (ptr - arr) */
int arr_bsearch(const int *arr, size_t n, int target)
{ (void)arr; (void)n; (void)target; return -1; }

/* ------------------------------------------------------------------ */
/*  vtable dispatch stubs                                              */
/* ------------------------------------------------------------------ */

double      shape_area     (const Shape *s) { (void)s; return 0.0; }
double      shape_perimeter(const Shape *s) { (void)s; return 0.0; }
const char *shape_name     (const Shape *s) { (void)s; return ""; }

/* TODO: implement these three, then assign them to circle_ops below */
static double      circle_area     (const void *self) { (void)self; return 0.0; }  /* TODO: M_PI * r * r */
static double      circle_perimeter(const void *self) { (void)self; return 0.0; }  /* TODO: 2 * M_PI * r */
static const char *circle_name     (void)              { return "Circle"; }

static const ShapeOps circle_ops = {
    circle_area, circle_perimeter, circle_name
};

/* TODO: set c->radius and point c->base.ops to &circle_ops */
void circle_init(Circle *c, double radius)
{ (void)c; (void)radius; }

/* TODO: implement these three, then assign them to shaperect_ops below */
static double      shaperect_area     (const void *self) { (void)self; return 0.0; }  /* TODO: w * h */
static double      shaperect_perimeter(const void *self) { (void)self; return 0.0; }  /* TODO: 2*(w+h) */
static const char *shaperect_name     (void)              { return "Rectangle"; }

static const ShapeOps shaperect_ops = {
    shaperect_area, shaperect_perimeter, shaperect_name
};

/* TODO: set r->width, r->height and point r->base.ops to &shaperect_ops */
void shaperect_init(ShapeRect *r, double width, double height)
{ (void)r; (void)width; (void)height; }

/* Suppress unused-variable warnings on the vtable structs in RED phase. */
static const ShapeOps *_unused_c = &circle_ops;
static const ShapeOps *_unused_r = &shaperect_ops;
