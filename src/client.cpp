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
  fd_set sock_fds;
  struct timeval tv;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host)
  {
    bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
    serv_sock_addr.sin_family = AF_INET;
    bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
    serv_sock_addr.sin_port = htons(port);

    fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL)|O_NONBLOCK); //set nonblocking

    if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
    {
      if(errno == EINPROGRESS)
      {
        FD_ZERO(&sock_fds);
        FD_SET(sock_fd, &sock_fds);
        tv.tv_sec = 0; tv.tv_usec = FG_US_PER_TICK;

        if((select(sock_fd+1, NULL, &sock_fds, NULL, &tv) <= 0) || !FD_ISSET(sock_fd, &sock_fds))
        {
          fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL)&(~O_NONBLOCK)); //set blocking
          con_state = CONNECTION_STATE_DISCONNECTING; //either error or timeout- disconnect in both cases
        }
        else
        {
          fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL)&(~O_NONBLOCK)); //set blocking

          con_state = CONNECTION_STATE_WAITING;

          FD_ZERO(&sock_fds);
          FD_SET(sock_fd, &sock_fds);
          tv.tv_sec = 0; tv.tv_usec = FG_US_PER_TICK;

          int retval = select(sock_fd+1, &sock_fds, NULL, NULL, &tv);
          if(retval == -1) con_state = CONNECTION_STATE_DISCONNECTING;
          else if(retval && FD_ISSET(sock_fd, &sock_fds))
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
                if(con_id == 0) con_state = CONNECTION_STATE_DISCONNECTING;
                else con_state = CONNECTION_STATE_CONNECTED;
              }
              else con_state = CONNECTION_STATE_DISCONNECTING;
            }
            else con_state = CONNECTION_STATE_DISCONNECTING;
          }

          while(con_state == CONNECTION_STATE_CONNECTED)
            tick();
        }
      }
      else con_state = CONNECTION_STATE_DISCONNECTING; //actual error on connect
    }
  }
  fg_log("Client: disconnecting");
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
  if(len > 0) fg_log("Client: recv %s",l->data);
  return (len > 0);
}

bool Client::sendLoad(Load *l)
{
  int len;
  len = send(sock_fd, l->data, FG_LOAD_BUFF_SIZE, 0);
  if(len > 0) fg_log("Client: sent %s",l->data);
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

