/*
 * strings_arrays.c — TDD RED PHASE: stubs.
 * Implement each function. Run `make test` after each one.
 */

#include "strings_arrays.h"
#include <stdlib.h>

size_t str_len(const char *s)          { (void)s; return 0; }

size_t str_ncopy(char *dst, size_t dst_size, const char *src)
{ (void)dst; (void)dst_size; (void)src; return 0; }

size_t str_ncat(char *dst, size_t dst_size, const char *src)
{ (void)dst; (void)dst_size; (void)src; return 0; }

char *str_find(const char *s, char c)  { (void)s; (void)c; return NULL; }

void str_rev(char *s)                  { (void)s; }

int str_is_palindrome(const char *s)   { (void)s; return 0; }

int **matrix_create(size_t rows, size_t cols)
{ (void)rows; (void)cols; return NULL; }

void matrix_set(int **m, size_t row, size_t col, int val)
{ (void)m; (void)row; (void)col; (void)val; }

int matrix_get(int **m, size_t row, size_t col)
{ (void)m; (void)row; (void)col; return 0; }

void matrix_free(int **m, size_t rows) { (void)m; (void)rows; }

char **strarray_create(const char * const *src, size_t count)
{ (void)src; (void)count; return NULL; }

void strarray_free(char **arr, size_t count) { (void)arr; (void)count; }
