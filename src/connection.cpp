#include "connection.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::conThreadHandoff(void * arg)  { return ((ConThreadHandle*) arg)->connection->fork(); }

Connection::Connection()
{
  ip = getIP();
  port = 8080;

  evt_id_inc = 0;
  connection = '0';
  keep_connection = false;
  connecting = false;
  connected = false;
  handle.connection = this;
}

void Connection::connect()
{
  if(!stale()) { fg_log("Connection: abort connect (connection exists)"); return; }
  //port = _port;
  //serv_ip = _ip;

  connecting = true;
  keep_connection = true;

  fg_log("Connection: connect (%s:%d)", serv_ip.ptr(), port);
  int r = pthread_create(&thread, NULL, conThreadHandoff, (void *)&handle);
  if(r != 0) { fg_log("Connection: abort connect (failed fork)"); connecting = false; keep_connection = false; }
}

void * Connection::fork()
{
  connected = true;
  connecting = false;
  int len;
  Event *send_evt;

  fcntl(sock_fd, F_SETFL, O_NONBLOCK);
  len = 0;
  bzero(buff, FG_BUFF_SIZE);
  while(keep_connection)
  {
    len = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
    if(len == 0) keep_connection = false;
    int mess_num = 0;
    while(len > 0)
    {
      Event e(buff+(mess_num*e_ser_len));
      if(e.type == e_type_ack) ackReceived(e);
      else recv_q.enqueue(e);
      fg_log("Connection(%d): rec(%d) %s",connection,len,buff+(mess_num*e_ser_len));
      len-= e_ser_len;
      mess_num++;
    }

    while((send_evt = send_q.next()))
    {
      len = send(sock_fd, send_evt->serialize(), e_ser_len, 0);
      if(len <= 0) { fg_log("Connection: abort connection (failed write)"); keep_connection = false; }
      fg_log("Connection(%d): sen(%d) %s",connection,len,send_evt->serialize());
      len = 0;
    }
  }
  connected = false;
  recv_q.empty();
  send_q.empty();
  ack_q.empty();

  close(sock_fd);
  return 0;
}

void Connection::broadcast(char con, char card, char t)
{
  Event e(con, card, t, nextEventId());
  broadcast(e);
}

void Connection::broadcast(Event e)
{
  send_q.enqueue(e);
}

int Connection::nextEventId()
{
  if(evt_id_inc == 0) evt_id_inc = (connection-'0');
  evt_id_inc += FG_MAX_CONNECTIONS; //technically, event_id is sufficient enough to know from which connection evt was sent
  return evt_id_inc - FG_MAX_CONNECTIONS;
}

void Connection::enqueueAckWait(Event e)
{
  ack_q.enqueue(e);
}

void Connection::ackReceived(Event e)
{
  ack_q.get(e); //finds and removes from q. note we don't actually do anything with it
}

void Connection::disconnect()
{
  fg_log("Connection: abort connection (on purpose)");
  keep_connection = false;
  pthread_join(thread, NULL);
}

bool Connection::healthy()
{
  return connected && keep_connection;
}

bool Connection::transitioning()
{
  return !healthy() && !stale();
}

bool Connection::stale()
{
  return !connecting && !connected && !keep_connection;
}

Event *Connection::getEvent()
{
  return recv_q.next();
}

Connection::~Connection()
{
  if(keep_connection) disconnect();
}

