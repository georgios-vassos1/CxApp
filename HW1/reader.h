#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 21
#define CHUNK_SIZE 5000
#define NO_WORD_LENGTH_CONDITION -1

char** readLinesFromFile(const char* filename, int* totalWords, const int wordLength);
// char** readLinesFromFile(const char* filename, int* totalWords);

#endif /* READER_H */

