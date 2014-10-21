#ifndef _FG_CLIENT_H
#define _FG_CLIENT_H

#include "network.h"
#include "event.h"
#include "circ_q.h"

namespace Network
{
  void * cliThreadHandoff(void * arg);

  class Client;
  struct CliThreadHandle { Client *client; };
  class Client
  {
    private:
      bool keep_connection;
      bool connected;

      int evt_id_inc;
      int nextEventId();
      void enqueueAckWait(Event e);
      void ackReceived(int id);

      circQ<Event, FG_EVT_Q_SIZE> recv_q;
      circQ<Event, FG_EVT_Q_SIZE> send_q;
      circQ<Event, FG_EVT_Q_SIZE> ack_q;

      String ip;
      String serv_ip;
      int port;

      int sock_fd;
      pthread_t thread;
      struct sockaddr_in serv_sock_addr; //client's serv addr
      struct hostent *serv_host; //client's reference to server
      char buff[FG_BUFF_SIZE];
    public:
      CliThreadHandle handle;

      Client();
      ~Client();

      void connect(const String &_ip, int _port);
      void broadcast(Event e);
      void disconnect();
      bool healthy();

      Event *getEvent(); //aka dequeue

      void *fork();
  };
}

#endif

