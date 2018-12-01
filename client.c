#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#include "socket_helper.h"

#define MAXLINE 4096
#define EXIT_COMMAND "exit\n"


int main(int argc, char **argv) {
  int    port, sockfd;
  char * ip;
  char   error[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 3) {
    strcpy(error,"uso: ");
    strcat(error,argv[0]);
    strcat(error," <IPaddress, Port>");
    perror(error);
    exit(1);
  }


  ip = argv[1];
  port = atoi(argv[2]);

  while(1) {
    int n;
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    servaddr = ClientSockaddrIn(AF_INET, ip, port);
    Connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    char *buf = (char*) malloc(sizeof(char)*MAXLINE);
    bzero(buf, MAXLINE);
    while((n = receive(sockfd, buf, MAXLINE))){
      if (n >= 0) {
        buf[n] = '\0';
      } else {
        strncpy(buf, "INVALID", 8);
      }
      printf("%s\n", buf);
      char opt[MAXLINE];
      if (!fgets(opt, MAXLINE-1, stdin)) {
        perror("fgets");
        Send(sockfd, "", strlen(""));
      } else {
        opt[strcspn(opt, "\n")] = '\0';
        Send(sockfd, opt, strlen(opt));
      }
    }
    free(buf);
    Close(sockfd);
  }
  exit(0);
}
