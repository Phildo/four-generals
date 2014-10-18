#ifndef _FG_NETWORK_H
#define _FG_NETWORK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>

extern "C"
{
  #include <pthread.h>
  #include <unistd.h>
}

#include "string.h"

#define FG_MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define FG_BUFF_SIZE 256

namespace Network
{
  extern bool host_priv;
  String getIP();
  void * servThreadHandoff(void * arg);
  void * conThreadHandoff(void * arg);
  void * cliThreadHandoff(void * arg);

  class Connection;
  struct ConThreadHandle { Connection *connection; };
  class Connection
  {
    private:
      bool keep_connection;
      bool connected;
    public:
      ConThreadHandle handle;

      int connection; //0-FG_MAX_CONNECTIONS
      bool stale;
      bool welcome;

      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;

      pthread_t thread;
      char buff[FG_BUFF_SIZE]; int bufflen;
      char mess[FG_BUFF_SIZE]; int messlen;

      Connection();
      ~Connection();

      void disconnect();
      bool healthy();

      void *fork();
  };

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

  class Client;
  struct CliThreadHandle { Client *client; };
  class Client
  {
    private:
      CliThreadHandle handle;

      String ip;
      String serv_ip;
      int port;

      bool keep_connection;
      bool connected;

      int sock_fd;
      pthread_t thread;
      struct sockaddr_in serv_sock_addr; //client's serv addr
      struct hostent *serv_host; //client's reference to server
      char buff[FG_BUFF_SIZE]; int bufflen;
      char mess[FG_BUFF_SIZE]; int messlen;
    public:
      Client();
      ~Client();

      void connect(const String &_ip, int _port);
      void broadcast(const String &s);
      void disconnect();
      bool healthy();

      void *fork();
  };
}

#endif

