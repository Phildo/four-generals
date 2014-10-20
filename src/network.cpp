#include "network.h"

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "defines.h"
#include "logger.h"

using namespace Network;

bool host_priv = true;

String Network::getIP()
{
  struct ifaddrs *ap;
  struct ifaddrs *cur;
  int r;

  char host[NI_MAXHOST];
  String ip;

  r = getifaddrs(&ap);
  for(cur = ap; cur != NULL; cur = cur->ifa_next)
  {
    r = cur->ifa_addr->sa_family;

    if(r == AF_INET)
    {
      r = getnameinfo(cur->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if(strcmp(cur->ifa_name, "en0") == 0) ip = String(host, strlen(host));
    }
  }

 freeifaddrs(ap);
 return ip;
}

void * Network::servThreadHandoff(void * arg) { return ((ServThreadHandle*)arg)->server->fork(); }
void * Network::conThreadHandoff(void * arg)  { return ((ConThreadHandle*) arg)->connection->fork(); }
void * Network::cliThreadHandoff(void * arg)  { return ((CliThreadHandle*) arg)->client->fork(); }

Event::Event()               : null('\0') {};
Event::Event(const Event &e) : null('\0')
{
  connection = e.connection;
  cardinal = e.cardinal;
  type = e.type;
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = e.id_c[i];
  id_i = e.id_i;
}
Event & Event::operator=(const Event &e)
{
  connection = e.connection;
  cardinal = e.cardinal;
  type = e.type;
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = e.id_c[i];
  id_i = e.id_i;
  return *this;
} //no need to set null
Event::Event(char con, char card, char t, int id) : connection(con), cardinal(card), type(t), id_c("00000"), null('\0'), id_i(id) { id_c[5] = '0'; /*would be '\0' fron initialiazer*/ }

char *Event::serialize()
{
  //fill id_c with 6 digit char rep of id (ie '000012')
  int tmp_left = id_i;
  int tmp_this = 0;
  for(int i = FG_EVT_MAX_DEC_LEN; i < 0; i++)
  {
    tmp_this = tmp_left%10;
    id_c[i] = '0'+tmp_this;
    tmp_left -= tmp_this; //prob don't need to do because of truncation in division
    tmp_left /= 10;
  }

  return (char *)&connection;
}
Event::Event(char *c) : null('\0')
{
  connection = c[0];
  cardinal = c[1];
  type = c[2];
  for(int i = 0; i < FG_EVT_MAX_DEC_LEN; i++)
    id_c[i] = c[3+i];
  id_i = atoi(id_c);
}
int Event::serlen() { return 9; }

Server::Server()
{
  ip = getIP();
  port = 8080;
  connected = false;
  keep_connection = false;
  handle.server = this;
}

void Server::connect(int _port)
{
  if(connected || keep_connection) { fg_log("Aborting server connect request- standing connection unhealthy."); return; }
  port = _port;

  fg_log("Server connecting on port %d",port);
  keep_connection = true;
  int r = pthread_create(&thread, NULL, servThreadHandoff, (void *)&handle)  ;
  if(r != 0) { fg_log("Failure creating server thread."); keep_connection = false; }
}

void * Server::fork()
{
  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) cons[i].connection = '1'+i; //char so con != null ('0' for no connection)
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ps[i] = &cons[i];

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sock_fd, F_SETFL, O_NONBLOCK);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
  {
    fg_log("Failure binding server socket.");
    keep_connection = false;
    return 0;
  }
  listen(sock_fd,FG_ACCEPT_Q_SIZE);
  connected = true;

  Connection *con;
  Connection *tmp_con_p;
  while(keep_connection) //spin wait. bad idea? tune in next time to find out.
  {
    con = con_ps[n_cons];

    con->stale = false;
    con->welcome = true;
    con->sock_addr_len = sizeof(con->sock_addr);
    con->sock_fd = -1;

    con->sock_fd = accept(sock_fd, (struct sockaddr *)&(con->sock_addr), &(con->sock_addr_len));
    if(con->sock_fd >= 0)
    {
      n_cons++;

      //Final connection will be told off and closed
      //Wait for that to happen before accepting any more
      if(n_cons == FG_MAX_CONNECTIONS)
      {
        con->welcome = false;
        int r = pthread_create(&(con->thread), NULL, conThreadHandoff, (void *)&con->handle);
        if(r != 0) fg_log("Failure creating connection thread.");
        con->disconnect();

        //find con and put it back at the end of the queue #ugly
        int con_index = 0;
        for(int i = 0; i < n_cons; i++) if(con_ps[i] == con) con_index = i;
        n_cons--;

        tmp_con_p = con_ps[con_index];
        con_ps[con_index] = con_ps[n_cons];
        con_ps[n_cons] = tmp_con_p;
      }
      else
      {
        int r = pthread_create(&(con->thread), NULL, conThreadHandoff, (void *)&con->handle);
        if(r != 0) { fg_log("Failure creating connection thread."); keep_connection = false; }
      }
    }

    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ps[i]->stale)
      {
        //clean up/kill thread/connection
        con_ps[i]->disconnect();
        n_cons--;

        //put newly cleaned con at end of list
        tmp_con_p = con_ps[i];
        con_ps[i] = con_ps[n_cons];
        con_ps[n_cons] = tmp_con_p;

        i--;
      }
    }
  }
  connected = false;

  for(int i = 0; i < n_cons; i++)
    con_ps[i]->disconnect();
  n_cons = 0;

  return 0;
}

void Server::broadcast(const String &s)
{
  fg_log("Serv should broadcast %s",s.ptr());
}

void Server::disconnect()
{
  fg_log("Server disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
  close(sock_fd);
}

bool Server::healthy()
{
  return connected && keep_connection;
}

Server::~Server()
{
  if(keep_connection) disconnect();
}


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

Connection::Connection()
{
  connection = 0;
  connected = false;
  keep_connection = false;
  handle.connection = this;
}

void * Connection::fork()
{
  if(!welcome)
  {
    keep_connection = false;
    fg_log("Connection unwelcome");
    Event e(connection, '0', e_type_refuse_con, nextEventId());
    writlen = send(sock_fd, e.serialize(), e.serlen(), 0);
    if(writlen <= 0) fg_log("Failure writing connection.");
  }
  else
  {
    keep_connection = true;
    fg_log("Connection created");
    Event e(connection, '0', e_type_assign_con, nextEventId());
    writlen = send(sock_fd, e.serialize(), e.serlen(), 0);
    if(writlen <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
  }
  connected = true;

  readlen = 0;
  writlen = 0;
  bzero(read, FG_BUFF_SIZE);
  bzero(writ, FG_BUFF_SIZE);
  while(keep_connection)
  {
    readlen = recv(sock_fd, read, FG_BUFF_SIZE-1, 0);
    if(readlen > 0)
    {
      Event e(read);
      if(e.type == e_type_ack) ackReceived(e.id_i);
      else
      {
        //needs to be processed and potentially re-broadcast
      }
      fg_log("Serv Received %d: %s",readlen,read);
      readlen = 0;
    }
    else if(readlen == 0)
      keep_connection = false;

    if(writlen > 0)
    {
      writlen = send(sock_fd, writ, writlen, 0);
      if(writlen <= 0) { fg_log("Failure writing connection."); keep_connection = false; }
      writlen = 0;
    }
  }
  connected = false;
  stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

int Connection::nextEventId()
{
  evt_id_inc++;
  return evt_id_inc - 1;
}

void Connection::enqueueAckWait(Event e)
{
  ack_q[ack_q_back] = e;
  ack_q_back = (ack_q_back+1)%FG_EVT_Q_SIZE;
}

void Connection::ackReceived(int id)
{
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

