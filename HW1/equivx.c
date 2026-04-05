#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equivx.h"

int codesEqual(const int *code1, const int *code2, int length) {
    for (int i = 0; i < length; i++) {
        if (code1[i] != code2[i]) {
            return 0;
        }
    }
    return 1;
}

int compareEqClasses(const void *a, const void *b) {
    const EquivalenceClass *x1 = (const EquivalenceClass *)a;
    const EquivalenceClass *x2 = (const EquivalenceClass *)b;
    if (x2->numInstances > x1->numInstances) return  1;
    if (x2->numInstances < x1->numInstances) return -1;
    return 0;
}

int *generateCode(const char *word, int word_length, const char *letter) {
    int *code = calloc(word_length, sizeof(int));
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

void updateEquivalenceClasses(EquivalenceClass **groups, int *numGroups,
                              int *code, int word_length, const char *word) {
    for (int j = 0; j < *numGroups; j++) {
        if (codesEqual((*groups)[j].code, code, word_length)) {
            (*groups)[j].numInstances++;
            char **tmp_words = realloc((*groups)[j].words,
                                       (*groups)[j].numInstances * sizeof(char *));
            if (tmp_words == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            (*groups)[j].words = tmp_words;
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
    EquivalenceClass *tmp_groups = realloc(*groups,
                                           (*numGroups) * sizeof(EquivalenceClass));
    if (tmp_groups == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    *groups = tmp_groups;
    (*groups)[*numGroups - 1].code = code;
    (*groups)[*numGroups - 1].numInstances = 1;
    (*groups)[*numGroups - 1].words = malloc(sizeof(char *));
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

EquivalenceClass *generateEquivalenceClasses(char **words, int totalWords,
                                             int word_length, const char *letter,
                                             int *numGroups) {
    *numGroups = 0;
    EquivalenceClass *groups = NULL;

    for (int i = 0; i < totalWords; i++) {
        int *code = generateCode(words[i], word_length, letter);
        updateEquivalenceClasses(&groups, numGroups, code, word_length, words[i]);
    }

    qsort(groups, *numGroups, sizeof(EquivalenceClass), compareEqClasses);

    return groups;
}

void printWords(char **words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        printf("%s\n", words[i]);
    }
}

char **copyWords(char **words, int numWords) {
    char **tmp = malloc(numWords * sizeof(char *));
    if (tmp == NULL) {
        perror("Failed to allocate temporary array");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < numWords; i++) {
        tmp[i] = strdup(words[i]);
        if (tmp[i] == NULL) {
            perror("Failed to duplicate string");
            for (int j = 0; j < i; j++) {
                free(tmp[j]);
            }
            free(tmp);
            exit(EXIT_FAILURE);
        }
    }
    return tmp;
}

void freeWords(char **words, int numWords) {
    for (int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

void freeEquivalenceClasses(EquivalenceClass *groups, int numGroups) {
    for (int i = 0; i < numGroups; i++) {
        free(groups[i].code);
        for (int j = 0; j < groups[i].numInstances; j++) {
            free(groups[i].words[j]);
        }
        free(groups[i].words);
    }
    free(groups);
}
