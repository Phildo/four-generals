#include "connection.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::conThreadHandoff(void * arg)  { return ((ConThreadHandle*) arg)->connection->fork(); }

Connection::Connection()
{
  connection = 0;
  connected = false;
  keep_connection = false;
  handle.connection = this;
}

void * Connection::fork()
{
  int len;
  Event *send_evt;
  if(!welcome)
  {
    keep_connection = false;
    fg_log("Connection unwelcome");
    Event e(connection, '0', e_type_refuse_con, nextEventId());
    len = send(sock_fd, e.serialize(), e.serlen(), 0);
    if(len <= 0) fg_log("Failure writing connection.");
  }
  else
  {
    keep_connection = true;
    fg_log("Connection created");
    Event e(connection, '0', e_type_assign_con, nextEventId());
    len = send(sock_fd, e.serialize(), e.serlen(), 0);
    if(len <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
  }
  connected = true;

  len = 0;
  bzero(buff, FG_BUFF_SIZE);
  while(keep_connection)
  {
    len = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
    if(len > 0)
    {
      Event e(buff);
      recv_q.enqueue(e);
      if(e.type == e_type_ack) ackReceived(e.id_i);
      else
      {
        //needs to be processed and potentially re-broadcast
      }
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
  stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

void Connection::broadcast(Event e)
{
  send_q.enqueue(e);
}

int Connection::nextEventId()
{
  evt_id_inc++;
  return evt_id_inc - 1;
}

void Connection::enqueueAckWait(Event e)
{
  ack_q.enqueue(e);
}

void Connection::ackReceived(int id)
{
  /*
  for(int i = ack_q_front; i != ack_q_back; i = (i+1)%FG_EVT_Q_SIZE)
  {
    if(ack_q[i].id_i == id)
    {
      Event e = ack_q[ack_q_front];
      ack_q[ack_q_front] = ack_q[i];
      ack_q[i] = e;
      ack_q_front = (ack_q_front+1)%FG_EVT_Q_SIZE;
      return;
    }
  }
  */
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
  return connected && keep_connection;
}

Connection::~Connection()
{
  if(keep_connection) disconnect();
}

