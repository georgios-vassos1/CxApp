#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reader.h"

char *getFullPath(const char *filename) {
    char cwd[MAX_PATH_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error: Unable to get current working directory");
        exit(EXIT_FAILURE);
    }

    char *full_path = malloc(MAX_PATH_LENGTH);
    if (full_path == NULL) {
        perror("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", cwd, filename);
    return full_path;
}

char **readLinesFromFile(const char *filename, int *totalWords, int wordLength) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char **words = malloc(CHUNK_SIZE * sizeof(char *));
    if (words == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    int wordsAllocated = CHUNK_SIZE;

    /*
     * Build the format string from MAX_WORD_LENGTH so the width limit
     * stays in sync with the buffer size automatically.
     */
    char scan_fmt[16];
    snprintf(scan_fmt, sizeof(scan_fmt), "%%%ds", MAX_WORD_LENGTH - 1);

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, scan_fmt, word) == 1) {
        if (wordLength == NO_WORD_LENGTH_CONDITION ||
            (int)strlen(word) == wordLength) {
            if (j == wordsAllocated) {
                wordsAllocated += CHUNK_SIZE;
                char **tmp = realloc(words, wordsAllocated * sizeof(char *));
                if (tmp == NULL) {
                    fprintf(stderr, "Error allocating memory\n");
                    exit(EXIT_FAILURE);
                }
                words = tmp;
            }

            words[j] = strdup(word);
            if (words[j] == NULL) {
                fprintf(stderr, "Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            j++;
        }
    }

    fclose(file);

    /* Trim the pointer array to the exact number of words read. */
    if (j > 0) {
        char **tmp = realloc(words, j * sizeof(char *));
        if (tmp != NULL) {
            words = tmp;
        }
    }

    *totalWords = j;
    return words;
}
