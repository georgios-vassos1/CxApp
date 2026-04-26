/*
 * structs.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "structs.h"
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Rect stubs                                                         */
/* ------------------------------------------------------------------ */

/* TODO: r.width * r.height */
double rect_area(Rect r)             { (void)r; return 0.0; }
/* TODO: 2 * (r.width + r.height) */
double rect_perimeter(Rect r)        { (void)r; return 0.0; }
/* TODO: p strictly inside origin..(origin+width, origin+height) */
int    rect_contains(Rect r, Point p){ (void)r; (void)p; return 0; }
/* TODO: two rects overlap iff neither is fully left/right/above/below the other */
int    rect_intersects(Rect a, Rect b){ (void)a; (void)b; return 0; }

/* ------------------------------------------------------------------ */
/*  Queue internals (hidden from callers)                              */
/* ------------------------------------------------------------------ */

typedef struct QNode {
    int           val;
    struct QNode *next;
} QNode;

struct Queue {
    QNode  *head;
    QNode  *tail;
    size_t  size;
};

/* ------------------------------------------------------------------ */
/*  Queue stubs                                                        */
/* ------------------------------------------------------------------ */

/* TODO: calloc Queue; head=tail=NULL; size=0 */
Queue *queue_create(void)                      { return NULL; }
/* TODO: malloc QNode; append to tail (update head if empty) */
void   queue_enqueue(Queue *q, int val)        { (void)q; (void)val; }
/* TODO: if empty return 0; store head->val in *out; advance head; free old node */
int    queue_dequeue(Queue *q, int *out)       { (void)q; (void)out; return 0; }
/* TODO: if empty return 0; store head->val in *out without removing */
int    queue_peek(const Queue *q, int *out)    { (void)q; (void)out; return 0; }
/* TODO: return q->size */
size_t queue_size(const Queue *q)              { (void)q; return 0; }
/* TODO: return q->size == 0 */
int    queue_is_empty(const Queue *q)          { (void)q; return 1; }
/* TODO: free each node in order, then free(q) */
void   queue_free(Queue *q)                    { (void)q; }
