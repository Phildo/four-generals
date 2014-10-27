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

  connection = '0';
  keep_connection = false;
  connecting = false;
  connected = false;
  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  if(!stale()) { fg_log("Aborting client connect request- standing connection exists."); return; }
  port = _port;
  serv_ip = _ip;

  connecting = true;
  keep_connection = true;

  fg_log("Client connecting to IP:%s on port %d", serv_ip.ptr(), port);
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle);
  if(r != 0) { fg_log("Failure creating client thread."); connecting = false; keep_connection = false; }
}

void * Client::fork()
{
  int sock_fd;
  struct sockaddr_in serv_sock_addr; //client's serv addr
  struct hostent *serv_host; //client's reference to server

  int len;
  Event *send_evt;
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host == NULL) { fg_log("Failure finding server."); keep_connection = false; connecting = false; }

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
  serv_sock_addr.sin_port = htons(port);

  if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Failure connecting client.");
    keep_connection = false;
    connecting = false;
    return 0;
  }
  connected = true;
  connecting = false;

  fcntl(sock_fd, F_SETFL, O_NONBLOCK);
  len = 0;
  bzero(buff, FG_BUFF_SIZE);
  while(keep_connection)
  {
    len = recv(sock_fd, buff, FG_BUFF_SIZE-1, 0);
    if(len > 0)
    {
      Event e(buff);
      if     (e.type == e_type_assign_con)                                connection = e.connection;
      else if(e.type == e_type_revoke_con || e.type == e_type_refuse_con) { connection = '0'; keep_connection = false; }
      else if(e.type == e_type_ack)                                       ackReceived(e);
      else                                                                recv_q.enqueue(e);

      fg_log("Cli Received(%d): %s",len,buff);
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
  connection = '0';

  close(sock_fd);
  return 0;
}

void Client::broadcast(char card, char t)
{
  Event e(connection, card, t, nextEventId());
  broadcast(e);
}

void Client::broadcast(Event e)
{
  send_q.enqueue(e);
}

int Client::nextEventId()
{
  if(evt_id_inc == 0) evt_id_inc = connection;
  evt_id_inc+=5; //technically, event_id is sufficient enough to know from which connection evt was sent
  return evt_id_inc - 1;
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
  fg_log("Cient disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
}

bool Client::healthy()
{
  return connected && keep_connection;
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

