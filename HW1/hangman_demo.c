#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include "reader.h"
#include "hangman.h"

/*
 * Parse a positive integer from str, up to max_val inclusive.
 * Returns the parsed value, or -1 on any error.
 */
static int parse_positive_int(const char *str, int max_val) {
    char *endptr;
    errno = 0;
    long val = strtol(str, &endptr, 10);
    if (errno != 0 || *endptr != '\0' || val <= 0 || val > max_val) {
        return -1;
    }
    return (int)val;
}

int main(int argc, char **argv) {
    int word_length = NO_WORD_LENGTH_CONDITION;
    int num_rounds = 7;
    char *heap_path = getFullPath("data/words.txt");
    char *filename = heap_path;

    if (argc > 1) {
        word_length = parse_positive_int(argv[1], MAX_WORD_LENGTH - 1);
        if (word_length < 0) {
            fprintf(stderr, "Error: invalid word length '%s'\n", argv[1]);
            free(heap_path);
            return 1;
        }
    }
    if (argc > 2) {
        num_rounds = parse_positive_int(argv[2], ALPHABET_SIZE);
        if (num_rounds < 0) {
            fprintf(stderr, "Error: invalid number of rounds '%s'\n", argv[2]);
            free(heap_path);
            return 1;
        }
    }
    if (argc > 3) {
        filename = argv[3];
    }

    if (word_length == NO_WORD_LENGTH_CONDITION) {
        printf("Word length: any\n");
    } else {
        printf("Word length: %d\n", word_length);
    }
    printf("Number of rounds: %d\n", num_rounds);
    printf("Path to dictionary: %s\n", filename);

    int totalWords;
    char **words = readLinesFromFile(filename, &totalWords, word_length);
    if (totalWords == 0) {
        fprintf(stderr, "Error: no words found in %s\n", filename);
        free(words);
        free(heap_path);
        return 1;
    }

    int player_wins = 0;
    int program_wins = 0;
    char play_again;

    srand(time(NULL));
    do {
        int idx = rand() % totalWords;
        play_game(words[idx], num_rounds, &player_wins, &program_wins);

        printf("\n\nYOU: %d - ME: %d\n\n", player_wins, program_wins);
        printf("Play again? (y/n)\n");
        if (scanf(" %c", &play_again) != 1) play_again = 'n';

        while (tolower((unsigned char)play_again) != 'y' &&
               tolower((unsigned char)play_again) != 'n') {
            printf("Invalid input. Play again? (y/n)\n");
            if (scanf(" %c", &play_again) != 1) { play_again = 'n'; break; }
        }
    } while (tolower((unsigned char)play_again) == 'y');

    printf("Thank you for playing!\n");

    for (int i = 0; i < totalWords; i++) {
        free(words[i]);
    }
    free(words);
    free(heap_path);

    return 0;
}
