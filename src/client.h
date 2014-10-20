#ifndef _FG_CLIENT_H
#define _FG_CLIENT_H

#include "network.h"
#include "event.h"

namespace Network
{
  void * cliThreadHandoff(void * arg);

  class Client;
  struct CliThreadHandle { Client *client; };
  class Client
  {
    private:
      CliThreadHandle handle;

      String ip;
      String serv_ip;
      int port;

      bool keep_connection;
      bool connected;

      int sock_fd;
      pthread_t thread;
      struct sockaddr_in serv_sock_addr; //client's serv addr
      struct hostent *serv_host; //client's reference to server
      char read[FG_BUFF_SIZE]; int readlen;
      char writ[FG_BUFF_SIZE]; int writlen;

      void enqueueEvent(Event);
      Event evt_q[FG_EVT_Q_SIZE];
      int evt_q_front;
      int evt_q_back;
    public:
      Client();
      ~Client();

      void connect(const String &_ip, int _port);
      void broadcast(const String &s);
      void disconnect();
      bool healthy();

      Event *getEvent(); //aka dequeue

      void *fork();
  };
}

#endif

