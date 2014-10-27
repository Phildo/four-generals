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
  if(!stale()) { fg_log("Aborting connection connect request- standing connection exists."); return; }
  //port = _port;
  //serv_ip = _ip;

  connecting = true;
  keep_connection = true;

  fg_log("Connection connecting to IP:%s on port %d", serv_ip.ptr(), port);
  int r = pthread_create(&thread, NULL, conThreadHandoff, (void *)&handle);
  if(r != 0) { fg_log("Failure creating connection thread."); connecting = false; keep_connection = false; }
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
    if(len > 0)
    {
      Event e(buff);
      if(e.type == e_type_ack) ackReceived(e);
      else recv_q.enqueue(e);
      fg_log("Serv Received(%d): %s",len,buff);
      len = 0;
    }
    else if(len == 0)
      keep_connection = false;

    while((send_evt = send_q.next()))
    {
      len = send(sock_fd, send_evt->serialize(), send_evt->serlen(), 0);
      if(len <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
      len = 0;
    }
  }
  connected = false;

  close(sock_fd);
  return 0;
}

void Connection::broadcast(char card, char t)
{
  Event e(connection, card, t, nextEventId());
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
  fg_log("Connection disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
}

bool Connection::healthy()
{
  return connected && keep_connection;
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

