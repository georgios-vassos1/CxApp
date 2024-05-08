#include "reader.h"

#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define ALPHABET_SIZE 26

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

    printf("\n== ROUND %d/%d ==\n\n", cur_round + 1, num_rounds);

    while (cur_round < num_rounds) {
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

    printf("\nYou lost! The word is %c.\n", word[0]);
    (*program_wins)++;
}

int main (int argc, char** argv) {
    // Input arguments
    int word_length;
    int num_rounds = 7;
    char* file_name = "/Users/gva/Desktop/CHW/L11_Examples/words.txt";
    // Check if arguments were provided
    if (argc > 1) {
        word_length = atoi(argv[1]);
    } else {
        word_length = NO_WORD_LENGTH_CONDITION; // Use default value if no argument is provided
    }
    if (argc > 2) {
        num_rounds = atoi(argv[2]);
    }
    if (argc > 3) {
        file_name = argv[3];
    }
    // Print input arguments
    printf("Word length: %d\n", word_length);
    printf("Number of rounds: %d\n", num_rounds);
    printf("Path to dictionary: %s\n", file_name);

    int totalWords;
    char** words = readLinesFromFile(file_name, &totalWords, word_length);

    int player_wins = 0;
    int program_wins = 0;
    char play_again;

    srand(time(NULL));
    do {
        int idx = rand() % totalWords;
        char *word = words[idx];
        play_game(word, num_rounds, &player_wins, &program_wins);

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

