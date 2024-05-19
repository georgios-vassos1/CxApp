#ifndef EQUIVX_H
#define EQUIVX_H

// Define EquivalenceClass struct
typedef struct {
    int* code;
    int numInstances;
    char** words;
} EquivalenceClass;

// Function prototypes
int compareCodes(int* code1, int* code2, int length);
int compareEqClasses(const void *a, const void *b);
int* generateCode(char* word, int word_length, char* letter);
void updateEquivalenceClasses(EquivalenceClass** groups, int* numGroups, int* code, char* word);
EquivalenceClass* generateEquivalenceClasses(char** words, int totalWords, int word_length, char* letter, int* numGroups);
void freeEquivalenceClasses(EquivalenceClass* groups, int numGroups);

#endif // EQUIVX_H
