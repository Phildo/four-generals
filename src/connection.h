#ifndef _FG_CONNECTION_H
#define _FG_CONNECTION_H

#include "network.h"
#include "event.h"

namespace Network
{
  void * conThreadHandoff(void * arg);

  class Connection;
  struct ConThreadHandle { Connection *connection; };
  class Connection
  {
    private:
      bool keep_connection;
      bool connected;

      int evt_id_inc;
      int nextEventId();
      void enqueueAckWait(Event e);
      void ackReceived(int id);
      Event ack_q[FG_EVT_Q_SIZE];
      int ack_q_front;
      int ack_q_back;
    public:
      ConThreadHandle handle;

      char connection; //'1' thru '1'+(FG_MAX_CONNECTIONS-1) or '0' for none
      bool stale;
      bool welcome;

      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;

      pthread_t thread;
      char read[FG_BUFF_SIZE]; int readlen;
      char writ[FG_BUFF_SIZE]; int writlen;

      Connection();
      ~Connection();

      void disconnect();
      bool healthy();

      void *fork();
  };
}

#endif

