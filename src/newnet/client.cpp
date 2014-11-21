#include "client.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::cliThreadHandoff(void * arg)  { return ((CliThreadHandle*) arg)->client->fork(); }

Client::Client()
{
  ip = getIP();
  port = 8080;

  evt_id_inc = 0;
  connection = '0';
  keep_connection = false;
  connecting = false;
  connected = false;
  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  if(!stale()) { fg_log("Client: abort connect (existing connection)"); return; }
  port = _port;
  serv_ip = _ip;

  connecting = true;
  keep_connection = true;

  fg_log("Client: connect (%s:%d)", serv_ip.ptr(), port);
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle);
  if(r != 0) { fg_log("Client: abort connect (failed fork)"); connecting = false; keep_connection = false; }
}

void * Client::fork()
{
  int sock_fd;
  struct sockaddr_in serv_sock_addr; //client's serv addr
  struct hostent *serv_host; //client's reference to server

  int len = 0;
  Event *send_evt;
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host == NULL) { fg_log("Client: abort connect (can't find server)"); keep_connection = false; connecting = false; return 0; }

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
  serv_sock_addr.sin_port = htons(port);

  if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Client: abort connect (unable to connect)");
    keep_connection = false;
    connecting = false;
    return 0;
  }
  connected = true;
  connecting = false;

  //sock list
  fd_set sock_fds;
  struct timeval tv;
  int retval;

  bzero(buff, FG_BUFF_SIZE);
  while(keep_connection)
  {
    FD_ZERO(&sock_fds);
    FD_SET(sock_fd, &sock_fds);
    tv.tv_sec = 0; tv.tv_usec = 250000;

    //fg_log("bsclientlogdebugging: selecting...");
    retval = select(sock_fd+1, &sock_fds, NULL, NULL, &tv);
    //fg_log("bsclientlogdebugging: selected");

    if(retval == -1) keep_connection = false;
    else if(retval && FD_ISSET(sock_fd, &sock_fds))
    {
      len = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
      if(len == 0) keep_connection = false;
      int mess_num = 0;
      while(len > 0)
      {
        Event e(buff+(mess_num*e_ser_len));
        if     (e.type == e_type_assign_con)                                { connection = e.connection; broadcast('0',e_type_join_con); }
        else if(e.type == e_type_revoke_con || e.type == e_type_refuse_con) { connection = '0'; keep_connection = false; }
        else if(e.type == e_type_ack)                                       ackReceived(e);
        else                                                                recv_q.enqueue(e);

        fg_log("Client    (%c): rec(%d) %s",connection,len,e.debug());
        len -= e_ser_len;
        mess_num++;
      }
    }

    while((send_evt = send_q.next()))
    {
      len = send(sock_fd, send_evt->serialize(), e_ser_len, 0);
      if(len <= 0) { fg_log("Client: abort connection (failed write)"); keep_connection = false; }
      fg_log("Client    (%c): sen(%d) %s",connection,len,send_evt->debug());
      len = 0;
    }
  }
  connected = false;
  connection = '0';
  recv_q.empty();
  send_q.empty();
  ack_q.empty();

  close(sock_fd);
  return 0;
}

void Client::broadcast(char card, char t)
{
  Event e;
  e.connection = connection;
  e.cardinal = card;
  e.type = t;
  e.id_i = nextEventId();
  broadcast(&e);
}

void Client::broadcast(Event *e)
{
  send_q.enqueue(*e); //copy
}

int Client::nextEventId()
{
  if(evt_id_inc == 0) evt_id_inc = (connection-'0');
  evt_id_inc += FG_MAX_CONNECTIONS; //technically, event_id is sufficient enough to know from which connection evt was sent
  return evt_id_inc - FG_MAX_CONNECTIONS;
}

void Client::enqueueAckWait(Event e)
{
  ack_q.enqueue(e);
}

void Client::ackReceived(Event e)
{
  ack_q.get(e); //finds and removes from q. note we don't actually do anything with it
}

void Client::disconnect()
{
  fg_log("Client: abort connection (on purpose)");
  keep_connection = false;
  pthread_join(thread, NULL);
}

bool Client::healthy()
{
  return connected && keep_connection;
}

bool Client::transitioning()
{
  return !healthy() && !stale();
}

bool Client::stale()
{
  return !connecting && !connected && !keep_connection;
}

Event *Client::getEvent()
{
  return recv_q.next();
}

Client::~Client()
{
  if(keep_connection) disconnect();
}
