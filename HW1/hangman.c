#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "hangman.h"

void print_word(const char *word, const char *guessed_letters) {
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        if (strchr(guessed_letters, word[i]) != NULL) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

bool is_letter(char c) {
    return isalpha((unsigned char)c);
}

bool is_already_guessed(char c, const char *guessed_letters) {
    return strchr(guessed_letters, c) != NULL;
}

void play_game(const char *word, int num_rounds, int *player_wins, int *program_wins) {
    /* At most ALPHABET_SIZE distinct letters can ever be guessed. */
    char guessed_letters[ALPHABET_SIZE + 1];
    guessed_letters[0] = '\0';
    int guessed_count = 0;
    int cur_round = 0;

    while (cur_round < num_rounds) {
        printf("\n== ROUND %d/%d ==\n\n", cur_round + 1, num_rounds);
        printf("Used letters: %s\n", guessed_letters);

        char unused_letters[ALPHABET_SIZE + 1];
        int unused_count = 0;
        for (char c = 'a'; c <= 'z'; c++) {
            if (!is_already_guessed(c, guessed_letters)) {
                unused_letters[unused_count++] = c;
            }
        }
        unused_letters[unused_count] = '\0';
        printf("Unused letters: %s\n", unused_letters);

        printf("\nWord: ");
        print_word(word, guessed_letters);

        printf("\nGuess a letter: ");
        char guess;
        if (scanf(" %c", &guess) != 1) break;

        if (!is_letter(guess)) {
            printf("%c is not a letter.\n", guess);
            continue;
        }

        guess = tolower((unsigned char)guess);

        if (is_already_guessed(guess, guessed_letters)) {
            printf("You have already guessed %c.\n", guess);
            continue;
        }

        guessed_letters[guessed_count++] = guess;
        guessed_letters[guessed_count] = '\0';

        if (strchr(word, guess) == NULL) {
            printf("Wrong guess! You have %d guesses left.\n", num_rounds - cur_round - 1);
        }

        /*
         * Win condition: every letter in the word is now in guessed_letters.
         * Checking against strlen(word) was wrong for words with repeated
         * letters (e.g. "hello" has length 5 but only 4 unique letters).
         */
        bool all_revealed = true;
        for (int j = 0; word[j] != '\0'; j++) {
            if (strchr(guessed_letters, word[j]) == NULL) {
                all_revealed = false;
                break;
            }
        }
        if (all_revealed) {
            printf("\nYou won! The word is %s.\n", word);
            (*player_wins)++;
            return;
        }
        cur_round++;
    }

    printf("\nWord: ");
    print_word(word, guessed_letters);

    printf("\nYou lost! The word is %s.\n", word);
    (*program_wins)++;
}

