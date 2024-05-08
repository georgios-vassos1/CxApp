#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 21
#define CHUNK_SIZE  20

typedef struct EquivalenceClass {
    char representative[MAX_WORD_LENGTH];
    int count;
    char **members;
} EquivalenceClass;

void add_word(EquivalenceClass *x, char *word) {
    x->members = (char **)realloc(x->members, (x->count + 1) * sizeof(char *));
    x->members[x->count] = strdup(word);
    x->count++;
}

void set_representative(EquivalenceClass *x, char *representative) {
    strcpy(x->representative, representative);
}

EquivalenceClass *create_equivalence_class() {
    EquivalenceClass *x = (EquivalenceClass *)malloc(sizeof(EquivalenceClass));
    x->members = NULL;
    x->count = 0;
    return x;
}

EquivalenceClass **group_words(char letter, int *word_count) {
    EquivalenceClass **groups = (EquivalenceClass **)malloc(CHUNK_SIZE * sizeof(EquivalenceClass *));
    char word[MAX_WORD_LENGTH];
    *word_count = 0;

    printf("Enter words (type 'end' to finish):\n");

    while (1) {
        scanf("%s", word);

        if (strcmp(word, "end") == 0) {
            break;
        }

        int len = strlen(word);
        EquivalenceClass *group = create_equivalence_class();

        for (int j = 0; j < len; j++) {
            if (word[j] == letter) {
                group->representative[j] = letter;
            } else {
                group->representative[j] = '_';
            }
        }
        group->representative[len] = '\0';

        set_representative(group, group->representative);
        add_word(group, word);
        groups[*word_count] = group;
        (*word_count)++;
    }

    return groups;
}

void print_equivalence_classes(EquivalenceClass **equivalence_classes, int word_count) {
    for (int i = 0; i < word_count; i++) {
        EquivalenceClass *equivalence_class = equivalence_classes[i];
        printf("Representative: %s\n", equivalence_class->representative);
        printf("Count: %d\n", equivalence_class->count);
        printf("Words: ");
        for (int j = 0; j < equivalence_class->count; j++) {
            printf("%s", equivalence_class->members[j]);
            if (j < equivalence_class->count - 1) {
                printf(", ");
            }
        }
        printf("\n\n");
    }
}

int main() {
    int count;
    char letter;

    printf("Enter the letter to group words by: ");
    scanf("%c", &letter);
    fflush(stdin); // Clearing input buffer

    // Print the size of the equivalence class object sizeof(EquivalenceClass)
    // printf("Size of EquivalenceClass: %lu\n", sizeof(EquivalenceClass));

    EquivalenceClass **equivalence_classes = group_words(letter, &count);
    print_equivalence_classes(equivalence_classes, count);

    // Free memory
    for (int i = 0; i < count; i++) {
        free(equivalence_classes[i]->members);
        free(equivalence_classes[i]);
    }
    free(equivalence_classes);

    return 0;
}

