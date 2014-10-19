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
#define FG_EVT_Q_SIZE 1024

namespace Network
{
  extern bool host_priv;
  String getIP();
  void * servThreadHandoff(void * arg);
  void * conThreadHandoff(void * arg);
  void * cliThreadHandoff(void * arg);

  //to keep track of already assigned (CAPS = taken) (god what a terrible system)
  //AbCdefghiJkLmNopqRstuvwxyz
  static const char e_type_assign_con  = 'c';
  static const char e_type_refuse_con  = 'n';
  static const char e_type_join        = 'j';
  static const char e_type_leave       = 'l';
  static const char e_type_assign_card = 'a';
  static const char e_type_revoke_card = 'r';
  struct Event //all members chars for quick/simple serializability
  {
    char connection;
    char cardinal;
    char type;
    const char null;

    //default constructor
    Event() : null('\0') {};

    //copy constructor
    Event(const Event &e)     : null('\0') { connection = e.connection; cardinal = e.cardinal; type = e.type; }
    Event &operator=(const Event &e) { connection = e.connection; cardinal = e.cardinal; type = e.type; return *this; } //no need to set null

    //custom constructor
    Event(char con, char card, char t) : connection(con), cardinal(card), type(t), null('\0') {}

    //serializability
    char *serialize() { return (char *)&connection; }
    Event(char *c) : null('\0') { connection = c[0]; cardinal = c[1]; type = c[2]; }
    int serlen() { return 3; }
  };

  class Connection;
  struct ConThreadHandle { Connection *connection; };
  class Connection
  {
    private:
      bool keep_connection;
      bool connected;
    public:
      ConThreadHandle handle;

      char connection; //'0' - '0'+FG_MAX_CONNECTIONS
      bool stale;
      bool welcome;

      int sock_fd;
      socklen_t sock_addr_len; //sizeof addr
      struct sockaddr_in sock_addr;

      pthread_t thread;
      char read[FG_BUFF_SIZE]; int readlen;
      char writ[FG_BUFF_SIZE]; int writlen;

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
      char read[FG_BUFF_SIZE]; int readlen;
      char writ[FG_BUFF_SIZE]; int writlen;

      void enqueueEvent(Event);
      Event evt_q[FG_EVT_Q_SIZE];
      int evt_q_front_i;
      int evt_q_back_i;
    public:
      Client();
      ~Client();

      void connect(const String &_ip, int _port);
      void broadcast(const String &s);
      void disconnect();
      bool healthy();

      Event *getEvent(); //aka dequeue

      void *fork();
  };
}

#endif

