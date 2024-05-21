#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "reader.h"
#include "equivx.h"
#include "hangman.h"

void print_word(char *word, char *guessed_letters) {
    for (int i = 0; i < strlen(word); i++) {
        if (strchr(guessed_letters, word[i]) != NULL) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

bool is_letter(char c) {
    return isalpha(c);
}

bool is_already_guessed(char c, char *guessed_letters) {
    return strchr(guessed_letters, c) != NULL;
}

void play_game_x(char **words, int totalWords, const int num_rounds, int *player_wins, int *program_wins) {
    char guessed_letters[num_rounds + 1];
    guessed_letters[0] = '\0';
    int cur_round = 0;
    int correct_guesses = 0;

    srand(time(NULL));
    int idx = rand() % totalWords;
    char *word = strdup(words[idx]); // Important: Create copy of word, not pointer to word

    // View solution (DEBUG)
    // printf("Word: %s\n", word);
    int word_length = strlen(word);

    int tmp_totalWords = totalWords;
    char **tmp_words = copyWords(words, totalWords);

    int numGroups;
    EquivalenceClass* groups = NULL;
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
        scanf(" %c", &guess);

        if (!is_letter(guess)) {
            printf("%c is not a letter.\n", guess);
            continue;
        }
 
        guess = tolower(guess);

        if (is_already_guessed(guess, guessed_letters)) {
            printf("You have already guessed %c.\n", guess);
            continue;
        }

        guessed_letters[cur_round] = guess;
        guessed_letters[cur_round + 1] = '\0';

        groups = generateEquivalenceClasses(tmp_words, tmp_totalWords, word_length, &guess, &numGroups);
        // Check if any element in group 1 is null
        for (int i = 0; i < groups[0].numInstances; i++) {
            if (groups[0].words[i] == NULL) {
                printf("Index %d is null\n", i);
                printf("Err: Memory allocation failed.\n");
            }
        }

        if (groups != NULL) {
            // Choose one word at random from the first group
            // int wordIdx = rand() % groups[1].numInstances;
            // strcpy(word, groups[1].words[wordIdx]);
        }

        if (strchr(word, guess) == NULL) {
            printf("Wrong guess! You have %d guesses left.\n", num_rounds - cur_round);
        } else {
            correct_guesses++;
            // freeWords(tmp_words, tmp_totalWords);
            // tmp_words = copyWords(groups[1].words, groups[1].numInstances);
            // tmp_totalWords = groups[1].numInstances;
        }

        // Free memory
        if (groups != NULL) {
            // printf("Freeing memory...\n");
            freeEquivalenceClasses(groups, numGroups);
        }

        if (correct_guesses == word_length) {
            printf("\nYou won! The word is %s.\n", word);
            (*player_wins)++;
            return;
        }

        cur_round++;
    }
    // free(word);

    printf("\nWord: ");
    print_word(word, guessed_letters);

    printf("\nYou lost! The word is %s.\n", word);
    (*program_wins)++;
}

