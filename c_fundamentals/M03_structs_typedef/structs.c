/*
 * structs.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "structs.h"
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Rect stubs                                                         */
/* ------------------------------------------------------------------ */

double rect_area(Rect r)             { (void)r; return 0.0; }
double rect_perimeter(Rect r)        { (void)r; return 0.0; }
int    rect_contains(Rect r, Point p){ (void)r; (void)p; return 0; }
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

Queue *queue_create(void)                      { return NULL; }
void   queue_enqueue(Queue *q, int val)        { (void)q; (void)val; }
int    queue_dequeue(Queue *q, int *out)       { (void)q; (void)out; return 0; }
int    queue_peek(const Queue *q, int *out)    { (void)q; (void)out; return 0; }
size_t queue_size(const Queue *q)              { (void)q; return 0; }
int    queue_is_empty(const Queue *q)          { (void)q; return 1; }
void   queue_free(Queue *q)                    { (void)q; }
