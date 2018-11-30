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

#define LISTENQ 4
#define MAXDATASIZE 4096

typedef void sig_func(int);
const char * MessageToDisplay(char * recieved);

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
  int fd[2];

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

  char * readbuf = (char*) malloc(sizeof(char)*MAXDATASIZE);
  for ( ; ; ) {
    pid_t pid;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    pipe(fd);
    if ((connfd = Accept(listenfd, (struct sockaddr *) &clientaddr, &clientaddr_len)) < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        perror("accept error");
      }
    }
    if((pid = fork()) == 0) {
      Close(listenfd);
      close(fd[0]);

      const char * msg = MessageToDisplay(readbuf);
      Send(connfd, msg, strlen(msg));

      char * buf = (char*) malloc(sizeof(char)*MAXDATASIZE);
      bzero(buf, MAXDATASIZE);
      int n = receive(connfd, buf, MAXDATASIZE);
      if (n >= 0) {
        buf[n] = '\0';
      } else {
        strncpy(buf, "INVALID", 8);
      }
      write(fd[1], buf, strlen(buf)+1);
      close(fd[1]);
      Close(connfd);
      free(buf);
      exit(0);
    } else {
      close(fd[1]);
      if (read(fd[0], readbuf, sizeof(readbuf)) != -1) {
        printf("%s\n", readbuf);
      }
      close(fd[0]);
    }

    Close(connfd);
  }

  return(0);
}

void kill_handler(int sig_type) {
  printf("closing child process\n");
  exit(1);
}

const char * MessageToDisplay(char * recieved) {
  if (strcmp(recieved, "") == 0) {
    return "Welcome to the Hangman Game! Choose an option\n-------\n1) Play Single-player mode \n2) Be hangman on next match\n3) Play Multi-player Mode\n";
  } else if (strcmp(recieved, "1") == 0) {
    return "Starting single-player game...\n";
  }
  return "PARSE ERROR";
}
