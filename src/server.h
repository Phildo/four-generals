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
      bool connecting;
      bool connected;

      //an odd pattern-
      //holds connection memory contiguously on stack
      //pointers to the above memory, Q'd w/ emtpy connctions last
      int n_cons;
      Connection cons[FG_MAX_CONNECTIONS];
      Connection *con_ptrs[FG_MAX_CONNECTIONS];

      Event history[1024]; //don't know best way to do this...
      int history_i;
    public:
      Server();
      ~Server();

      void connect(int _port);
      void dumpHistory(Connection *c);
      void broadcast(char con, char card, char t);
      void broadcast(Event e);
      void disconnect();
      bool healthy();
      bool transitioning();
      bool stale();

      Event *getEvent(); //aka dequeue

      void *fork();
  };
}

#endif

