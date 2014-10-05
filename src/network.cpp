#include "network.h"

#include "defines.h"
#include "logger.h"

const bool host_priv = true;
const int portno = 8080;

bool listening = false;
char ip[MAX_IP_LENGTH];

bool should_disconnect = false;

int serv_sock_fd;
struct sockaddr_in serv_sock_addr;
pthread_t serv_thread;

int n_cons;
Connection cons[MAX_CONNECTIONS];

struct hostent *server;

void * serverThread(void * arg);
void * connectionThread(void * arg);
void * clientThread(void * arg);

void Network::connectAsServer()
{
  listening = true;
  n_cons = 0;
  serv_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&serv_sock_addr, sizeof(serv_sock_addr));
  serv_sock_addr.sin_family = AF_INET;
  serv_sock_addr.sin_addr.s_addr = INADDR_ANY;
  serv_sock_addr.sin_port = htons(portno);

  if(bind(serv_sock_fd, (struct sockaddr *) &serv_sock_addr, sizeof(serv_sock_addr)) < 0) fg_log("Failure binding server socket.");
  listen(serv_sock_fd,5);

  int r = pthread_create(&serv_thread, NULL, serverThread, NULL)  ;
  if(r != 0) fg_log("Failure creating server thread.");
}

void * serverThread(void * arg)
{
  while(!should_disconnect)
  {
    cons[n_cons].connection = n_cons;
    cons[n_cons].sock_addr_len = sizeof(cons[n_cons].sock_addr);
    cons[n_cons].sock_fd = accept(serv_sock_fd, (struct sockaddr *)&cons[n_cons].sock_addr, &cons[n_cons].sock_addr_len);
    if(cons[n_cons].sock_fd < 0) fg_log("Failure accepting connection.");

    int r = pthread_create(&cons[n_cons].thread, NULL, connectionThread, (void *)(&cons[n_cons]));
    if(r != 0) fg_log("Failure creating connection thread.");
    n_cons++;

    //Final connection will be told off and closed
    //Wait for that to happen before accepting any more
    if(n_cons == MAX_CONNECTIONS)
    {
      pthread_join(cons[n_cons-1].thread, NULL);
      n_cons--;
    }
  }
  for(int i = 0; i < n_cons; i++)
  {
    pthread_join(cons[i].thread, NULL);
    close(cons[i].sock_fd);
  }
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

