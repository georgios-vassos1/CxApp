#ifndef DLIST_H
#define DLIST_H

/*
 * Doubly linked circular list with a sentinel node.
 *
 * Layout:  sentinel <-> node_0 <-> node_1 <-> ... <-> node_n <-> sentinel
 *
 * The sentinel is a permanent dummy node whose word is NULL.  It acts as
 * both the head and the tail anchor, eliminating all NULL edge-cases.
 * Nodes are kept in ascending lexicographic order by word.
 *
 * Both DList and DNode are opaque — external code accesses node fields
 * through accessor functions and iterates via dlist_foreach().  This
 * makes the module suitable for wrapping from Python (ctypes/cffi) or R
 * (.Call / Rcpp) without leaking implementation details.
 */

/* Opaque types — full definitions live in dlist.c */
typedef struct DNode DNode;
typedef struct DList DList;

/* ---- node accessors ---- */

const char *dnode_word       (const DNode *n);
int         dnode_num_reviews(const DNode *n);
float       dnode_sum_ratings(const DNode *n);

/* ---- lifecycle ---- */

DList *dlist_create (void);                    /* NULL on alloc failure */
void   dlist_destroy(DList *list);

/* ---- query ---- */

int    dlist_size  (const DList *list);
DNode *dlist_search(const DList *list, const char *word);

/*
 * dlist_foreach: calls cb(node, arg) for every real node in sorted order.
 * The callback must NOT mutate the list structure (insert/delete).
 */
void dlist_foreach(const DList *list,
                   void (*cb)(const DNode *node, void *arg), void *arg);

/* ---- node mutation ---- */

/*
 * dnode_accumulate: adds delta_reviews and delta_ratings to an existing
 * node's counters.  The node must have been obtained from dlist_search()
 * or htable_search() (which returns a mutable DNode*).
 */
void dnode_accumulate(DNode *n, int delta_reviews, float delta_ratings);

/* ---- list mutation ---- */

/*
 * dlist_insert_sorted: inserts a new node in lexicographic order.
 * If word already exists, num_reviews and sum_ratings are accumulated
 * into the existing node (no duplicate nodes are ever created).
 * Returns 0 on success, -1 on allocation failure.
 */
int  dlist_insert_sorted(DList *list, const char *word,
                         int num_reviews, float sum_ratings);

/*
 * dlist_delete: removes the node for word if present.
 * Returns 1 if a node was removed, 0 if word was not found.
 */
int  dlist_delete(DList *list, const char *word);

/* ---- debug ---- */

void dlist_print(const DList *list);

#endif /* DLIST_H */
