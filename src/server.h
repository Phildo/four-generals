#ifndef _FG_SERVER_H
#define _FG_SERVER_H

#include "network.h"
#include "connection.h"

namespace Network
{
  void * servThreadHandoff(void * arg);

  class Server;
  struct ServThreadHandle { Server *server; };
  class Server
  {
    private:
      ServThreadHandle handle;

      String ip;
      int port;

      bool keep_connection;
      bool connected;

      int sock_fd;
      struct sockaddr_in sock_addr;
      pthread_t thread;

      //an odd pattern-
      //holds connection memory contiguously on stack
      //pointers to the above memory, Q'd w/ emtpy connctions last
      Connection cons[FG_MAX_CONNECTIONS];
      Connection *con_ps[FG_MAX_CONNECTIONS];
      int n_cons;
    public:
      Server();
      ~Server();

      void connect(int _port);
      void broadcast(const String &s);
      void disconnect();
      bool healthy();

      void *fork();
  };
}

#endif

