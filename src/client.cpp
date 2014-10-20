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
  evt_q_front = 0;
  evt_q_back = 0;
  connected = false;
  keep_connection = false;
  handle.client = this;
}

void Client::connect(const String &_ip, int _port)
{
  if(connected || keep_connection) { fg_log("Aborting client connect request- standing connection unhealthy."); return; }
  port = _port;
  serv_ip = _ip;

  fg_log("Client connecting to IP:%s on port %d", serv_ip.ptr(), port);
  keep_connection = true;
  int r = pthread_create(&thread, NULL, cliThreadHandoff, (void *)&handle)  ;
  if(r != 0) { fg_log("Failure creating client thread."); keep_connection = false; }
}

void * Client::fork()
{
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  serv_host = gethostbyname(serv_ip.ptr());
  if(serv_host == NULL) { fg_log("Failure finding server."); keep_connection = false; }

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  bcopy((char *)serv_host->h_addr, (char *)&serv_sock_addr.sin_addr.s_addr, serv_host->h_length);
  serv_sock_addr.sin_port = htons(port);

  if(::connect(sock_fd,(struct sockaddr *)&serv_sock_addr, sizeof(serv_sock_addr)) < 0)
  {
    fg_log("Failure connecting client.");
    keep_connection = false;
    return 0;
  }
  connected = true;

  fcntl(sock_fd, F_SETFL, O_NONBLOCK);
  readlen = 0;
  writlen = 0;
  bzero(read, FG_BUFF_SIZE);
  bzero(writ, FG_BUFF_SIZE);
  while(keep_connection)
  {
    readlen = recv(sock_fd, read, FG_BUFF_SIZE-1, 0);
    if(readlen > 0)
    {
      enqueueEvent(Event(read));
      fg_log("Cli Received(%d): %s",readlen,read);
      readlen = 0;
    }

    if(writlen > 0)
    {
      writlen = send(sock_fd, writ, writlen, 0);
      if(writlen <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
      writlen = 0;
    }
  }
  connected = false;
  return 0;
}

void Client::broadcast(const String &s)
{
  fg_log("Cli should broadcast %s",s.ptr());
}

void Client::disconnect()
{
  fg_log("Cient disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
  close(sock_fd);
}

bool Client::healthy()
{
  return connected && keep_connection;
}

void Client::enqueueEvent(Event e)
{
  evt_q[evt_q_back] = e;
  evt_q_back = (evt_q_back+1)%FG_EVT_Q_SIZE;
}

Event *Client::getEvent()
{
  if(evt_q_front == evt_q_back) return 0;
  int tmp = evt_q_front;
  evt_q_front = (evt_q_front+1)%FG_EVT_Q_SIZE;
  return &evt_q[tmp];
}

Client::~Client()
{
  if(keep_connection) disconnect();
}

