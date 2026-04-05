#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

#include "hw2.h"

/* Maximum word length we'll ever extract from review text.
 * No English word exceeds this; anything longer is silently truncated. */
#define MAX_WORD_LEN 256

/* =========================================================================
 * Small I/O helpers
 * ========================================================================= */

static void print_menu(void)
{
    printf("\n--- Movie Review System ---\n");
    printf("  A  Load reviews from file\n");
    printf("  S  Search a word\n");
    printf("  I  Infer rating of a new review\n");
    printf("  B  Best-rated words\n");
    printf("  W  Worst-rated words\n");
    printf("  P  Print reviews for a movie\n");
    printf("  R  Remove all reviews by a reviewer\n");
    printf("  D  Dump hash table\n");
    printf("  Q  Quit\n");
    printf("Select: ");
    fflush(stdout);
}

/*
 * Read one line from stdin into a heap-allocated string (newline stripped).
 * Returns NULL on EOF.
 */
static char *read_line(void)
{
    char buf[2048];
    if (!fgets(buf, sizeof(buf), stdin)) return NULL;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[--len] = '\0';
    char *s = strdup(buf);
    if (!s) { perror("read_line: strdup"); exit(EXIT_FAILURE); }
    return s;
}

/* =========================================================================
 * Word tokeniser
 *
 * Calls cb(lowercase_word, arg) for every maximal alphabetic run in text.
 * Uses a stack buffer — no heap allocation per word.
 * ========================================================================= */

static void for_each_word(const char *text,
                          void (*cb)(const char *word, void *arg),
                          void *arg)
{
    const char *p = text;
    char word[MAX_WORD_LEN];

    while (*p) {
        while (*p && !isalpha((unsigned char)*p)) p++;
        if (!*p) break;

        const char *start = p;
        while (*p && isalpha((unsigned char)*p)) p++;

        size_t len = (size_t)(p - start);
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;   /* truncate */

        for (size_t i = 0; i < len; i++)
            word[i] = (char)tolower((unsigned char)start[i]);
        word[len] = '\0';

        cb(word, arg);
    }
}

/* =========================================================================
 * A — Load reviews from file
 * ========================================================================= */

typedef struct { HashTable *ht; float score; } LoadCtx;

static void insert_word(const char *word, void *arg)
{
    LoadCtx *ctx = arg;
    htable_insert(ctx->ht, word, 1, ctx->score);
}

static void do_load(SList *sl, HashTable *ht)
{
    printf("Filename: ");
    fflush(stdout);
    char *fname = read_line();
    if (!fname) return;

    int n = 0;
    review_t *r;
    while ((r = read_review(fname)) != NULL) {
        LoadCtx ctx = { ht, r->review_score };
        for_each_word(r->review_text, insert_word, &ctx);
        if (slist_insert(sl, r) != 0) {
            fprintf(stderr, "Out of memory while loading reviews.\n");
            break;
        }
        n++;
    }
    printf("Loaded %d review(s) from '%s'.\n", n, fname);
    free(fname);
}

/* =========================================================================
 * S — Search a word
 * ========================================================================= */

static void do_search(HashTable *ht)
{
    printf("Word: ");
    fflush(stdout);
    char *word = read_line();
    if (!word) return;

    for (char *p = word; *p; p++) *p = (char)tolower((unsigned char)*p);

    DNode *node = htable_search(ht, word);
    if (!node)
        printf("'%s' not found.\n", word);
    else
        printf("'%s'  reviews: %d  avg rating: %.4f\n",
               dnode_word(node), dnode_num_reviews(node),
               dnode_sum_ratings(node) / (float)dnode_num_reviews(node));
    free(word);
}

/* =========================================================================
 * I — Infer the rating of a new review
 *
 * Score = mean of the per-word average ratings for every known word.
 * Classification:  [3,4] Positive  |  [2,3) Neutral  |  [0,2) Negative
 * ========================================================================= */

typedef struct { HashTable *ht; double sum; int count; } InferCtx;

static void infer_word(const char *word, void *arg)
{
    InferCtx *ctx  = arg;
    DNode    *node = htable_search(ctx->ht, word);
    if (node) {
        ctx->sum += dnode_sum_ratings(node) / (double)dnode_num_reviews(node);
        ctx->count++;
    }
}

static void do_infer(HashTable *ht)
{
    printf("Review text: ");
    fflush(stdout);
    char *text = read_line();
    if (!text) return;

    InferCtx ctx = { ht, 0.0, 0 };
    for_each_word(text, infer_word, &ctx);

    if (ctx.count == 0) {
        printf("No known words found — cannot infer a rating.\n");
    } else {
        double score = ctx.sum / ctx.count;
        const char *label = (score >= 3.0) ? "Positive" :
                            (score >= 2.0) ? "Neutral"  : "Negative";
        printf("Inferred score: %.4f  →  %s\n", score, label);
    }
    free(text);
}

/* =========================================================================
 * B / W — Best and worst rated words
 *
 * Two-pass over the hash table using htable_foreach:
 *   1. Find the global max / min average.
 *   2. Print every word within TOLERANCE of that extreme.
 * ========================================================================= */

#define TOLERANCE 0.005f

typedef struct { float lo; float hi; } ExtremeCtx;

static void extremes_cb(const DNode *node, void *arg)
{
    ExtremeCtx *ctx = arg;
    float avg = dnode_sum_ratings(node) / (float)dnode_num_reviews(node);
    if (avg < ctx->lo) ctx->lo = avg;
    if (avg > ctx->hi) ctx->hi = avg;
}

typedef struct { float target; } NearCtx;

static void print_near_cb(const DNode *node, void *arg)
{
    NearCtx *ctx = arg;
    float avg = dnode_sum_ratings(node) / (float)dnode_num_reviews(node);
    if (fabsf(avg - ctx->target) <= TOLERANCE)
        printf("  %-24s  avg: %.4f\n", dnode_word(node), avg);
}

static void do_best(const HashTable *ht)
{
    if (htable_count(ht) == 0) { printf("Hash table is empty.\n"); return; }
    ExtremeCtx ext = { FLT_MAX, -FLT_MAX };
    htable_foreach(ht, extremes_cb, &ext);
    printf("Highest average rating: %.4f\n", ext.hi);
    NearCtx near = { ext.hi };
    htable_foreach(ht, print_near_cb, &near);
}

static void do_worst(const HashTable *ht)
{
    if (htable_count(ht) == 0) { printf("Hash table is empty.\n"); return; }
    ExtremeCtx ext = { FLT_MAX, -FLT_MAX };
    htable_foreach(ht, extremes_cb, &ext);
    printf("Lowest average rating: %.4f\n", ext.lo);
    NearCtx near = { ext.lo };
    htable_foreach(ht, print_near_cb, &near);
}

/* =========================================================================
 * P — Print reviews for a movie
 * ========================================================================= */

static void do_print_reviews(const SList *sl)
{
    printf("Movie title (* for all): ");
    fflush(stdout);
    char *title = read_line();
    if (!title) return;
    slist_print_movie(sl, title);
    free(title);
}

/* =========================================================================
 * R — Remove all reviews by a reviewer
 *
 * Two logical passes:
 *   1. slist_foreach to update the hash table (decrement word counters).
 *   2. slist_delete_reviewer to remove the review nodes.
 *
 * We do the hash-table updates before the slist removal so we still have
 * access to the review text and score when we need them.
 * ========================================================================= */

typedef struct { HashTable *ht; float score; } RemoveCtx;

static void decrement_word(const char *word, void *arg)
{
    RemoveCtx *ctx  = arg;
    DNode     *node = htable_search(ctx->ht, word);
    if (!node) return;

    dnode_accumulate(node, -1, -(ctx->score));

    if (dnode_num_reviews(node) == 0)
        htable_delete(ctx->ht, word);
}

typedef struct { HashTable *ht; const char *reviewer; } ReviewerCtx;

static void update_htable_for_review(const review_t *review, void *arg)
{
    ReviewerCtx *ctx = arg;
    if (strcmp(review->reviewer_name, ctx->reviewer) != 0) return;
    RemoveCtx rctx = { ctx->ht, review->review_score };
    for_each_word(review->review_text, decrement_word, &rctx);
}

static void do_remove_reviewer(SList *sl, HashTable *ht)
{
    printf("Reviewer name: ");
    fflush(stdout);
    char *name = read_line();
    if (!name) return;

    /* Pass 1: fix the hash table via public iterator. */
    ReviewerCtx ctx = { ht, name };
    slist_foreach(sl, update_htable_for_review, &ctx);

    /* Pass 2: remove review nodes from the list. */
    int removed = slist_delete_reviewer(sl, name);
    printf("Removed %d review(s) by '%s'.\n", removed, name);
    free(name);
}

/* =========================================================================
 * main
 * ========================================================================= */

int main(void)
{
    SList     *sl = slist_create();
    HashTable *ht = htable_create(INIT_HSIZE);
    if (!sl || !ht) {
        fprintf(stderr, "Failed to initialise data structures.\n");
        return EXIT_FAILURE;
    }

    int sel;
    for (;;) {
        print_menu();

        sel = getchar();
        if (sel == EOF) break;

        /* consume the rest of the input line */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        switch (toupper((unsigned char)sel)) {
        case 'A': do_load(sl, ht);            break;
        case 'S': do_search(ht);              break;
        case 'I': do_infer(ht);               break;
        case 'B': do_best(ht);                break;
        case 'W': do_worst(ht);               break;
        case 'P': do_print_reviews(sl);       break;
        case 'R': do_remove_reviewer(sl, ht); break;
        case 'D': htable_print(ht);           break;
        case 'Q':
            slist_destroy(sl);
            htable_destroy(ht);
            printf("Bye.\n");
            return 0;
        default:
            printf("Invalid selection.\n");
        }
    }

    slist_destroy(sl);
    htable_destroy(ht);
    return 0;
}
