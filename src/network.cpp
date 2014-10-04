#include "network.h"

#include "defines.h"
#include "logger.h"

Network::Network()
{
}

Network::~Network()
{
}

void Network::connectAsServer()
{
  /*
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) fg_log("Nope");

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    fg_log("Nope");
  listen(sockfd,5);

  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
  if(newsockfd < 0) fg_log("Nope");

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 255);
  if(n < 0) fg_log("Nope");

  printf("got dat message: %s\n",buffer);
  n = write(newsockfd,"I gotchu",8);
  if(n < 0) fg_log("Nope");

  close(newsockfd);
  close(sockfd);
  */
}

void Network::connectAsClient()
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
}

void Network::broadcast(char *c, int l)
{
}

