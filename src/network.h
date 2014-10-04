#ifndef NETWORK_H
#define NETWORK_H

#define MAX_PORT_LENGTH 20
#define MAX_IP_LENGTH 256 //really only need 12, but might as well leave it for urls?

/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
*/

class Network
{
  private:
    bool host_priv;
    char port[MAX_PORT_LENGTH];
    char ip[MAX_IP_LENGTH];

    /*
    int sockfd;
    int newsockfd;
    static const int portno = 8080;
    int n;

    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    struct hostent *server;
    */

  public:
    Network();
    ~Network();

    void connectAsServer();
    void connectAsClient();
    void broadcast(char *c, int l);
};

#endif

