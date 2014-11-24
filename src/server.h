#ifndef _FG_SERVER_H
#define _FG_SERVER_H

#include "network.h"
#include "circ_q.h"
#include "id_store.h"

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

      int sock_fd;
      struct sockaddr_in sock_addr;

      String ip;
      int port;

      int n_cons;
      Connection cons[FG_MAX_CONNECTIONS];
      Connection *con_ptrs[FG_MAX_CONNECTIONS];

      circQ<Load, FG_LOAD_Q_SIZE> recv_q;
      circQ<Load, FG_LOAD_Q_SIZE> send_q;
      circQ<Load, FG_LOAD_Q_SIZE> individual_send_q;
      IdStore<FG_MAX_CONNECTIONS> con_id_store;

      void tick();
      void acceptConnection();
      void closeConnection(Connection *con);
      int receiveLoad(Connection *con, Load *l);
      int sendLoad(Connection *con, Load *l);
    public:
      ConnectionState con_state;
      Server();
      ~Server();

      void connect(int _port);
      void broadcast(const Load &l);
      void individualBroadcast(const Load &l);
      bool read(Load &l);
      void disconnect();

      bool hasConnection(char con_id);

      void *fork();
  };
}

#endif

