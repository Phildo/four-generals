#ifndef _FG_CLIENT_H
#define _FG_CLIENT_H

#include "network.h"
#include "circ_q.h"

namespace Network
{
  void * cliThreadHandoff(void * arg);

  class Client;
  struct CliThreadHandle { Client *client; };
  class Client
  {
    private:
      CliThreadHandle handle;
      pthread_t thread;

      int sock_fd;
      struct sockaddr_in serv_sock_addr; //client's serv addr
      struct hostent *serv_host; //client's reference to server

      String ip;
      String serv_ip;
      int port;

      circQ<Load, FG_LOAD_Q_SIZE> recv_q;
      circQ<Load, FG_LOAD_Q_SIZE> send_q;

      void tick();
      bool receiveLoad(Load *l);
      bool sendLoad(Load *l);
    public:
      ConnectionState con_state;
      char con_id;

      Client();
      ~Client();

      void connect(const String &_ip, int _port);
      void broadcast(const Load &l);
      bool read(Load &l);
      void disconnect();

      void *fork();
  };
}

#endif

