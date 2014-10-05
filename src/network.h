#ifndef NETWORK_H
#define NETWORK_H

namespace Network
{
  void connectAsServer();
  void connectAsClient();
  void broadcast(char *c, int l);
  void disconnect();
};

#endif

