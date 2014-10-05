#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

extern "C"
{
  #include <pthread.h>
  #include <unistd.h>
}

#include "defines.h"
#include "logger.h"

#define MAX_IP_LENGTH 256 //really only need 12, but might as well leave it for urls?

#define MAX_CONNECTIONS 5 //hold 5th to inform it of its rejection
#define BUFF_SIZE 256

struct Connection
{
  int connection; //0-MAX_CONNECTIONS
  bool stale;

  int sock_fd;
  socklen_t sock_addr_len; //sizeof addr
  struct sockaddr_in sock_addr;

  pthread_t thread;
  char read_buff[BUFF_SIZE];
  char write_buff[BUFF_SIZE];
};

void * serverThread(void * arg);
void * connectionThread(void * arg);
void * clientThread(void * arg);

const bool host_priv = true;
int portno = 8080;
char ip[MAX_IP_LENGTH];

bool listening = false;

bool should_disconnect = false;

int serv_sock_fd;
struct sockaddr_in serv_sock_addr;
pthread_t serv_thread;

int n_cons;

//an odd pattern-
//holds connection memory contiguously on stack
//pointers to the above memory, Q'd w/ emtpy connctions last
Connection cons[MAX_CONNECTIONS];
Connection *con_ps[MAX_CONNECTIONS];

struct hostent *server;

void Network::connectAsServer()
{
  listening = true;
  int r = pthread_create(&serv_thread, NULL, serverThread, NULL)  ;
  if(r != 0) fg_log("Failure creating server thread.");
}

void * serverThread(void * arg)
{
  n_cons = 0;
  for(int i = 0; i < MAX_CONNECTIONS; i++) cons[i].connection = i;
  for(int i = 0; i < MAX_CONNECTIONS; i++) con_ps[i] = &cons[i];

  serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
  serv_sock_addr.sin_port = htons(portno);

  if(bind(serv_sock_fd, (struct sockaddr *) &serv_sock_addr, sizeof(serv_sock_addr)) < 0) fg_log("Failure binding server socket.");
  listen(serv_sock_fd,5);

  Connection *tmp_con_p;
  while(!should_disconnect)
  {
    //check for disconnected cons
    for(int i = 0; i < n_cons; i++)
    {
      if(con_ps[i]->stale)
      {
        //clean up/kill thread/connection
        pthread_join(con_ps[i]->thread, NULL);
        close(con_ps[i]->sock_fd);
        n_cons--;

        //put newly cleaned con at end of list
        tmp_con_p = con_ps[i];
        con_ps[i] = con_ps[n_cons];
        con_ps[n_cons] = tmp_con_p;
      }
    }
    int n = con_ps[n_cons]->connection; //n = index of next available con

    cons[n].stale = false;
    cons[n].sock_addr_len = sizeof(cons[n].sock_addr);
    cons[n].sock_fd = accept(serv_sock_fd, (struct sockaddr *)&cons[n].sock_addr, &cons[n].sock_addr_len);
    if(cons[n].sock_fd < 0) fg_log("Failure accepting connection.");

    int r = pthread_create(&cons[n].thread, NULL, connectionThread, (void *)(&cons[n]));
    if(r != 0) fg_log("Failure creating connection thread.");
    n_cons++;

    //Final connection will be told off and closed
    //Wait for that to happen before accepting any more
    if(n_cons == MAX_CONNECTIONS)
    {
      pthread_join(cons[n].thread, NULL);
      close(cons[n].sock_fd);
      n_cons--;
    }
  }

  for(int i = 0; i < n_cons; i++)
  {
    pthread_join(con_ps[i]->thread, NULL);
    close(con_ps[i]->sock_fd);
  }
  n_cons = 0;

  close(serv_sock_fd);
  return 0;
}

void * connectionThread(void * arg)
{
  Connection *con = (Connection *)arg;
  int n = 1;

  while(!should_disconnect && n >= 0)
  {
    bzero(con->read_buff, BUFF_SIZE);
    n = read(con->sock_fd, con->read_buff, BUFF_SIZE-1);
    if(n < 0) fg_log("Failure reading connection.");
    else
    {
      fg_log("Received: %s",con->read_buff);

      //ack
      n = write(con->sock_fd,"I gotchu\n",9);
      if(n < 0) fg_log("Failure writing connection.");
    }
  }
  con->stale = true; //cheap way to alert server to kill this thread at its convenience
  return 0;
}

void Network::connectAsClient()
{
  listening = false;
}

void * clientThread(void *arg)
{
  /*
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) fg_log("Nope");

  server = gethostbyname("blah.com");//should take raw ip
  if(server == NULL) fg_log("Nope");

  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;

  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    fg_log("Nope");

  printf("Massage?: ");
  bzero(buffer,256);
  fgets(buffer,255,stdin);
  n = write(sockfd, buffer, strlen(buffer));
  if(n < 0) fg_log("Nope");

  bzero(buffer, 256);
  n = read(sockfd,buffer,255);
  if(n < 0) fg_log("Nope");

  printf("%s\n",buffer);
  close(sockfd);
  */
  return 0;
}

void Network::broadcast(char *c, int l)
{
}

void Network::disconnect()
{
  should_disconnect = true;
  if(listening) pthread_join(serv_thread, NULL); //main thread waits for serv_thread to finish
}

