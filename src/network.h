#ifndef NETWORK_H
#define NETWORK_H

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

#define MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define MAX_IP_LENGTH 16
#define BUFF_SIZE 256

class Model;

struct Connection
{
  int connection; //0-MAX_CONNECTIONS
  bool stale;
  bool welcome;

  int sock_fd;
  socklen_t sock_addr_len; //sizeof addr
  struct sockaddr_in sock_addr;

  pthread_t thread;
  char buff[BUFF_SIZE];
};

class Network
{
  private:
    char ip[MAX_IP_LENGTH];
    int iplen;
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
    Connection cons[MAX_CONNECTIONS];
    Connection *con_ps[MAX_CONNECTIONS];

    int cli_sock_fd;
    pthread_t cli_thread;
    struct sockaddr_in cli_serv_sock_addr; //client's serv addr
    struct hostent *cli_server; //client's reference to server
    char cli_buff[BUFF_SIZE];

    Model *model;
  public:
    Network(Model *m);
    ~Network();
    void connectAsServer(int _port);
    void connectAsClient(char *_ip, int _port);
    void broadcast(char *c, int l);
    void disconnect();
    void getIP(char *_ip, int *_len);

    void *serverThread();
    void *connectionThread(Connection * con);
    void *clientThread();
};

#endif

