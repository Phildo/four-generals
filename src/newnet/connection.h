#ifndef _FG_CONNECTION_H
#define _FG_CONNECTION_H

#include "network.h"
#include "circ_q.h"

namespace Network
{
  class Connection
  {
    private:
      String ip;
      String serv_ip;
      int port;
      ConnectionState con_state;

      circQ<Load, FG_LOAD_Q_SIZE> recv_q;
      circQ<Load, FG_LOAD_Q_SIZE> send_q;

      char buff[FG_BUFF_SIZE];
    public:
      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;

      Connection();
      ~Connection();

      void connect();
      void broadcast(const Load &l);
      bool read(Load &l);
      void disconnect();

      void *fork();
  };
}

#endif

