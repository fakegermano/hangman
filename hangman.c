#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "dictionary.h"

#define ALPHABET 26
#define NOGUESS 0x00000000000000000000000000
#define clear() printf("\033[H\033[J")

typedef struct player {
  int lives;
  int tries;
  int guesses[26];
} player_t;

void init_display(char * display, int n);
int check_guess_and_update(char * display, char * word, player_t * player, char guess);
int check_if_won(player_t * player, char * word);
void print_ui(char * display, int lives, int * guesses, char * message, int won);

int main() {
  char * word = NULL;
  player_t * player = (player_t *) malloc(sizeof(player_t));
  char * message = NULL;
  word = get_word("dictionary.txt", 0);

  player->lives = 6;
  player->tries = 0;
  bzero(&player->guesses, 26);

  message = malloc(sizeof(char)*256);
  bzero(message, 256);

  char * display = malloc(sizeof(char)*(2*strlen(word)-1));
  init_display(display, strlen(word));
  while ((player->tries <= player->lives-1) && !check_if_won(player, word)) {
    char guess;
    print_ui(display, player->lives - player->tries, player->guesses, message, 0);
    scanf("%c", &guess);
    if ((guess >= 'A' && guess <= 'Z') || (guess >= 'a' && guess <= 'z')) {
      int r = check_guess_and_update(display, word, player, guess);
      bzero(message, 256);
      if (r == -1) {
        sprintf(message, "You already guessed that one");
      } else {
        if (r == 0) {
          sprintf(message, "Letter not present in %s", display);
        } else {
          sprintf(message, "You guessed right: %s", display);
        }
      }
    }
  }
  int won = 0;
  if (player->tries < player->lives) {
    won = 1;
    sprintf(message, "YOU WON! THE WORD WAS: %s", display);
  } else {
    won = 0;
    sprintf(message, "SHAME, YOU LOST :( [the word was: '%s']", word);
  }
  print_ui(display, player->lives - player->tries, player->guesses, message, won);
  return 0;
}

void print_ui(char * display, int lives, int * guesses, char * message, int won) {
  int i;
  char alfabet = 'A';
  clear();
  if (message[0] != '\0') {
    printf("======================\n");
    printf("THE HANGMAN SAYS: %s\n", message);
    printf("======================\n");
    if(lives && !won){
      printf("+-----------------------------------+\n");
      printf("|");
      for(i = 0; i < 26; i++){
        if(guesses[i]){
          printf("(%c)|", alfabet);
        } else {
          printf(" %c |", alfabet);
        }
        alfabet++;
        if((i+1)%9 == 0){
          printf("\n");
          printf("+-----------------------------------+\n|");
        }
      }
      printf("   |\n");
      printf("+-----------------------------------+\n");
    }
  }
  if (lives > 0 && !won) {
    printf("Welcome to the Hangman Game\n");
    printf("The hangman has a word for you: ");
    printf("%s\n", display);
    if(lives > 1){
      printf("You have %d lifes remaining\n", lives);
    } else if (lives == 1){
      printf("BE CAREFUL! You only have %d life remaining\n", lives);
    }
    printf("Guess a letter ([a-zA-Z]): \n");
  }
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
