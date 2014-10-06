#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>

extern "C"
{
  #include <pthread.h>
  #include <unistd.h>
}

#include "defines.h"
#include "logger.h"

#define MAX_IP_LENGTH 256 //really only need 12, but might as well leave it for urls?

#define MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define BUFF_SIZE 256

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

void * serverThread(void * arg);
void * connectionThread(void * arg);
void * clientThread(void * arg);

const bool host_priv = true;
int portno = 8080;
char ip[MAX_IP_LENGTH];

bool is_serv = false;
bool is_cli = false;

bool should_disconnect = false;

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

void Network::connectAsServer()
{
  is_serv = true;
  int r = pthread_create(&serv_thread, NULL, serverThread, NULL)  ;
  if(r != 0) fg_log("Failure creating server thread.");
}

void * serverThread(void * arg)
{
  n_cons = 0;
  for(int i = 0; i < MAX_CONNECTIONS; i++) cons[i].connection = i;
  for(int i = 0; i < MAX_CONNECTIONS; i++) con_ps[i] = &cons[i];

  serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
  serv_sock_addr.sin_port = htons(portno);

  if(bind(serv_sock_fd, (struct sockaddr *) &serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Failure binding server socket.");
    close(serv_sock_fd);
    return 0;
  }
  listen(serv_sock_fd,5);

  Connection *con;
  Connection *tmp_con_p;
  while(!should_disconnect)
  {
    con = con_ps[n_cons];

    con->stale = false;
    con->welcome = true;
    con->sock_addr_len = sizeof(con->sock_addr);
    con->sock_fd = accept(serv_sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
    if(con->sock_fd < 0) fg_log("Failure accepting connection.");
    n_cons++;

    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ps[i]->stale)
      {
        //clean up/kill thread/connection
        pthread_join(con_ps[i]->thread, NULL);
        close(con_ps[i]->sock_fd);
        n_cons--;

        //put newly cleaned con at end of list
        tmp_con_p = con_ps[i];
        con_ps[i] = con_ps[n_cons];
        con_ps[n_cons] = tmp_con_p;
      }
    }

    //Final connection will be told off and closed
    //Wait for that to happen before accepting any more
    if(n_cons == MAX_CONNECTIONS)
    {
      con->welcome = false;
      int r = pthread_create(&(con->thread), NULL, connectionThread, (void *)con);
      if(r != 0) fg_log("Failure creating connection thread.");
      pthread_join(con->thread, NULL);
      close(con->sock_fd);

      //find con and put it back at the end of the queue #ugly
      int con_index = 0;
      for(int i = 0; i < n_cons; i++) if(con_ps[i] == con) con_index = i;
      n_cons--;

      tmp_con_p = con_ps[con_index];
      con_ps[con_index] = con_ps[n_cons];
      con_ps[n_cons] = tmp_con_p;
    }
    else
    {
      int r = pthread_create(&(con->thread), NULL, connectionThread, (void *)con);
      if(r != 0) fg_log("Failure creating connection thread.");
    }
  }

  for(int i = 0; i < n_cons; i++)
  {
    pthread_join(con_ps[i]->thread, NULL);
    close(con_ps[i]->sock_fd);
  }
  n_cons = 0;

  close(serv_sock_fd);
  return 0;
}

void * connectionThread(void * arg)
{
  Connection *con = (Connection *)arg;
  int n = 1;

  if(!con->welcome)
  {
    n = write(con->sock_fd,"Go away.\n",9);
    if(n <= 0) fg_log("Failure writing connection.");
    n = 0;
  }
  while(!should_disconnect && n > 0)
  {
    bzero(con->buff, BUFF_SIZE);
    n = read(con->sock_fd, con->buff, BUFF_SIZE-1);
    if(n <= 0) fg_log("Failure reading connection.");
    else
    {
      fg_log("Serv Received: %s",con->buff);

      //ack
      n = write(con->sock_fd,"I gotchu\n",9);
      if(n <= 0) fg_log("Failure writing connection.");
    }
  }
  con->stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

void Network::connectAsClient()
{
  is_cli = true;
  int r = pthread_create(&cli_thread, NULL, clientThread, NULL)  ;
  if(r != 0) fg_log("Failure creating client thread.");
}

void * clientThread(void *arg)
{
  cli_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  cli_server = gethostbyname("localhost");
  if(cli_server == NULL) fg_log("Failure finding server.");

  bzero((char *)&cli_serv_sock_addr, sizeof(cli_serv_sock_addr));
  cli_serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)cli_server->h_addr, (char *)&cli_serv_sock_addr.sin_addr.s_addr, cli_server->h_length);
  cli_serv_sock_addr.sin_port = htons(portno);

  if(connect(cli_sock_fd,(struct sockaddr *)&cli_serv_sock_addr, sizeof(cli_serv_sock_addr)) < 0)
  {
    fg_log("Failure connecting client.");
    close(cli_sock_fd);
    return 0;
  }
  int n = 1;

  while(!should_disconnect && n > 0)
  {
    bzero(cli_buff, BUFF_SIZE);
    n = read(cli_sock_fd,cli_buff,BUFF_SIZE-1);
    if(n <= 0) fg_log("Failure reading client.");
    else fg_log("Cli Received(%d): %s",n,cli_buff);
  }
  close(cli_sock_fd);
  return 0;
}

void Network::broadcast(char *c, int l)
{
  if(is_serv) fg_log("Serv should broadcast %s",c);
  if(is_cli) fg_log("Cli should broadcast %s",c);
}

void Network::disconnect()
{
  should_disconnect = true;
  if(is_serv) pthread_join(serv_thread, NULL); //main thread waits for serv_thread to finish
  if(is_cli)  pthread_join(cli_thread, NULL); //main thread waits for cli_thread to finish
  is_cli = false;
  is_serv = false;
}

void Network::getIP(char *ip)
{
  struct ifaddrs *ap;
  struct ifaddrs *cur;
  int r;

  char host[NI_MAXHOST];
  strcpy(ip,"0.0.0.0");

  r = getifaddrs(&ap);
  for(cur = ap; cur != NULL; cur = cur->ifa_next)
  {
    r = cur->ifa_addr->sa_family;

    if(r == AF_INET)
    {
      r = getnameinfo(cur->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if(strcmp(cur->ifa_name, "en0") == 0) strcpy(ip, host);
    }
  }

  freeifaddrs(ap);
}

