#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "equivx.h"
#include "hangman.h"
#include "hangmanx.h"

void play_game_x(char **words, int totalWords, int num_rounds,
                 int *player_wins, int *program_wins) {
    if (totalWords <= 0) {
        fprintf(stderr, "Error: no words available to play.\n");
        return;
    }
    int word_length = strlen(words[0]);

    /*
     * Pattern revealed to the player.
     * Positions start as '_' and are filled in as letters are correctly guessed.
     */
    char *pattern = malloc(word_length + 1);
    if (pattern == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    memset(pattern, '_', word_length);
    pattern[word_length] = '\0';

    /* At most ALPHABET_SIZE distinct letters can be guessed. */
    char guessed_letters[ALPHABET_SIZE + 1];
    guessed_letters[0] = '\0';
    int guessed_count = 0;

    /*
     * Working candidate set — an owned copy so we can freely
     * replace it each round with the winning equivalence class.
     */
    int tmp_totalWords = totalWords;
    char **tmp_words = copyWords(words, totalWords);

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
        for (int i = 0; i < word_length; i++) {
            printf("%c ", pattern[i]);
        }
        printf("\n");

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

        /*
         * generateEquivalenceClasses expects a null-terminated string,
         * not a bare char pointer — pass a two-byte array.
         */
        char guess_str[2] = {guess, '\0'};

        int numGroups;
        EquivalenceClass *groups = generateEquivalenceClasses(
            tmp_words, tmp_totalWords, word_length, guess_str, &numGroups);

        /*
         * groups[0] is the largest equivalence class (sorted by numInstances
         * descending).  Reveal positions where the guessed letter appears
         * according to that class's code.
         */
        bool letter_found = false;
        for (int j = 0; j < word_length; j++) {
            if (groups[0].code[j] != 0) {
                pattern[j] = guess;
                letter_found = true;
            }
        }

        /* Narrow the candidate set to the largest equivalence class. */
        freeWords(tmp_words, tmp_totalWords);
        tmp_words = copyWords(groups[0].words, groups[0].numInstances);
        tmp_totalWords = groups[0].numInstances;

        freeEquivalenceClasses(groups, numGroups);

        if (!letter_found) {
            printf("Wrong guess! You have %d guesses left.\n",
                   num_rounds - cur_round - 1);
        }

        /* Win condition: no underscores remain in the pattern. */
        bool won = true;
        for (int j = 0; j < word_length; j++) {
            if (pattern[j] == '_') {
                won = false;
                break;
            }
        }
        if (won) {
            printf("\nYou won! The word is %s.\n", tmp_words[0]);
            (*player_wins)++;
            freeWords(tmp_words, tmp_totalWords);
            free(pattern);
            return;
        }

        cur_round++;
    }

    /* Player lost — show partial pattern and reveal a candidate word. */
    printf("\nWord: ");
    for (int i = 0; i < word_length; i++) {
        printf("%c ", pattern[i]);
    }
    printf("\n");

    printf("\nYou lost! The word is %s.\n", tmp_words[0]);
    (*program_wins)++;
    freeWords(tmp_words, tmp_totalWords);
    free(pattern);
}
