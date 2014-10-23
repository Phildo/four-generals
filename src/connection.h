#ifndef _FG_CONNECTION_H
#define _FG_CONNECTION_H

#include "network.h"
#include "event.h"
#include "circ_q.h"

namespace Network
{
  void * conThreadHandoff(void * arg);

  class Connection;
  struct ConThreadHandle { Connection *connection; };
  class Connection
  {
    private:
      int evt_id_inc;
      int nextEventId();
      void enqueueAckWait(Event e);
      void ackReceived(Event e);

      circQ<Event, FG_EVT_Q_SIZE> recv_q;
      circQ<Event, FG_EVT_Q_SIZE> send_q;
      circQ<Event, FG_EVT_Q_SIZE> ack_q;

      String ip;
      String serv_ip;
      int port;

      char buff[FG_BUFF_SIZE];
    public:
      bool keep_connection;
      bool connecting;
      bool connected;

      ConThreadHandle handle;
      pthread_t thread;

      //difference between connection and client
      char connection; //'1' thru '1'+(FG_MAX_CONNECTIONS-1) or '0' for none
      bool welcome;

      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;
      //end difference

      Connection();
      ~Connection();

      void broadcast(Event e);

      void disconnect();
      bool healthy();
      bool stale();

      Event *getEvent(); //aka dequeue

      void *fork();
  };
}

#endif

