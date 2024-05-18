#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader.h"
#include "equivx.h"


int main(int argc, char** argv) {
    // Input arguments
    int word_length;
    int num_rounds = 7;
    char* filename = getFullPath("data/words.txt");
    // Check if arguments were provided
    if (argc > 1) {
        word_length = atoi(argv[1]);
    } else {
        word_length = 7; // Use default value if no argument is provided
        printf("No word length provided. Using default value of %d.\n", word_length);
    }
    if (argc > 2) {
        num_rounds = atoi(argv[2]);
    }
    if (argc > 3) {
        filename = argv[3];
    }
    // Print input arguments
    printf("Word length: %d\n", word_length);
    printf("Number of rounds: %d\n", num_rounds);
    printf("Path to dictionary: %s\n", filename);

    // Get user input letter
    char letter[num_rounds];
    // Initialize all elements to '_'
    memset(letter, '\0', sizeof(letter));

    // printf("Enter the letter to group words by: ");
    // scanf("%c", &letter[0]);
    // fflush(stdin); // Clearing input buffer
    letter[0] = 'e';
    letter[1] = 'r';

    int totalWords;
    char** words = readLinesFromFile(filename, &totalWords, word_length);

    int numGroups;
    EquivalenceClass* groups = generateEquivalenceClasses(words, totalWords, word_length, letter, &numGroups);

    // int numGroups = 0;
    // EquivalenceClass* groups = (EquivalenceClass*) malloc(sizeof(EquivalenceClass));
    // groups[0].code = (int*) calloc(word_length, sizeof(int));
    // for (int j = 0; j < word_length; j++) {
    //     if (words[0][j] == letter) {
    //         groups[0].code[j] = 1;
    //     }
    // }
    // groups[0].numInstances = 1;
    // numGroups++;

    // for (int i = 1; i < totalWords; i++) {
    //     int *code = generateCode(words[i], word_length, letter);
    //     if (code == NULL) {
    //         printf("Memory allocation failed.\n");
    //         exit(1);
    //     }
    //     updateEquivalenceClasses(&groups, &numGroups, code, word_length);
    // }
    // // Using qsort to sort the array of EquivalenceClass structs
    // qsort(groups, numGroups, sizeof(EquivalenceClass), compareEqClasses);

    // Print groups
    for (int i = 0; i < numGroups; i++) {
        printf("Group %d: ", i);
        for (int j = 0; j < word_length; j++) {
            printf("%d", groups[i].code[j]);
        }
        printf(" (%d)\n", groups[i].numInstances);
    }

    freeEquivalenceClasses(groups, numGroups);
    // Free allocated memory
    for (int i = 0; i < totalWords; i++) {
        free(words[i]);
    }
    free(words);
    return 0;
}

