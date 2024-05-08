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
int* generateCode(char* word, int word_length, char* letter) {
    int* code = (int*) calloc(word_length, sizeof(int));
    for (int j = 0; j < word_length; j++) {
        int k = 0;
        while (letter[k] != '\0') {
            if (word[j] == letter[k]) {
                code[j] = k + 1;
            }
            k++;
        }
    }
    return code;
}

// Function to update equivalence classes
void updateEquivalenceClasses(EquivalenceClass** groups, int* numGroups, int* code, int word_length) {
    int found = 0;
    for (int j = 0; j < *numGroups; j++) {
        if (compareCodes((*groups)[j].code, code, word_length)) {
            (*groups)[j].numInstances++;
            found = 1;
            break;
        }
    }

    if (!found) {
        (*numGroups)++;
        *groups = (EquivalenceClass*) realloc(*groups, (*numGroups) * sizeof(EquivalenceClass));
        if (*groups == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        (*groups)[*numGroups - 1].numInstances = 1;
        (*groups)[*numGroups - 1].code = code;
    } else {
        free(code);
    }
}

// Function to generate equivalence classes for words
EquivalenceClass* generateEquivalenceClasses(char** words, int totalWords, int word_length, char* letter, int* numGroups) {
    *numGroups = 0;
    EquivalenceClass* groups = (EquivalenceClass*) malloc(sizeof(EquivalenceClass));
    groups[0].code = generateCode(words[0], word_length, letter);
    groups[0].numInstances = 1;
    (*numGroups)++;

    for (int i = 1; i < totalWords; i++) {
        int* code = generateCode(words[i], word_length, letter);
        if (code == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        updateEquivalenceClasses(&groups, numGroups, code, word_length);
    }

    qsort(groups, *numGroups, sizeof(EquivalenceClass), compareEqClasses);

    return groups;
}

// Function to free memory allocated for equivalence classes
void freeEquivalenceClasses(EquivalenceClass* groups, int numGroups) {
    for (int i = 0; i < numGroups; i++) {
        free(groups[i].code);
    }
    free(groups);
}

