#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "reader.h"
#include "equivx.h"


int compareCodes(int* code1, int* code2, int length) {
    for (int i = 0; i < length; i++) {
        if (code1[i] != code2[i]) {
            return 0;
        }
    }
    return 1;
}

// Comparison function for sorting EquivalenceClass structs by numInstances
int compareEqClasses(const void *a, const void *b) {
    const EquivalenceClass *x1 = (const EquivalenceClass *)a;
    const EquivalenceClass *x2 = (const EquivalenceClass *)b;
    return (x2->numInstances - x1->numInstances);
}

// Function to generate code for a word based on a letter
int* generateCode(const char* word, int word_length, const char* letter) {
    int* code = (int*) calloc(word_length, sizeof(int));
    if (code == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int j = 0; j < word_length; j++) {
        for (int k = 0; letter[k] != '\0'; k++) {
            if (word[j] == letter[k]) {
                code[j] = k + 1;
                break;
            }
        }
    }
    return code;
}

// Function to update equivalence classes
void updateEquivalenceClasses(EquivalenceClass** groups, int* numGroups, int* code, const char* word) {
    int word_length = strlen(word);

    for (int j = 0; j < *numGroups; j++) {
        if (compareCodes((*groups)[j].code, code, word_length)) {
            (*groups)[j].numInstances++;
            (*groups)[j].words = (char**) realloc((*groups)[j].words, (*groups)[j].numInstances * sizeof(char*));
            if ((*groups)[j].words == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            (*groups)[j].words[(*groups)[j].numInstances - 1] = strdup(word);
            if ((*groups)[j].words[(*groups)[j].numInstances - 1] == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            free(code);
            return;
        }
    }

    (*numGroups)++;
    *groups = (EquivalenceClass*) realloc(*groups, (*numGroups) * sizeof(EquivalenceClass));
    if (*groups == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    (*groups)[*numGroups - 1].code = code;
    (*groups)[*numGroups - 1].numInstances = 1;
    (*groups)[*numGroups - 1].words = (char**) malloc(sizeof(char*));
    if ((*groups)[*numGroups - 1].words == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    (*groups)[*numGroups - 1].words[0] = strdup(word);
    if ((*groups)[*numGroups - 1].words[0] == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

// Function to generate equivalence classes for words
EquivalenceClass* generateEquivalenceClasses(char** words, int totalWords, int word_length, const char* letter, int* numGroups) {
    *numGroups = 0;
    EquivalenceClass* groups = NULL;

    for (int i = 0; i < totalWords; i++) {
        int* code = generateCode(words[i], word_length, letter);
        updateEquivalenceClasses(&groups, numGroups, code, words[i]);
    }

    qsort(groups, *numGroups, sizeof(EquivalenceClass), compareEqClasses);

    return groups;
}

void printWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        printf("%s\n", words[i]);
    }
}

// Function to copy strings from one group to a temporary array
char** copyWords(char **words, int numWords) {
    char **tmp = (char**)malloc(numWords * sizeof(char*));
    if (tmp == NULL) {
        perror("Failed to allocate temporary array");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numWords; i++) {
        tmp[i] = strdup(words[i]);
        if (tmp[i] == NULL) {
            perror("Failed to duplicate string");
            // Free already allocated strings
            for (int j = 0; j < i; j++) {
                free(tmp[j]);
            }
            free(tmp);
            exit(EXIT_FAILURE);
        }
    }
    return tmp;
}

// Function to free vector of words
void freeWords(char** words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

// Function to free memory allocated for equivalence classes
void freeEquivalenceClasses(EquivalenceClass* groups, int numGroups) {
    for (int i = 0; i < numGroups; i++) {
        free(groups[i].code);
        for (int j = 0; j < groups[i].numInstances; j++) {
            free(groups[i].words[j]);
        }
        free(groups[i].words);
    }
    free(groups);
}

