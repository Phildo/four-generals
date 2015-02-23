#include "client.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

#include "errno.h" //ughhh

using namespace Network;

void * Network::cliThreadHandoff(void * arg)  { return ((CliThreadHandle*) arg)->client->fork(); }

Client::Client()
{
  con_state = CONNECTION_STATE_DISCONNECTED;
  con_id = 0;
  sock_fd = -1;
  ip = getIP();
  port = 4040;

  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) { fg_log("Client: refuse connect (connection exists)"); return; }
  con_state = CONNECTION_STATE_CONNECTING;
  serv_ip = _ip;
  port = _port;

  fg_log("Client: connect (%s:%d)",serv_ip.ptr(),port);
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle)  ;
  if(r != 0)  { fg_log("Client: abort connect (failed fork)"); con_state = CONNECTION_STATE_DISCONNECTED; }
}

void * Client::fork()
{
  fd_set sock_fds;
  struct timeval tv;
  int retval;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(!serv_host) { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (unable to resolve host)"); }

  if(con_state != CONNECTION_STATE_DISCONNECTING)
  {
    bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
    serv_sock_addr.sin_family = AF_INET;
    bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
    serv_sock_addr.sin_port = htons(port);

    fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL)|O_NONBLOCK); //set nonblocking
    retval = ::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr));
    fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL)&(~O_NONBLOCK)); //set blocking

    if(retval < 0) // 0 = success
    {
      if(errno == EINPROGRESS) //give a second to connect
      {
        FD_ZERO(&sock_fds);
        FD_SET(sock_fd, &sock_fds);
        tv.tv_sec = 0; tv.tv_usec = FG_US_CONNECT;

        retval = select(sock_fd+1, NULL, &sock_fds, NULL, &tv);
        if(retval == 0) FD_ZERO(&sock_fds);

        if(retval < 0)                    { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (connect error)"); }
        if(!FD_ISSET(sock_fd, &sock_fds)) { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (connect timeout)"); }
      }
      else { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (connection error)"); }
    }
  }

  if(con_state != CONNECTION_STATE_DISCONNECTING)
  {
    con_state = CONNECTION_STATE_WAITING;

    FD_ZERO(&sock_fds);
    FD_SET(sock_fd, &sock_fds);
    tv.tv_sec = 1; tv.tv_usec = 0; //give whole second to listen for ack

    retval = select(sock_fd+1, &sock_fds, NULL, NULL, &tv);
    if(retval == 0) FD_ZERO(&sock_fds);

    if(retval < 0)                         { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (ack error)"); }
    else if(!FD_ISSET(sock_fd, &sock_fds)) { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (ack timeout)"); }
    else if(FD_ISSET(sock_fd, &sock_fds))
    {
      Load l;
      if(receiveLoad(&l))
      {
        if(con_id == 0 && //this is the silliest thing...
          l.data[ 0] == 'F' &&
          l.data[ 1] == 'G' &&
          l.data[ 2] == '_' &&
          l.data[ 3] == 'H' &&
          l.data[ 4] == 'A' &&
          l.data[ 5] == 'N' &&
          l.data[ 6] == 'D' &&
          l.data[ 7] == 'S' &&
          l.data[ 8] == 'H' &&
          l.data[ 9] == 'A' &&
          l.data[10] == 'K' &&
          l.data[11] == 'E' &&
          l.data[12] == ':'
          )
        {
          con_id = String(&l.data[13],3).intVal();
          con_state = CONNECTION_STATE_CONNECTED; //assume success
          if(con_id == 0) { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (ack con_id 0)"); }
        }
        else { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (invalid ack)"); }
      }
      else { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (ack read failed)"); }
    }
  }

  while(con_state == CONNECTION_STATE_CONNECTED)
    tick();

  fg_log("Client: disconnecting (left tick loop)");
  con_state = CONNECTION_STATE_DISCONNECTING;

  recv_q.empty();
  send_q.empty();

  close(sock_fd);
  con_id = 0;
  con_state = CONNECTION_STATE_STALE;
  return 0;
}

void Client::tick()
{
  fd_set sock_fds;
  struct timeval tv;
  int retval;

  FD_ZERO(&sock_fds);
  FD_SET(sock_fd, &sock_fds);
  tv.tv_sec = 0; tv.tv_usec = FG_US_PER_TICK;

  retval = select(sock_fd+1, &sock_fds, NULL, NULL, &tv);
  if(retval == 0) FD_ZERO(&sock_fds);

  if(retval < 0) { con_state = CONNECTION_STATE_DISCONNECTING; fg_log("Client: abort connection (mux error)"); }
  else if(FD_ISSET(sock_fd, &sock_fds))
  {
    Load l;
    if(receiveLoad(&l)) recv_q.enqueue(l);
    else
    {
      fg_log("Client: abort connection (failed read)");
      con_state = CONNECTION_STATE_DISCONNECTING;
    }
  }

  Load *l_p;
  while((l_p = send_q.next()))
  {
    if(sendLoad(l_p)) ; //do nothing- already dequeued
    else
    {
      fg_log("Client: abort connection (failed send)");
      con_state = CONNECTION_STATE_DISCONNECTING;
    }
  }
}

bool Client::receiveLoad(Load *l)
{
  int len;
  len = recv(sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  if(len > 0) fg_log("Client: recv %s",l->data);
  else        fg_log("Client: failed recv");
  return (len > 0);
}

bool Client::sendLoad(Load *l)
{
  int len;
  len = send(sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  if(len > 0) fg_log("Client: sent %s",l->data);
  else        fg_log("Client: failed sent %s",l->data);
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
  if(con_state == CONNECTION_STATE_DISCONNECTING) return;
  if(con_state == CONNECTION_STATE_DISCONNECTED)  return;
  if(con_state == CONNECTION_STATE_STALE) fg_log("Client: abort connection (cleanup)");
  else                                    fg_log("Client: abort connection (on purpose)");
  con_state = CONNECTION_STATE_DISCONNECTING;
  pthread_join(thread, NULL);
  con_state = CONNECTION_STATE_DISCONNECTED;
  fg_log("Client: aborted");
}

Client::~Client()
{
  disconnect(); //will pass through if not needed
}

