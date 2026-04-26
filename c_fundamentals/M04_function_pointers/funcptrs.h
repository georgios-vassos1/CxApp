/*
 * funcptrs.h — Module 4: Function Pointers (C Fundamentals).
 *
 * Three groups of exercises:
 *
 * 1. Higher-order array operations — map, filter, reduce.
 *    Goal: pass functions as arguments, practise callback syntax.
 *
 * 2. Standard-library callbacks — qsort and bsearch wrappers.
 *    Goal: understand the comparator contract and how to adapt it.
 *
 * 3. vtable-style polymorphism — a Shape "interface" with a struct of
 *    function pointers.
 *    Goal: understand how C++/OOP vtables work at the C level.
 *    This pattern appears in OS kernels, game engines, and embedded code.
 */

#ifndef C_FUND_FUNCPTRS_H
#define C_FUND_FUNCPTRS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t */

/* ------------------------------------------------------------------ */
/*  1. Higher-order array operations                                   */
/* ------------------------------------------------------------------ */

/*
 * Apply fn to every element of arr in place: arr[i] = fn(arr[i]).
 */
void arr_map(int *arr, size_t n, int (*fn)(int));

/*
 * Return the number of elements in arr[0..n-1] for which pred returns non-zero.
 */
size_t arr_count_if(const int *arr, size_t n, int (*pred)(int));

/*
 * Left fold: accumulator starts at init, then acc = fn(acc, arr[i]) for each i.
 * Returns the final accumulator.
 */
int arr_reduce(const int *arr, size_t n, int init, int (*fn)(int, int));

/* ------------------------------------------------------------------ */
/*  2. Standard-library callbacks                                      */
/* ------------------------------------------------------------------ */

/*
 * Sort arr[0..n-1] in place using cmp as the ordering function.
 * cmp(a, b) must return < 0 if a < b, 0 if equal, > 0 if a > b.
 * Wraps qsort — do not re-implement sorting.
 */
void arr_sort(int *arr, size_t n, int (*cmp)(int, int));

/*
 * Binary search for target in sorted arr[0..n-1].
 * arr must already be sorted in the order defined by cmp_asc
 * (ascending: cmp_asc(a,b) < 0 iff a < b).
 * Returns the index of target, or -1 if not found.
 * Wraps bsearch — do not re-implement binary search.
 */
int arr_bsearch(const int *arr, size_t n, int target);

/* ------------------------------------------------------------------ */
/*  3. vtable-style polymorphism                                       */
/* ------------------------------------------------------------------ */

/*
 * A "vtable" — a struct of function pointers shared by all instances
 * of a given "class".  Every Shape type provides one of these.
 */
typedef struct {
    double      (*area)     (const void *self);
    double      (*perimeter)(const void *self);
    const char *(*name)     (void);
} ShapeOps;

/*
 * Base "class" — every concrete shape embeds this as its first member.
 * This lets a Shape* be safely cast to Circle*, ShapeRect*, etc.
 */
typedef struct {
    const ShapeOps *ops;
} Shape;

/* Dispatch area/perimeter/name through the vtable. */
double      shape_area     (const Shape *s);
double      shape_perimeter(const Shape *s);
const char *shape_name     (const Shape *s);

/* --- Circle --- */
typedef struct {
    Shape  base;     /* MUST be first */
    double radius;
} Circle;

/* Initialise *c.  Sets base.ops to the Circle vtable. */
void circle_init(Circle *c, double radius);

/* --- Rectangle --- */
typedef struct {
    Shape  base;     /* MUST be first */
    double width;
    double height;
} ShapeRect;

/* Initialise *r.  Sets base.ops to the ShapeRect vtable. */
void shaperect_init(ShapeRect *r, double width, double height);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_FUNCPTRS_H */
