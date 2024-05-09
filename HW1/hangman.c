#include "reader.h"
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

void play_game(char *word, const int num_rounds, int *player_wins, int *program_wins) {
    char guessed_letters[num_rounds + 1];
    guessed_letters[0] = '\0';
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
        
        if (strchr(word, guess) == NULL) {
            printf("Wrong guess! You have %d guesses left.\n", num_rounds - cur_round);
        }

        if (strcmp(word, guessed_letters) == 0) {
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

