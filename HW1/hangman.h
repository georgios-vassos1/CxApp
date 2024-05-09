#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define ALPHABET_SIZE 26

void print_word(char *word, char *guessed_letters);
bool is_letter(char c);
bool is_already_guessed(char c, char *guessed_letters);
void play_game(char *word, const int num_rounds, int *player_wins, int *program_wins);

