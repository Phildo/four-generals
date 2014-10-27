#include "server.h"

#include "defines.h"
#include "logger.h"

#include <strings.h>

using namespace Network;

void * Network::servThreadHandoff(void * arg) { return ((ServThreadHandle*)arg)->server->fork(); }

Server::Server()
{
  ip = getIP();
  port = 8080;
  keep_connection = false;
  connecting = false;
  connected = false;
  handle.server = this;
}

void Server::connect(int _port)
{
  if(!stale()) { fg_log("Aborting server connect request- standing connection exists."); return; }
  port = _port;

  connecting = true;

  fg_log("Server connecting on port %d",port);
  int r = pthread_create(&thread, NULL, servThreadHandoff, (void *)&handle)  ;
  if(r != 0)  { fg_log("Failure creating server thread."); connecting = false; }
}

void * Server::fork()
{
  keep_connection = true;
  int sock_fd;
  struct sockaddr_in sock_addr;

  n_cons = 0;
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) cons[i].connection = '1'+i; //char so con != null ('0' for no connection)
  for(int i = 0; i < FG_MAX_CONNECTIONS; i++) con_ptrs[i] = &cons[i];

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sock_fd, F_SETFL, O_NONBLOCK);

  bzero((char *)&sock_addr, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = INADDR_ANY;
  sock_addr.sin_port = htons(port);

  if(bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
  {
    close(sock_fd);
    fg_log("Failure binding server socket.");
    keep_connection = false;
    connecting = false;
    return 0;
  }
  listen(sock_fd,FG_ACCEPT_Q_SIZE);
  connected = true;
  connecting = false;

  Connection *con;
  Connection *tmp_con_p;
  while(keep_connection) //spin wait. bad idea? tune in next time to find out.
  {
    con = con_ptrs[n_cons];

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
        con->connect();
        con->broadcast('0', e_type_refuse_con);
        con->disconnect(); //race condition!!!

        //find con and put it back at the end of the queue #ugly
        int con_index = 0;
        for(int i = 0; i < n_cons; i++) if(con_ptrs[i] == con) con_index = i;
        n_cons--;

        tmp_con_p = con_ptrs[con_index];
        con_ptrs[con_index] = con_ptrs[n_cons];
        con_ptrs[n_cons] = tmp_con_p;
      }
      else
      {
        con->connect();
        con->broadcast('0', e_type_assign_con);
      }
    }

    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ptrs[i]->stale())
      {
        //clean up/kill thread/connection
        con_ptrs[i]->disconnect();
        n_cons--;

        //put newly cleaned con at end of list
        tmp_con_p = con_ptrs[i];
        con_ptrs[i] = con_ptrs[n_cons];
        con_ptrs[n_cons] = tmp_con_p;

        i--;
      }
    }
  }
  connected = false;

  for(int i = 0; i < n_cons; i++)
    con_ptrs[i]->disconnect();
  n_cons = 0;

  close(sock_fd);
  return 0;
}

void Server::broadcast(Event e)
{
  fg_log("Serv should broadcast %s",e.serialize());
  //send_q.enqueue(e);
}

void Server::disconnect()
{
  fg_log("Server disconnecting");
  keep_connection = false;
  pthread_join(thread, NULL);
}

bool Server::healthy()
{
  return connected && keep_connection;
}

bool Server::stale()
{
  return !connecting && !connected && !keep_connection;
}


Event *Server::getEvent()
{
  return 0;
}

Server::~Server()
{
  if(keep_connection) disconnect();
}

