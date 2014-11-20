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
#define FG_BUFF_SIZE 256
#define FG_LOAD_BUFF_SIZE 256
#define FG_LOAD_Q_SIZE 32

namespace Network
{
  enum ConnectionState;
  struct Load;
  class Server;
  class Connection;
  class Client;

  String getIP();
}

#endif

