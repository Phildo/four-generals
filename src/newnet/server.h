#ifndef _FG_SERVER_H
#define _FG_SERVER_H

#include "network.h"
#include "connection.h"
#include "circ_q.h"

namespace Network
{
  void * servThreadHandoff(void * arg);

  class Server;
  struct ServThreadHandle { Server *server; };
  class Server
  {
    private:
      ServThreadHandle handle;
      pthread_t thread;

      String ip;
      int port;
      ConnectionState con_state;

      int n_cons;
      Connection cons[FG_MAX_CONNECTIONS];
      Connection *con_ptrs[FG_MAX_CONNECTIONS];
    public:
      Server();
      ~Server();

      void connect(int _port);
      void broadcast(const Load &l);
      bool read(Load &l);
      void disconnect();

      void *fork();
  };
}

#endif

