#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reader.h"


char* getFullPath(const char* filename) {
    // Allocate memory for the full path
    char* full_path = (char*)malloc(MAX_PATH_LENGTH * sizeof(char));
    if (full_path == NULL) {
        perror("Error: Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Get the current working directory
    if (getcwd(full_path, MAX_PATH_LENGTH) == NULL) {
        perror("Error: Unable to get current working directory");
        exit(EXIT_FAILURE);
    }

    // Concatenate cwd with the filename
    strcat(full_path, "/");
    strcat(full_path, filename);

    return full_path;
}

char** readLinesFromFile(const char* filename, int* totalWords, const int wordLength) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    char** words = (char**) malloc(CHUNK_SIZE * sizeof(char*));
    if (words == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }

    int j = 0;
    int wordsAllocated = CHUNK_SIZE;

    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF) {
        if (wordLength == NO_WORD_LENGTH_CONDITION || strlen(word) == wordLength) {
            if (j == wordsAllocated) {
                wordsAllocated += CHUNK_SIZE;
                words = (char**) realloc(words, wordsAllocated * sizeof(char*));
                if (words == NULL) {
                    printf("Error allocating memory\n");
                    exit(1);
                }
            }

            words[j] = (char*) malloc(MAX_WORD_LENGTH * sizeof(char));
            if (words[j] == NULL) {
                printf("Error allocating memory\n");
                exit(1);
            }

            strcpy(words[j], word);
            j++;
        }
    }

    fclose(file);

    *totalWords = j;
    return words;
}

