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

class Model;

struct Connection
{
  int connection; //0-FG_MAX_CONNECTIONS
  bool stale;
  bool welcome;

  int sock_fd;
  socklen_t sock_addr_len; //sizeof addr
  struct sockaddr_in sock_addr;

  pthread_t thread;
  char buff[FG_BUFF_SIZE];
};

class Network
{
  private:
    String ip;
    int port;

    bool host_priv;

    bool is_serv;
    bool is_cli;

    bool should_disconnect;

    int serv_sock_fd;
    struct sockaddr_in serv_sock_addr;
    pthread_t serv_thread;

    int n_cons;

    //an odd pattern-
    //holds connection memory contiguously on stack
    //pointers to the above memory, Q'd w/ emtpy connctions last
    Connection cons[FG_MAX_CONNECTIONS];
    Connection *con_ps[FG_MAX_CONNECTIONS];

    int cli_sock_fd;
    pthread_t cli_thread;
    struct sockaddr_in cli_serv_sock_addr; //client's serv addr
    struct hostent *cli_server; //client's reference to server
    char cli_buff[FG_BUFF_SIZE];

    Model *model;
  public:
    Network(Model *m);
    ~Network();
    void connectAsServer(int _port);
    void connectAsClient(const String &_ip, int _port);
    void broadcast(const String &s);
    void disconnect();
    String getIP();

    void *serverThread();
    void *connectionThread(Connection * con);
    void *clientThread();
};

#endif

