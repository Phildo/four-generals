#ifndef NETWORK_H
#define NETWORK_H

namespace Network
{
  void connectAsServer(int port);
  void connectAsClient(char *ip, int port);
  void broadcast(char *c, int l);
  void disconnect();
  void getIP(char *ip, int *len);

  #define MAX_IP_LENGTH 16
  extern char ip[MAX_IP_LENGTH];
  extern int iplen;
  extern int port;
};

#endif

