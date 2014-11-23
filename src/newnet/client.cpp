#include "client.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::cliThreadHandoff(void * arg)  { return ((CliThreadHandle*) arg)->client->fork(); }

Client::Client()
{
  con_state = CONNECTION_STATE_DISCONNECTED;
  sock_fd = -1;
  ip = getIP();
  port = 8080;

  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) { fg_log("Client: abort connect (connection exists)"); return; }
  con_state = CONNECTION_STATE_CONNECTING;
  serv_ip = _ip;
  port = _port;

  fg_log("Client: connect (%s:%d)",serv_ip.ptr(),port);
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle)  ;
  if(r != 0)  { fg_log("Client: abort connect (failed fork)"); con_state = CONNECTION_STATE_DISCONNECTED; }
}

void * Client::fork()
{
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host)
  {
    bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
    serv_sock_addr.sin_family = AF_INET;
    bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
    serv_sock_addr.sin_port = htons(port);

    if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) > 0)
    {
      con_state = CONNECTION_STATE_CONNECTED;
      while(con_state == CONNECTION_STATE_CONNECTED)
        tick();
    }
  }
  con_state = CONNECTION_STATE_DISCONNECTING;

  recv_q.empty();
  send_q.empty();

  close(sock_fd);
  con_state = CONNECTION_STATE_STALE;
  return 0;
}

void Client::tick()
{
  fd_set sock_fds;
  struct timeval tv;
  int retval;
  int biggest_fd;

  FD_ZERO(&sock_fds);
  biggest_fd = sock_fd;
  FD_SET(sock_fd, &sock_fds);
  tv.tv_sec = 0; tv.tv_usec = 250000;

  retval = select(bigest_fd+1, &sock_fds, NULL, NULL, &tv);
  if(retval == -1) con_state = CONNECTION_STATE_DISCONNECTING;
  else if(retval && FD_ISSET(sock_fd, &sock_fds))
  {
    Load l;
    if(receiveLoad(&l)) recv_q.enqueue(l);
    else con_state = CONNECTION_STATE_DISCONNECTING;
  }

  Load *l_p;
  while((l_p = send_q.next()))
  {
    if(sendLoad(l_p)) ; //do nothing- already dequeued
    else con_state = CONNECTION_STATE_DISCONNECTING;
  }
}

bool Client::receiveLoad(Load *l)
{
  int len;
  len = recv(sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  return (len > 0);
}

bool Client::sendLoad(Load *l)
{
  int len;
  len = send(sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  return (len > 0);
}

void Client::broadcast(const Load &l)
{
  send_q.enqueue(l);
}

bool Client::read(Load &l)
{
  Load *l_p = recv_q.next();
  if(!l_p) return false;
  l = *l_p;
  return true;
}

void Client::disconnect()
{
  fg_log("Client: abort connection (on purpose)");
  con_state = CONNECTION_STATE_DISCONNECTING;
  pthread_join(thread, NULL);
  con_state = CONNECTION_STATE_DISCONNECTED;
}

Client::~Client()
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) disconnect();
}

