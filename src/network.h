#ifndef NETWORK_H
#define NETWORK_H

#define MAX_PORT_LENGTH 20
#define MAX_IP_LENGTH 256 //really only need 12, but might as well leave it for urls?

class Network
{
  private:
    bool host_priv;
    char port[MAX_PORT_LENGTH];
    char ip[MAX_IP_LENGTH];
  public:
    Network();
    ~Network();

    void connectAsServer();
    void connectAsClient();
    void broadcast(char *c, int l);
};

#endif

