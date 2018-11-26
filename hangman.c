#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

#define ALPHABET 26
#define NOGUESS 0x00000000000000000000000000

typedef struct player {
  int lives;
  int tries;
  int guesses[26];
} player_t;

int main() {
  char * word = NULL;
  player_t * player = (player_t *) malloc(sizeof(player_t));

  word = get_word("dictionary.txt", 0);

  player->lives = 6;
  player->tries = 0;
  bzero(&player->guesses, 26);

  printf("Welcome to the Hangman Game\n");
  printf("I have chosen a word for you: ");

  char * display = malloc(sizeof(char)*(2*strlen(word)-1));
  for (int i=0; i<strlen(word); i++) {
    display[2*i] = '_';
    if (i != strlen(word)-1) {
      display[2*i+1] = '.';
    }

  }
  display[2*strlen(word)-1] = '\0';

  printf(" %s\n", display);
  printf("Guess a letter: ");
  char guess;
  scanf("%c", &guess);
  printf("%c", guess);
  if ((guess >= 'A' && guess <= 'Z') || (guess >= 'a' && guess <= 'z')) {
    int already = 0;
    printf("\nYou guessed %c\n", guess);
    if (guess < 'a') {
      if (player->guesses[guess-'A'] == 1) {
        printf("You already guessed that one\n");
        already = 1;
      } else {
        player->guesses[guess-'A'] = 1;
      }
    } else {
      if (player->guesses[guess-'a'] == 1) {
        printf("You already guessed that one\n");
        already = 1;
      } else {
        player->guesses[guess-'a'] = 1;
      }
    }
    if (!already) {
      int found = 0;
      for (int i=0; i<strlen(word); i++) {
        if (word[i] == guess) {
          display[2*i] = guess;
          found = 1;
        }
      }
      if (!found) {
        printf("Letter not present\n");
      } else {
        printf("You guessed right\n");
      }
      printf("%s\n", display);
    }
  } else {
    printf("\nGuess a letter ([a-zA-Z])\n");
  }
  return 0;
}
