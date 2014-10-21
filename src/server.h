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
      pthread_t thread;

      String ip;
      int port;

      bool keep_connection;
      bool connected;

      //an odd pattern-
      //holds connection memory contiguously on stack
      //pointers to the above memory, Q'd w/ emtpy connctions last
      int n_cons;
      Connection cons[FG_MAX_CONNECTIONS];
      Connection *con_ptrs[FG_MAX_CONNECTIONS];
    public:
      Server();
      ~Server();

      void connect(int _port);
      void broadcast(const String &s);
      bool healthy();
      void disconnect();

      void *fork();
  };
}

#endif

