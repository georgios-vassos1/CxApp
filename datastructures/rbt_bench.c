/*
 * rbt_bench.c — Asymptotic complexity benchmark for the red-black tree.
 *
 * Measures per-operation time at geometrically-spaced sizes and verifies
 * that the empirical growth matches the expected worst-case complexity.
 *
 * Build:  make demos        (via the project Makefile)
 * Run:    ./rbt_bench
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "rbt.h"

/* ------------------------------------------------------------------ */
/*  Timing helpers                                                     */
/* ------------------------------------------------------------------ */

static double now_ns(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1e9 + (double)ts.tv_nsec;
}

/* ------------------------------------------------------------------ */
/*  Comparator                                                         */
/* ------------------------------------------------------------------ */

static int cmp_int(const void *a, const void *b)
{
    int va = *(const int *)a;
    int vb = *(const int *)b;
    return (va > vb) - (va < vb);
}

/* ------------------------------------------------------------------ */
/*  Shuffle helper (Fisher–Yates)                                      */
/* ------------------------------------------------------------------ */

static void shuffle(int *arr, size_t n)
{
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = (size_t)rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

/* ------------------------------------------------------------------ */
/*  Benchmark sizes                                                    */
/* ------------------------------------------------------------------ */

#define NUM_SIZES 4
static const size_t SIZES[NUM_SIZES] = { 10000, 50000, 200000, 1000000 };

/* ------------------------------------------------------------------ */
/*  Per-operation results                                              */
/* ------------------------------------------------------------------ */

typedef struct {
    size_t n;
    double total_ns;
    double per_op_ns;
} BenchResult;

/* ------------------------------------------------------------------ */
/*  Benchmark functions                                                */
/* ------------------------------------------------------------------ */

/*
 * Insert: expected O(log n) per operation.
 * Insert n elements (shuffled to avoid degenerate patterns on a plain BST,
 * though the RBT handles any order).
 */
static void bench_insert(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;
        shuffle(vals, n);

        RBTree *t = rbt_create(cmp_int, NULL);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
    }
}

/*
 * Search (hit): expected O(log n) per operation.
 * Build tree of n elements, then search for every element (shuffled order).
 */
static void bench_search(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        int *queries = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) {
            vals[i] = (int)i;
            queries[i] = (int)i;
        }
        shuffle(vals, n);
        shuffle(queries, n);

        RBTree *t = rbt_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            rbt_search(t, &queries[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
        free(queries);
    }
}

/*
 * Delete: expected O(log n) per operation.
 * Build tree of n elements, then delete every element (shuffled order).
 */
static void bench_delete(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        int *del_order = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) {
            vals[i] = (int)i;
            del_order[i] = (int)i;
        }
        shuffle(vals, n);
        shuffle(del_order, n);

        RBTree *t = rbt_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            rbt_delete(t, &del_order[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
        free(del_order);
    }
}

/*
 * Min / Max: expected O(log n) — walks leftmost / rightmost spine.
 * Calls rbt_min n times on a tree of size n.
 */
static void bench_min(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;
        shuffle(vals, n);

        RBTree *t = rbt_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            rbt_min(t);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
    }
}

/*
 * Inorder traversal: expected O(n) total, so O(1) amortized per element.
 */
static void noop_visit(const void *d) { (void)d; }

static void bench_inorder(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;
        shuffle(vals, n);

        RBTree *t = rbt_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);

        double t0 = now_ns();
        rbt_inorder(t, noop_visit);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
    }
}

/*
 * Insert (sorted): expected O(log n) per operation — worst-case input for
 * an unbalanced BST but the RBT should still be O(log n).
 */
static void bench_insert_sorted(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;

        RBTree *t = rbt_create(cmp_int, NULL);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            rbt_insert(t, &vals[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        rbt_free(t);
        free(vals);
    }
}

/* ------------------------------------------------------------------ */
/*  Verification                                                       */
/* ------------------------------------------------------------------ */

typedef enum { COMPLEXITY_O1, COMPLEXITY_LOGN } Complexity;

static const char *complexity_str(Complexity c)
{
    return c == COMPLEXITY_O1 ? "O(1)" : "O(log n)";
}

typedef struct {
    const char  *name;
    Complexity   expected;
    int          pass;       /* 1 = all pairs passed */
} Verdict;

static void print_table_and_verify(const char *name, Complexity expected,
                                   BenchResult results[NUM_SIZES],
                                   Verdict *verdict)
{
    printf("\n=== %s  [expected: %s] ===\n", name, complexity_str(expected));
    printf("%-10s %14s %12s %8s %10s %7s\n",
           "N", "Total (ms)", "Per-op (ns)", "Ratio", "Expected", "Result");
    printf("------------------------------------------------------------------------\n");

    int all_pass = 1;

    for (int s = 0; s < NUM_SIZES; s++) {
        double ratio    = 0.0;
        double exp_ratio = 0.0;
        const char *result = "";

        if (s > 0) {
            ratio = results[s].per_op_ns / results[s - 1].per_op_ns;

            if (expected == COMPLEXITY_O1) {
                exp_ratio = 1.0;
            } else {
                exp_ratio = log((double)SIZES[s]) / log((double)SIZES[s - 1]);
            }

            double lo, hi;
            if (expected == COMPLEXITY_O1) {
                lo = 0.25;
                hi = 4.0;
            } else {
                lo = exp_ratio * 0.25;
                hi = exp_ratio * 4.0;
            }

            if (ratio >= lo && ratio <= hi) {
                result = "PASS";
            } else {
                result = "FAIL";
                all_pass = 0;
            }
        }

        if (s == 0) {
            printf("%-10zu %14.3f %12.1f %8s %10s %7s\n",
                   results[s].n,
                   results[s].total_ns / 1e6,
                   results[s].per_op_ns,
                   "-", "-", "-");
        } else {
            printf("%-10zu %14.3f %12.1f %8.3f %10.3f %7s\n",
                   results[s].n,
                   results[s].total_ns / 1e6,
                   results[s].per_op_ns,
                   ratio,
                   exp_ratio,
                   result);
        }
    }

    verdict->name     = name;
    verdict->expected = expected;
    verdict->pass     = all_pass;
}

/* ------------------------------------------------------------------ */
/*  Main                                                               */
/* ------------------------------------------------------------------ */

int main(void)
{
    srand(42);   /* reproducible shuffles */

    printf("Red-Black Tree — Asymptotic Complexity Benchmark\n");
    printf("Sizes:");
    for (int i = 0; i < NUM_SIZES; i++) printf(" %zu", SIZES[i]);
    printf("\n");

    BenchResult results[NUM_SIZES];

    #define NUM_OPS 6
    Verdict verdicts[NUM_OPS];
    int vi = 0;

    /* --- insert (shuffled) --- */
    printf("\nRunning insert benchmark...\n");
    bench_insert(results);
    print_table_and_verify("insert", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- insert (sorted — worst case for plain BST) --- */
    printf("\nRunning insert (sorted) benchmark...\n");
    bench_insert_sorted(results);
    print_table_and_verify("insert (sorted)", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- search --- */
    printf("\nRunning search benchmark...\n");
    bench_search(results);
    print_table_and_verify("search", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- delete --- */
    printf("\nRunning delete benchmark...\n");
    bench_delete(results);
    print_table_and_verify("delete", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- min --- */
    printf("\nRunning min benchmark...\n");
    bench_min(results);
    print_table_and_verify("min", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- inorder traversal --- */
    printf("\nRunning inorder traversal benchmark...\n");
    bench_inorder(results);
    print_table_and_verify("inorder", COMPLEXITY_O1, results, &verdicts[vi++]);

    /* --- Summary --- */
    printf("\n========================================\n");
    printf("  SUMMARY\n");
    printf("========================================\n");

    int total_pass = 0;
    for (int i = 0; i < NUM_OPS; i++) {
        printf("  %-15s  %s  %s\n",
               verdicts[i].name,
               complexity_str(verdicts[i].expected),
               verdicts[i].pass ? "PASS" : "FAIL");
        if (verdicts[i].pass) total_pass++;
    }

    printf("========================================\n");
    printf("  %d / %d operations passed\n", total_pass, NUM_OPS);
    printf("========================================\n");

    return (total_pass == NUM_OPS) ? 0 : 1;
}
