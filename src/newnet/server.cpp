#include "server.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::servThreadHandoff(void * arg) { return ((ServThreadHandle*)arg)->server->fork(); }

Server::Server()
{
  con_state = CONNECTION_STATE_DISCONNECTED;
  sock_fd = -1;
  ip = getIP();
  port = 8080;

  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ids[i] = 0;
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
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) >= 0)
  {
    listen(sock_fd,FG_ACCEPT_Q_SIZE);
    con_state = CONNECTION_STATE_CONNECTED;
    while(con_state == CONNECTION_STATE_CONNECTED)
      tick();
  }
  con_state = CONNECTION_STATE_DISCONNECTING;

  recv_q.empty();
  send_q.empty();

  for(int i = 0; i < n_cons; i++)
    closeConnection(con_ptrs[0]->con_id);

  close(sock_fd);
  con_state = CONNECTION_STATE_STALE;
  return 0;
}

void Server::tick()
{
  fd_set sock_fds;
  struct timeval tv;
  int retval;
  int biggest_fd;

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
    if(FD_ISSET(sock_fd, &sock_fds))
      acceptConnection();

    Load l;
    for(int i = 0; i < n_cons; i++)
    {
      if(FD_ISSET(con->sock_fd, &sock_fds))
      {
        if(receiveLoad(con_ptrs[i], &l)) recv_q.enqueue(l);
        else closeConnection(con_ptrs[i--]);
      }
    }

    Load *l_p;
    while((l_p = send_q.next()))
    {
      for(int i = 0; i < n_cons; i++)
      {
        if(sendLoad(con_ptrs[i], l_p)) ; //do nothing- already dequeued
        else closeConnection(con_ptrs[i--]);
      }
    }
  }
}

void Server::acceptConnection()
{
  Connection *con = con_ptrs[n_cons];;
  con->sock_fd = accept(sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
  if(con->sock_fd >= 0)
  {
    con->con_id = con_id_store.getId();
    n_cons++;

    if(n_cons >= FG_MAX_CONNECTIONS)
    {
      //con->broadcast('getoutahere');
      closeConnection(con);
    }
  }
}

void Server::closeConnection(Connection *con)
{
  int i;
  for(i = 0; i < n_cons; i++)
    if(con_ptrs[i] == con) break;

  con_id_store.retireId(con->con_id);
  close(con->sock_fd);

  con_ptrs[i] = con_ptrs[n_cons-1];
  con_ptrs[n_cons-1] = con;
  n_cons--;
}

bool Server::receiveLoad(Connection *con, Load *l)
{
  int len;
  l->con_id = con->con_id;
  len = recv(con->sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  return (len > 0);
}

bool Server::sendLoad(Connection *con, Load *l)
{
  int len;
  len = send(con->sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  return (len > 0);
}

void Server::broadcast(const Load &l)
{
  send_q.enqueue(l);
}

bool Server::read(Load &l)
{
  Load *l_p = recv_q.next();
  if(!l_p) return false;
  l = *l_p;
  return true;
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

