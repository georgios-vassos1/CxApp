#ifndef READER_H
#define READER_H

#define MAX_WORD_LENGTH 21
#define CHUNK_SIZE      5000
#define MAX_PATH_LENGTH 1024

/* Pass 0 as wordLength to accept words of any length. */
#define NO_WORD_LENGTH_CONDITION 0

char *getFullPath(const char *filename);
char **readLinesFromFile(const char *filename, int *totalWords, int wordLength);

#endif // READER_H
