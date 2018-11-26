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

void init_display(char * display, int n);
int check_guess_and_update(char * display, char * word, player_t * player, char guess);
int check_if_won(player_t * player, char * word);

int main() {
  char * word = NULL;
  player_t * player = (player_t *) malloc(sizeof(player_t));

  word = get_word("../dictionary.txt", 0);

  player->lives = 6;
  player->tries = 0;
  bzero(&player->guesses, 26);

  printf("Welcome to the Hangman Game\n");
  printf("I have chosen a word for you: ");
  char * display = malloc(sizeof(char)*(2*strlen(word)-1));
  init_display(display, strlen(word));
  while ((player->tries <= player->lives-1) && !check_if_won(player, word)) {
    printf("%s\n", display);
    printf("(%d/%d) Guess a letter ([a-zA-Z]): ", player->tries, player->lives);
    char guess;
    scanf("%c", &guess);
    if ((guess >= 'A' && guess <= 'Z') || (guess >= 'a' && guess <= 'z')) {
      int r = check_guess_and_update(display, word, player, guess);
      if (r == -1) {
        printf("You already guessed that one\n");
      } else {
        if (r == 0) {
          printf("Letter not present in ");
        } else {
          printf("You guessed right: ");
        }
      }
    }
  }
  if (player->tries < player->lives) {
    printf("YOU WON! THE WORD WAS: %s\n", display);
  } else {
    printf("SHAME, YOU LOST :(the word was: '%s')\n", word);
  }
  return 0;
}

void init_display(char * display, int n) {
  for (int i=0; i<n; i++) {
    display[2*i] = '_';
    if (i != n-1) {
      display[2*i+1] = '.';
    }
  }
  display[2*n-1] = '\0';
}

int check_guess_and_update(char * display, char * word, player_t * player, char guess) {
  int already = 0;
  printf("You guessed: '%c'\n", guess);
  if (guess < 'a') {
    if (player->guesses[guess-'A'] == 1) {
      already = 1;
    } else {
      player->guesses[guess-'A'] = 1;
    }
  } else {
    if (player->guesses[guess-'a'] == 1) {
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
      (player->tries)++;
      return 0;
    } else {
      return 1;
    }
  }
  return -1;
}

int check_if_won(player_t * player, char * word) {
  int n = 0;
  for(int i=0; i<strlen(word); i++) {
    if(player->guesses[word[i]-'a']) {
      n++;
    }
  }
  if (n == strlen(word)) {
    return 1;
  }
  return 0;
}
