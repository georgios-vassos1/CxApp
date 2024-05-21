#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "reader.h"
#include "equivx.h"
#include "hangman.h"


int main (int argc, char** argv) {
    // Input arguments
    int word_length;
    int num_rounds = 7;
    char* filename = getFullPath("data/words.txt");
    // Check if arguments were provided
    if (argc > 1) {
        word_length = atoi(argv[1]);
    } else {
        // word_length = NO_WORD_LENGTH_CONDITION; // Use default value if no argument is provided
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

    int totalWords;
    char** words = readLinesFromFile(filename, &totalWords, word_length);

    int player_wins = 0;
    int program_wins = 0;
    char play_again;

    do {
        play_game_x(words, totalWords, num_rounds, &player_wins, &program_wins);

        printf("\n\nYOU: %d - ME: %d\n\n", player_wins, program_wins);
        printf("Play again? (y/n)\n");
        scanf(" %c", &play_again);
        
        while (tolower(play_again) != 'y' && tolower(play_again) != 'n') {
            printf("Invalid input. Play again? (y/n)\n");
            scanf(" %c", &play_again);
        }
    } while (tolower(play_again) == 'y');
    
    printf("Thank you for playing!\n");

    return 0;
}

