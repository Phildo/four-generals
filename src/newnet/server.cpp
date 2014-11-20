#include "server.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::servThreadHandoff(void * arg) { return ((ServThreadHandle*)arg)->server->fork(); }

Server::Server()
{
  con_state = CONNECTION_STATE_DISCONNECTED;
  ip = getIP();
  port = 8080;

  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) cons[i].connection = '1'+i;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ptrs[i] = &cons[i];

  handle.server = this;
}

void Server::connect(int _port)
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) { fg_log("Server: abort connect (connection exists)"); return; }
  con_state = CONNECTION_STATE_CONNECTING;
  port = _port;

  fg_log("Server: connect (%s:%d)",ip.ptr(),port);
  int r = pthread_create(&thread, NULL, servThreadHandoff, (void *)&handle)  ;
  if(r != 0)  { fg_log("Server: abort connect (failed fork)"); con_state = CONNECTION_STATE_DISCONNECTED; }
}

void * Server::fork()
{
  int sock_fd;
  struct sockaddr_in sock_addr;
  fd_set sock_fds;
  struct timeval tv;
  int retval;

  Connection *tmp_con_p;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
  {
    con_state = CONNECTION_STATE_DISCONNECTING;
    close(sock_fd);
    fg_log("Server: abort connect (cannot bind port)");
    con_state = CONNECTION_STATE_DISCONNECTED;
    return 0;
  }
  listen(sock_fd,FG_ACCEPT_Q_SIZE);
  con_state = CONNECTION_STATE_CONNECTED;
  while(con_state == CONNECTION_STATE_CONNECTED)
  {
    FD_ZERO(&sock_fds);
    biggest_fd = sock_fd;
    FD_SET(sock_fd, &sock_fds);
    for(int i = 0; i < n_cons; i++)
    {
      FD_SET(con_ptrs[i]->sock_fd, &sock_fds);
      if(biggest_fd > con_ptrs[i]->sock_fd) biggest_fd = con_ptrs[i]->sock_fd;
    }
    tv.tv_sec = 0; tv.tv_usec = 250000;

    retval = select(bigest_fd+1, &sock_fds, NULL, NULL, &tv);
    if(retval == -1) con_state = CONNECTION_STATE_DISCONNECTING;
    else if(retval)
    {
      if(FD_ISSET(sock_fd, &sock_fds)) //me
      {
        acceptConnection();
      }
      for(int i = 0; i < n_cons; i++)
      {
        len = recv(n_cons[i]->sock_fd, buff, FG_BUFF_SIZE-1, 0);
      }
    }

    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ptrs[i].con_state == CONNECTION_STATE_STALE)
      {
        con_ptrs[i]->disconnect();

        con = con_ptrs[i];
        con_ptrs[i] = con_ptrs[n_cons-1];
        con_ptrs[n_cons-1] = con_p;
        n_cons--;

        i--;
      }
    }
  }
  con_state = CONNECTION_STATE_DISCONNECTING;

  for(int i = 0; i < n_cons; i++)
    con_ptrs[i]->disconnect();
  n_cons = 0;

  close(sock_fd);
  con_state = CONNECTION_STATE_STALE;
  return 0;
}

void Server::acceptConnection()
{
  Connection *con = con_ptrs[n_cons];;
  con->sock_fd = accept(sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
  if(con->sock_fd >= 0)
  {
    n_cons++;
    con->connect();

    if(n_cons < FG_MAX_CONNECTIONS)
    {
      //cool
    }
    else
    {
      con->disconnect();
      n_cons--;
    }
  }
}


void Server::broadcast(const Load &l)
{
  for(int i = 0; i < n_cons; i++)
    con_ptrs[i]->broadcast(e);
}

bool Server::read(Load &l)
{
  for(int i = 0; i < n_cons; i++)
  {
    if(con_ptrs[i]->read(l))
      return true;
  }
  return false;
}

void Server::disconnect()
{
  fg_log("Server: abort connection (on purpose)");
  con_state = CONNECTION_STATE_DISCONNECTING;
  pthread_join(thread, NULL);
  con_state = CONNECTION_STATE_DISCONNECTED;
}

Server::~Server()
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) disconnect();
}

