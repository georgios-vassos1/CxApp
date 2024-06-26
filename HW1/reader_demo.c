#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Just for demonstration

#include "reader.h"


int main(int argc, char *argv[]) {
    int totalWords;
    int word_length;
    // Check if an argument was provided
    if (argc > 1) {
        word_length = atoi(argv[1]);
    } else {
        word_length = NO_WORD_LENGTH_CONDITION; // Use default value if no argument is provided
    }
    char* filename = getFullPath("data/words.txt");
    // Print file name
    printf("File name: %s\n", filename);
    char** words = readLinesFromFile(filename, &totalWords, word_length);

    // for (int i = 0; i < totalWords; i++) {
    //     printf("%s\n", words[i]);
    // }

    // Choose one word at random
    srand(time(NULL)); // Seed the random number generator
    int idx = rand() % totalWords;
    printf("Random word: %s\n", words[idx]);

    // Number of words read
    printf("Total words: %d\n", totalWords);

    for (int i = 0; i < totalWords; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}

