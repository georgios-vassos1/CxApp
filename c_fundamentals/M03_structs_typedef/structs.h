/*
 * structs.h — Module 3: Structs & typedef (C Fundamentals).
 *
 * Two groups of exercises:
 *
 * 1. Point / Rect — transparent structs with designated initializers.
 *    Goal: practise struct layout, field access, and passing structs
 *    by value vs. by pointer.
 *
 * 2. Queue — opaque struct following the pattern used throughout this
 *    repo (e.g. BTree, RBTree).
 *    Goal: understand why opaque types exist (ABI stability, invariant
 *    protection), and practise the alloc / free ownership contract.
 *
 * Designated initializer syntax (C99+):
 *   Point p = { .x = 1.0, .y = 2.0 };
 *   Rect  r = { .origin = { .x = 0.0, .y = 0.0 }, .width = 10.0, .height = 5.0 };
 */

#ifndef C_FUND_STRUCTS_H
#define C_FUND_STRUCTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>   /* size_t */

/* ------------------------------------------------------------------ */
/*  1. Point and Rect — transparent structs                           */
/* ------------------------------------------------------------------ */

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    Point  origin;   /* top-left corner */
    double width;
    double height;
} Rect;

/* Area = width × height. */
double rect_area(Rect r);

/* Perimeter = 2 × (width + height). */
double rect_perimeter(Rect r);

/*
 * Return 1 if point p lies strictly inside rect r (not on the boundary).
 * Return 0 otherwise.
 */
int rect_contains(Rect r, Point p);

/*
 * Return 1 if rects a and b overlap (share any interior area).
 * Touching edges / corners do not count as overlapping.
 * Return 0 otherwise.
 */
int rect_intersects(Rect a, Rect b);

/* ------------------------------------------------------------------ */
/*  2. Queue — opaque FIFO of ints                                    */
/* ------------------------------------------------------------------ */

/*
 * Opaque handle — the struct definition lives in structs.c.
 * Callers interact only through the functions below.
 *
 * Implementation note: use a singly-linked list of nodes.
 * Keep a tail pointer so enqueue is O(1).
 */
typedef struct Queue Queue;

/* Allocate and return a new, empty Queue. Returns NULL on failure. */
Queue *queue_create(void);

/* Add val to the back of the queue. */
void queue_enqueue(Queue *q, int val);

/*
 * Remove the front element and store it in *out.
 * Returns 1 on success, 0 if the queue is empty (out is untouched).
 */
int queue_dequeue(Queue *q, int *out);

/*
 * Peek at the front element without removing it, store it in *out.
 * Returns 1 on success, 0 if the queue is empty.
 */
int queue_peek(const Queue *q, int *out);

/* Number of elements currently in the queue. */
size_t queue_size(const Queue *q);

/* Return 1 if the queue has no elements, 0 otherwise. */
int queue_is_empty(const Queue *q);

/* Free all nodes and the Queue struct itself. */
void queue_free(Queue *q);

#ifdef __cplusplus
}
#endif

#endif /* C_FUND_STRUCTS_H */
