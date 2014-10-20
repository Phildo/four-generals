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
#define FG_ACCEPT_Q_SIZE 5 //recommended (also, can't be more... I don't think...)
#define FG_BUFF_SIZE 256
#define FG_EVT_Q_SIZE 1024
#define FG_EVT_MAX_DEC_LEN 6

namespace Network
{
  extern bool host_priv;
  String getIP();
  void * servThreadHandoff(void * arg);
  void * conThreadHandoff(void * arg);
  void * cliThreadHandoff(void * arg);

  //to keep track of already assigned (CAPS = taken) (god what a terrible system)
  //AbCDefghiJkLmNopqRstuvwxyz
  static const char e_type_ack         = 'a';
  static const char e_type_assign_con  = 'c';
  static const char e_type_refuse_con  = 'n';
  static const char e_type_join        = 'j';
  static const char e_type_leave       = 'l';
  static const char e_type_assign_card = 'd';
  static const char e_type_revoke_card = 'r';
  struct Event //all members chars for quick/simple serializability
  {
    char connection;
    char cardinal;
    char type;
    char id_c[FG_EVT_MAX_DEC_LEN]; //string val of id_i (ie "2415")
    const char null;
    int id_i;

    //default constructor
    Event();

    //copy constructor
    Event(const Event &e);
    Event &operator=(const Event &e);

    //custom constructor
    Event(char con, char card, char t, int id);

    //serializability
    char *serialize();
    Event(char *c);
    int serlen();
  };

  class Connection;
  struct ConThreadHandle { Connection *connection; };
  class Connection
  {
    private:
      bool keep_connection;
      bool connected;

      int evt_id_inc;
      int nextEventId();
      void enqueueAckWait(Event e);
      void ackReceived(int id);
      Event ack_q[FG_EVT_Q_SIZE];
      int ack_q_front;
      int ack_q_back;
    public:
      ConThreadHandle handle;

      char connection; //'1' thru '1'+(FG_MAX_CONNECTIONS-1) or '0' for none
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
      int evt_q_front;
      int evt_q_back;
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

