#include "connection.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

Connection::Connection()
{
  con_state = CONNECTION_STATE_DISCONNECTED;
  ip = getIP();
  port = 8080;

  sock_addr_len = sizeof(con->sock_addr);
  sock_fd = -1;
}

void Connection::connect()
{
  if(con_state != CONNECTION_STATE_DISCONNECTED) { fg_log("Connection: abort connect (connection exists)"); return; }
  con_state = CONNECTION_STATE_CONNECTING;
}

void Connection::recv()
{
  bzero(buff, FG_BUFF_SIZE);
  len = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
  if(len == 0) keep_connection = false;
  int mess_num = 0;
  while(len > 0)
  {
    Event e(buff+(mess_num*e_ser_len));
    if(e.type == e_type_ack) ackReceived(e);
    else recv_q.enqueue(e);
    fg_log("Connection(%c): rec(%d) %s",connection,len,e.debug());
    len-= e_ser_len;
    mess_num++;
  }
}

void Connection::send()
{
  while((send_evt = send_q.next()))
  {
    len = send(sock_fd, send_evt->serialize(), e_ser_len, 0);
    if(len <= 0) { fg_log("Connection: abort connection (failed write)"); keep_connection = false; }
    fg_log("Connection(%c): sen(%d) %s",connection,len,send_evt->debug());
    len = 0;
  }
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

  Load *lp;
  if((lp = recv_q.next()))
  {
    l = *lp;
    return true;
  }
Event *Connection::getEvent()
{
  return recv_q.next();
}

Connection::~Connection()
{
  if(keep_connection) disconnect();
}

