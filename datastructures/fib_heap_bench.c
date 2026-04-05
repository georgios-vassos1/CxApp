/*
 * fib_heap_bench.c — Asymptotic complexity benchmark for the Fibonacci heap.
 *
 * Measures per-operation time at geometrically-spaced sizes and verifies
 * that the empirical growth matches the expected amortized complexity.
 *
 * Build:  make demos        (via the project Makefile)
 * Run:    ./fib_heap_bench
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "fib_heap.h"

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
/*  Benchmark sizes                                                    */
/* ------------------------------------------------------------------ */

#define NUM_SIZES 4
static const size_t SIZES[NUM_SIZES] = { 10000, 50000, 200000, 1000000 };

/* Merge repetitions — scaled per size so large heaps don't take forever.
   We need enough reps for stable timing but can't rebuild million-element
   heaps 100K times. */
static int merge_reps(size_t n)
{
    if (n <= 10000)  return 50000;
    if (n <= 50000)  return 10000;
    if (n <= 200000) return 2000;
    return 500;
}

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

static void bench_insert(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        /* Pre-allocate data. */
        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;

        FibHeap *h = fh_create(cmp_int, NULL);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            fh_insert(h, &vals[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        fh_free(h);
        free(vals);
    }
}

static void bench_find_min(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;

        FibHeap *h = fh_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            fh_insert(h, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            fh_find_min(h);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        fh_free(h);
        free(vals);
    }
}

static void bench_extract_min(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        for (size_t i = 0; i < n; i++) vals[i] = (int)i;

        FibHeap *h = fh_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            fh_insert(h, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            fh_extract_min(h);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        fh_free(h);
        free(vals);
    }
}

static void bench_decrease_key(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        /* Insert values [n .. 2n-1], then decrease to [0 .. n-1]. */
        int *insert_vals  = malloc(n * sizeof(int));
        int *decrease_vals = malloc(n * sizeof(int));
        FibNode **handles = malloc(n * sizeof(FibNode *));

        for (size_t i = 0; i < n; i++) {
            insert_vals[i]  = (int)(n + i);
            decrease_vals[i] = (int)i;
        }

        FibHeap *h = fh_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            handles[i] = fh_insert(h, &insert_vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            fh_decrease_key(h, handles[i], &decrease_vals[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        fh_free(h);
        free(insert_vals);
        free(decrease_vals);
        free(handles);
    }
}

static void bench_delete(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];

        int *vals = malloc(n * sizeof(int));
        FibNode **handles = malloc(n * sizeof(FibNode *));

        for (size_t i = 0; i < n; i++) vals[i] = (int)i;

        FibHeap *h = fh_create(cmp_int, NULL);
        for (size_t i = 0; i < n; i++)
            handles[i] = fh_insert(h, &vals[i]);

        double t0 = now_ns();
        for (size_t i = 0; i < n; i++)
            fh_delete(h, handles[i]);
        double t1 = now_ns();

        results[s].n         = n;
        results[s].total_ns  = t1 - t0;
        results[s].per_op_ns = (t1 - t0) / (double)n;

        fh_free(h);
        free(vals);
        free(handles);
    }
}

static void bench_merge(BenchResult results[NUM_SIZES])
{
    for (int s = 0; s < NUM_SIZES; s++) {
        size_t n = SIZES[s];
        size_t half = n / 2;

        /* Pre-allocate data for all repetitions. */
        int *vals_a = malloc(half * sizeof(int));
        int *vals_b = malloc(half * sizeof(int));
        for (size_t i = 0; i < half; i++) {
            vals_a[i] = (int)i;
            vals_b[i] = (int)(half + i);
        }

        int reps = merge_reps(n);
        double total = 0.0;
        for (int r = 0; r < reps; r++) {
            FibHeap *a = fh_create(cmp_int, NULL);
            FibHeap *b = fh_create(cmp_int, NULL);
            for (size_t i = 0; i < half; i++) {
                fh_insert(a, &vals_a[i]);
                fh_insert(b, &vals_b[i]);
            }

            double t0 = now_ns();
            FibHeap *merged = fh_merge(a, b);
            double t1 = now_ns();
            total += (t1 - t0);

            fh_free(merged);
        }

        results[s].n         = n;
        results[s].total_ns  = total;
        results[s].per_op_ns = total / (double)reps;

        free(vals_a);
        free(vals_b);
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
    printf("Fibonacci Heap — Asymptotic Complexity Benchmark\n");
    printf("Sizes:");
    for (int i = 0; i < NUM_SIZES; i++) printf(" %zu", SIZES[i]);
    printf("\n");

    BenchResult results[NUM_SIZES];

    #define NUM_OPS 6
    Verdict verdicts[NUM_OPS];
    int vi = 0;

    /* --- insert --- */
    printf("\nRunning insert benchmark...\n");
    bench_insert(results);
    print_table_and_verify("insert", COMPLEXITY_O1, results, &verdicts[vi++]);

    /* --- find_min --- */
    printf("\nRunning find_min benchmark...\n");
    bench_find_min(results);
    print_table_and_verify("find_min", COMPLEXITY_O1, results, &verdicts[vi++]);

    /* --- extract_min --- */
    printf("\nRunning extract_min benchmark...\n");
    bench_extract_min(results);
    print_table_and_verify("extract_min", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- decrease_key --- */
    printf("\nRunning decrease_key benchmark...\n");
    bench_decrease_key(results);
    print_table_and_verify("decrease_key", COMPLEXITY_O1, results, &verdicts[vi++]);

    /* --- delete --- */
    printf("\nRunning delete benchmark...\n");
    bench_delete(results);
    print_table_and_verify("delete", COMPLEXITY_LOGN, results, &verdicts[vi++]);

    /* --- merge --- */
    printf("\nRunning merge benchmark...\n");
    bench_merge(results);
    print_table_and_verify("merge", COMPLEXITY_O1, results, &verdicts[vi++]);

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
