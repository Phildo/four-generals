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
#define FG_EVT_Q_SIZE 16
#define FG_EVT_HIST_SIZE 1024
#define FG_EVT_MAX_DEC_LEN 6

namespace Network
{
  class Server;
  class Connection;
  class Client;
  struct Event;

  extern bool host_priv;
  String getIP();
}

#endif

