#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
#include <strings.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "socket_helper.h"
#include "dictionary.h"

#define LISTENQ 4
#define MAXDATASIZE 4096

#define ALPHABET 26
#define NOGUESS 0x00000000000000000000000000

typedef struct player {
  int lives;
  int tries;
  int guesses[26];
  char * display;
} player_t;

typedef void sig_func(int);
char * MessageToDisplay(char * received);
char * start_game(char * word, player_t * player);
void init_display(char * display, int n);
int check_guess_and_update(char * display, char * word, player_t * player, char guess);
int check_if_won(player_t * player, char * word);
char * print_ui(char * display, int lives, int * guesses, char * message, int won);

sig_func * Signal(int signo, sig_func *func) {
  struct sigaction act, oact;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif

  } else {
#ifdef SA_RESTART
    act.sa_flags |= SA_RESTART;
#endif
  }
  if (sigaction(signo, &act, &oact) < 0) {
    return (SIG_ERR);
  }
  return (oact.sa_handler);
}

void sig_chld (int signo) {
  pid_t pid;
  int stat;
  while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
    printf("child %d terminated\n", pid);
  }
  return;
}

int main (int argc, char **argv) {
  int listenfd,
    connfd,
    port;
  struct sockaddr_in servaddr;
  char   error[MAXDATASIZE + 1];
  void sig_chld(int);
  char * word;
  player_t * player;

  if (argc != 2) {
    strcpy(error,"uso: ");
    strcat(error,argv[0]);
    strcat(error," <Port>");
    perror(error);
    exit(1);
  }

  port = atoi(argv[1]);

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  servaddr = ServerSockaddrIn(AF_INET, INADDR_ANY, port);

  Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);
  Signal(SIGCHLD, sig_chld);

  for ( ; ; ) {
    pid_t pid;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    if ((connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_len)) < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("accept error");
      }
    }
    if((pid = fork()) == 0) {
      int game_on = 0;
      int won = 0;
      char * message = NULL;
      char * response;
      char * buf = (char*) malloc(sizeof(char)*MAXDATASIZE);
      int n;

      Close(listenfd);
      response = "============================\nWelcome to the Hangman Game!\nPlease choose an option\n============================\n\n1) Play Single-player mode \n2) Be hangman on next match\n3) Play Multi-player Mode\n'-h' for our help menu";
      write(connfd, response, strlen(response));

      while((n = receive(connfd, buf, MAXDATASIZE))){
        if (n >= 0) {
          buf[n] = '\0';
        } else {
          strncpy(buf, "INVALID", 8);
        }
        if (strcmp(buf, "1") == 0 && !game_on && !won) {
          game_on = 1;
          player = (player_t *) malloc(sizeof(player_t));
          word = get_word("dictionary.txt", 0);

          message = malloc(sizeof(char)*256);
          bzero(message, 256);

          player->tries = 0;
          player->lives = 6;
          bzero(&player->guesses, 26);
          player->display = malloc(sizeof(char)*(2*strlen(word)-1));
          init_display(player->display, strlen(word));

          response = print_ui(player->display, player->lives - player->tries, player->guesses, message, 0);
          printf("word chosen: %s\n", word);
          write(connfd, response, strlen(response));
          continue;
        } else if (strcmp(buf,"-h") == 0 && !game_on && !won){
          response = "==========================\n        Game rules\n==========================\nAt the beggining of the game, a word is chosen by the Hangman.\nIt is your job to guess what is that word!\nYou will receive a fixed amount of tries and know how many letters the word has.\nYou can guess any letter of the alfabet.\nIf you guess a letter that is present in the word, you will play another round for free!\nThe hangman will also show you the place that the letter fits in the answer!\nIf you guess a wrong letter, the  hangman will charge you one of your tries, so be careful!\nIf you succeed in guessing the entire word before your tries run out, you win the game! :)\nBut, if you lose all your tries before guessing the entire word, you lose the game :(\n\nDeveloped with <3 by\nGiovani Garuffi Rondel\nDaniel Germano Travieso\n(Press '0' to go back to the Main Menu)\n";
          write(connfd, response, strlen(response));
        } else if (strcmp(buf,"0") == 0){
          response = "============================\nWelcome to the Hangman Game!\nPlease choose an option\n============================\n\n1) Play Single-player mode \n2) Be hangman on next match\n3) Play Multi-player Mode\n'-h' for our help menu";
          game_on = 0;
          won = 0;
          write(connfd, response, strlen(response));
        } else if (strcmp(buf,"2") == 0 && !game_on && !won){
          response = "Not implemented yet!";
          write(connfd, response, strlen(response));
        } else if (strcmp(buf,"3") == 0 && !game_on && !won){
          response = "Not implemented yet!";
          write(connfd, response, strlen(response));
        } else if (game_on && !won) {
          bzero(response, MAXDATASIZE);
          bzero(message, 256);
          int length = 0;
          char guess;

          guess = buf[0];
          if(strlen(buf)>1){
            sprintf(message, "You can't guess more than one letter at a time!");
            player->tries++;
          } else if ((guess >= 'A' && guess <= 'Z') || (guess >= 'a' && guess <= 'z')) {
            int r = check_guess_and_update(player->display, word, player, guess);
            if (r == -1) {
              sprintf(message, "You already guessed that one");
            } else {
              if (r == 0) {
                sprintf(message, "Letter not present in %s", player->display);
              } else {
                sprintf(message, "You guessed right: %s", player->display);
              }
            }
          } else {
            sprintf(message, "You can only guess letters from the alfabet!");
            player->tries++;
          }

          if(check_if_won(player, word)){
            won = 1;
            sprintf(message, "YOU WON! THE WORD WAS: %s", player->display);
          } else if (player->tries >= player->lives) {
            won = 0;
            sprintf(message, "SHAME, YOU LOST :( [the word was: '%s']", word);
            game_on = 0;
          }
          length += sprintf(response + length, "%s", print_ui(player->display, player->lives - player->tries, player->guesses, message, won));
          write(connfd, response, strlen(response));
        } else {
          response = "Not a valid play!\nPress '0' to exit to Main Menu\n";
          write(connfd, response, strlen(response));
        }
      }
      Close(connfd);
      free(buf);
      exit(0);
    }
  }
  Close(connfd);

  return(0);
}

void kill_handler(int sig_type) {
  printf("closing child process\n");
  exit(1);
}

char * print_ui(char * display, int lives, int * guesses, char * message, int won) {
  int i;
  int length = 0;
  char alfabet = 'A';
  char * response = NULL;

  response = malloc(sizeof(char)*MAXDATASIZE);

  if (message[0] != '\0') {
    length += sprintf(response+length, "===============================================\n");
    length += sprintf(response+length, "THE HANGMAN SAYS:\n%s\n===============================================\n", message);
    if(won){
      length += sprintf(response+length, "Press '0' to exit to Main Menu\n");
    }
    if(lives && !won){
      length += sprintf(response+length, "=============================================\n");
      length += sprintf(response+length, "+-----------------------------------+\n");
      length += sprintf(response+length, "|");
      for(i = 0; i < 26; i++){
        if(guesses[i]){
          length += sprintf(response+length, "(%c)|", alfabet);
        } else {
          length += sprintf(response+length, " %c |", alfabet);
        }
        alfabet++;
        if((i+1)%9 == 0){
          length += sprintf(response+length, "\n");
          length += sprintf(response+length, "+-----------------------------------+\n|");
        }
      }
      length += sprintf(response+length, "   |\n");
      length += sprintf(response+length, "+-----------------------------------+\n");
    }
  }
  if (lives > 0 && !won) {
    length += sprintf(response+length, "===========================\n");
    length += sprintf(response+length, "Welcome to the Hangman Game\n");
    length += sprintf(response+length, "===========================\n");
    length += sprintf(response+length, "The hangman has a word for you: ");
    length += sprintf(response+length, "%s\n", display);
    if(lives > 1){
      length += sprintf(response+length, "You have %d lifes remaining\n", lives);
    } else if (lives == 1){
      length += sprintf(response+length, "BE CAREFUL! You only have %d life remaining\n", lives);
    }
    length += sprintf(response+length, "Guess a letter ([a-z|A-Z]):\nPress '0' to exit to Main Menu");
  }

  return response;
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
