#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 21
#define CHUNK_SIZE 5000
#define NO_WORD_LENGTH_CONDITION -1
#define MAX_PATH_LENGTH 1024

char*  getFullPath(const char* filename);
char** readLinesFromFile(const char* filename, int* totalWords, const int wordLength);

#endif /* READER_H */

