#ifndef NETWORK_H
#define NETWORK_H

namespace Network
{
  void connectAsServer();
  void connectAsClient();
  void broadcast(char *c, int l);
  void disconnect();
  int getIP(char *ip);
  extern char ip[16];
  extern int iplen;
};

#endif

