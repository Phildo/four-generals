#ifndef _FG_NETWORK_H
#define _FG_NETWORK_H

#include "defines.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#ifdef FG_ANDROID
#include "third/android_ifaddrs.h" //android impl of ifaddrs
#else
#include <ifaddrs.h>
#endif

#include <fcntl.h>

extern "C"
{
  #include <pthread.h>
  #include <unistd.h>
}

#include "string.h"

#define FG_MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define FG_ACCEPT_Q_SIZE 5 //recommended (also, can't be more... I don't think...)
#define FG_LOAD_BUFF_SIZE 256
#define FG_LOAD_Q_SIZE 32

namespace Network
{
  enum ConnectionState
  {
    CONNECTION_STATE_DISCONNECTED,
    CONNECTION_STATE_CONNECTING,
    CONNECTION_STATE_CONNECTED,
    CONNECTION_STATE_DISCONNECTING,
    CONNECTION_STATE_STALE,
    CONNECTION_STATE_COUNT
  };

  struct Load
  {
    char data[FG_LOAD_BUFF_SIZE];
    char con_id;
  };

  struct Connection
  {
    private:
    public:
      Connection() : sock_fd(-1), sock_addr_len(sizeof(sock_addr)) {}

      char con_id;
      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;
  };

  class Server;
  class Client;

  String getIP();

  String decimalRep(int i, int digs);
  int intVal(char *s, int digs);
}

#endif

