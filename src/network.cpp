#include "network.h"

#include <fcntl.h>
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
  keep_connection = false;
  handle.server = this;
}

void Server::connect(int _port)
{
  port = _port;

  fg_log("Server connecting on port %d",port);
  keep_connection = true;
  int r = pthread_create(&thread, NULL, servThreadHandoff, (void *)&handle)  ;
  if(r != 0) { fg_log("Failure creating server thread."); keep_connection = false; }
}

void * Server::fork()
{
  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) cons[i].connection = i;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ps[i] = &cons[i];

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sock_fd, F_SETFL, O_NONBLOCK);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
  {
    fg_log("Failure binding server socket.");
    keep_connection = false;
    return 0;
  }
  listen(sock_fd,5);

  Connection *con;
  Connection *tmp_con_p;
  while(keep_connection) //spin wait. bad idea? tune in next time to find out.
  {
    con = con_ps[n_cons];

    con->stale = false;
    con->welcome = true;
    con->sock_addr_len = sizeof(con->sock_addr);
    con->sock_fd = -1;

    con->sock_fd = accept(sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
    if(con->sock_fd >= 0)
    {
      n_cons++;

      //Final connection will be told off and closed
      //Wait for that to happen before accepting any more
      if(n_cons == FG_MAX_CONNECTIONS)
      {
        con->welcome = false;
        int r = pthread_create(&(con->thread), NULL, conThreadHandoff, (void *)&con->handle);
        if(r != 0) fg_log("Failure creating connection thread.");
        con->disconnect();

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
        if(r != 0) { fg_log("Failure creating connection thread."); keep_connection = false; }
      }
    }

    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ps[i]->stale)
      {
        //clean up/kill thread/connection
        con_ps[i]->disconnect();
        n_cons--;

        //put newly cleaned con at end of list
        tmp_con_p = con_ps[i];
        con_ps[i] = con_ps[n_cons];
        con_ps[n_cons] = tmp_con_p;

        i--;
      }
    }
  }

  for(int i = 0; i < n_cons; i++)
    con_ps[i]->disconnect();
  n_cons = 0;

  return 0;
}

void Server::broadcast(const String &s)
{
  fg_log("Serv should broadcast %s",s.ptr());
}

void Server::disconnect()
{
  fg_log("Server disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
  close(sock_fd);
}

bool Server::healthy()
{
  return keep_connection;
}

Server::~Server()
{
  if(keep_connection) disconnect();
}


Client::Client()
{
  ip = getIP();
  port = 8080;
  keep_connection = false;
  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  port = _port;
  serv_ip = _ip;

  fg_log("Client connecting to IP:%s on port %d", serv_ip.ptr(), port);
  keep_connection = true;
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle)  ;
  if(r != 0) { fg_log("Failure creating client thread."); keep_connection = false; }
}

void * Client::fork()
{
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host == NULL) { fg_log("Failure finding server."); keep_connection = false; }

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
  serv_sock_addr.sin_port = htons(port);

  if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Failure connecting client.");
    keep_connection = false;
    return 0;
  }

  fcntl(sock_fd, F_SETFL, O_NONBLOCK);
  bufflen = 0;
  messlen = 0;
  bzero(buff, FG_BUFF_SIZE);
  bzero(mess, FG_BUFF_SIZE);
  while(keep_connection)
  {
    bufflen = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
    if(bufflen > 0)
    {
      fg_log("Cli Received(%d): %s",bufflen,buff);
      bufflen = 0;
    }

    if(messlen > 0)
    {
      messlen = send(sock_fd, mess, messlen, 0);
      if(messlen <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
      messlen = 0;
    }
  }
  return 0;
}

void Client::broadcast(const String &s)
{
  fg_log("Cli should broadcast %s",s.ptr());
}

void Client::disconnect()
{
  fg_log("Cient disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
  close(sock_fd);
}

bool Client::healthy()
{
  return keep_connection;
}

Client::~Client()
{
  if(keep_connection) disconnect();
}

Connection::Connection()
{
  keep_connection = false;
  handle.connection = this;
}

void * Connection::fork()
{
  if(!welcome)
  {
    fg_log("Connection unwelcome");
    messlen = send(sock_fd, "Go away.\n", 9, 0);
    if(messlen <= 0) fg_log("Failure writing connection.");
  }
  else fg_log("Connection created");
  keep_connection = welcome;

  bufflen = 0;
  messlen = 0;
  bzero(buff, FG_BUFF_SIZE);
  bzero(mess, FG_BUFF_SIZE);
  while(keep_connection)
  {
    bufflen = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
    if(bufflen > 0)
    {
      fg_log("Serv Received %d: %s",bufflen,buff);
      bufflen = 0;
    }

    if(messlen > 0)
    {
      messlen = send(sock_fd, mess, messlen, 0);
      if(messlen <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
      messlen = 0;
    }
  }
  stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

void Connection::disconnect()
{
  fg_log("Connection disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
  close(sock_fd);
}

bool Connection::healthy()
{
  return keep_connection;
}

Connection::~Connection()
{
  if(keep_connection) disconnect();
}

