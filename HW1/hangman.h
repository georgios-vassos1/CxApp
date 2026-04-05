#ifndef HANGMAN_H
#define HANGMAN_H

#include <stdbool.h>

#define ALPHABET_SIZE 26

void print_word(const char *word, const char *guessed_letters);
bool is_letter(char c);
bool is_already_guessed(char c, const char *guessed_letters);
void play_game(const char *word, int num_rounds, int *player_wins, int *program_wins);

#endif // HANGMAN_H
