#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern "C"
{
  #include <pthread.h>
  #include <unistd.h>
}

#define MAX_IP_LENGTH 256 //really only need 12, but might as well leave it for urls?

#define MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define BUFF_SIZE 256

struct Connection
{
  int connection; //0-MAX_CONNECTIONS
  bool stale;

  int sock_fd;
  socklen_t sock_addr_len; //sizeof addr
  struct sockaddr_in sock_addr;

  pthread_t thread;
  char read_buff[BUFF_SIZE];
  char write_buff[BUFF_SIZE];
};

namespace Network
{
  void connectAsServer();
  void connectAsClient();
  void broadcast(char *c, int l);
  void disconnect();
}

#endif

