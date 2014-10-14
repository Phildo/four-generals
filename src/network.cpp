#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "logger.h"

using namespace Network;

bool host_priv = true;

String Network::getIP()
{
  struct ifaddrs *ap;
  struct ifaddrs *cur;
  int r;

  char host[NI_MAXHOST];
  String ip;

  r = getifaddrs(&ap);
  for(cur = ap; cur != NULL; cur = cur->ifa_next)
  {
    r = cur->ifa_addr->sa_family;

    if(r == AF_INET)
    {
      r = getnameinfo(cur->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if(strcmp(cur->ifa_name, "en0") == 0) ip = String(host, strlen(host));
    }
  }

 freeifaddrs(ap);
 return ip;
}

void * Network::servThreadHandoff(void * arg) { return ((ServThreadHandle*)arg)->server->fork(); }
void * Network::conThreadHandoff(void * arg)  { return ((ConThreadHandle*) arg)->connection->fork(); }
void * Network::cliThreadHandoff(void * arg)  { return ((CliThreadHandle*) arg)->client->fork(); }

Server::Server()
{
  ip = getIP();
  port = 8080;
  should_disconnect = false;

  handle.server = this;
}

void Server::connect(int _port)
{
  port = _port;

  int r = pthread_create(&thread, NULL, servThreadHandoff, (void *)&handle)  ;
  if(r != 0) fg_log("Failure creating server thread.");
}

void * Server::fork()
{
  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) cons[i].connection = i;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ps[i] = &cons[i];

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
  {
    fg_log("Failure binding server socket.");
    close(sock_fd);
    return 0;
  }
  listen(sock_fd,5);

  Connection *con;
  Connection *tmp_con_p;
  while(!should_disconnect)
  {
    con = con_ps[n_cons];

    con->stale = false;
    con->welcome = true;
    con->sock_addr_len = sizeof(con->sock_addr);
    con->sock_fd = accept(sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
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
    if(n_cons == FG_MAX_CONNECTIONS)
    {
      con->welcome = false;
      int r = pthread_create(&(con->thread), NULL, conThreadHandoff, (void *)&con->handle);
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
      int r = pthread_create(&(con->thread), NULL, conThreadHandoff, (void *)&con->handle);
      if(r != 0) fg_log("Failure creating connection thread.");
    }
  }

  for(int i = 0; i < n_cons; i++)
  {
    con_ps[i]->disconnect();
    close(con_ps[i]->sock_fd);
  }
  n_cons = 0;

  close(sock_fd);
  return 0;
}

void Server::broadcast(const String &s)
{
  fg_log("Serv should broadcast %s",s.ptr());
}

void Server::disconnect()
{
  should_disconnect = true;
  pthread_join(thread, NULL);
}

Server::~Server()
{
}


Client::Client()
{
  ip = getIP();
  port = 8080;
  should_disconnect = false;

  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  port = _port;

  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle)  ;
  if(r != 0) fg_log("Failure creating client thread.");
}

void * Client::fork()
{
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname("localhost");
  if(serv_host == NULL) fg_log("Failure finding server.");

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
  serv_sock_addr.sin_port = htons(port);

  if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Failure connecting client.");
    close(sock_fd);
    return 0;
  }
  int n = 1;

  while(!should_disconnect && n > 0)
  {
    bzero(buff, FG_BUFF_SIZE);
    n = read(sock_fd,buff,FG_BUFF_SIZE-1);
    if(n <= 0) fg_log("Failure reading client.");
    else fg_log("Cli Received(%d): %s",n,buff);
  }
  close(sock_fd);
  return 0;
}

void Client::broadcast(const String &s)
{
  fg_log("Cli should broadcast %s",s.ptr());
}

void Client::disconnect()
{
  should_disconnect = true;
  pthread_join(thread, NULL);
}

Client::~Client()
{
}

Connection::Connection()
{
  should_disconnect = false;

  handle.connection = this;
}

void * Connection::fork()
{
  int n = 1;

  if(!welcome)
  {
    n = write(sock_fd,"Go away.\n",9);
    if(n <= 0) fg_log("Failure writing connection.");
    n = 0;
  }
  while(!should_disconnect && n > 0)
  {
    bzero(buff, FG_BUFF_SIZE);
    n = read(sock_fd, buff, FG_BUFF_SIZE-1);
    if(n <= 0) fg_log("Failure reading connection.");
    else
    {
      fg_log("Serv Received: %s",buff);

      //ack
      n = write(sock_fd,"I gotchu\n",9);
      if(n <= 0) fg_log("Failure writing connection.");
    }
  }
  stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

void Connection::disconnect()
{
  should_disconnect = true;
  pthread_join(thread, NULL);
}

Connection::~Connection()
{
}

