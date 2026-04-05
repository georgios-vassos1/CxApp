#ifndef EQUIVX_H
#define EQUIVX_H

typedef struct {
    int *code;
    int numInstances;
    char **words;
} EquivalenceClass;

int codesEqual(const int *code1, const int *code2, int length);
int compareEqClasses(const void *a, const void *b);
int *generateCode(const char *word, int word_length, const char *letter);
void updateEquivalenceClasses(EquivalenceClass **groups, int *numGroups,
                              int *code, int word_length, const char *word);
EquivalenceClass *generateEquivalenceClasses(char **words, int totalWords,
                                             int word_length, const char *letter,
                                             int *numGroups);
void freeEquivalenceClasses(EquivalenceClass *groups, int numGroups);
char **copyWords(char **words, int numWords);
void printWords(char **words, int numWords);
void freeWords(char **words, int numWords);

#endif // EQUIVX_H
