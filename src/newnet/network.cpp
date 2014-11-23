#include "network.h"

#include "logger.h"
#include <cstring>

using namespace Network;

struct Load
{
  char data[FG_LOAD_BUFF_SIZE];
  char con_id;
};

struct Connection
{
  private:
  public:
    Connection() : sock_addr_len(sizeof(sock_addr)), sock_fd(-1) {}

    char con_id;
    int sock_fd;
    socklen_t sock_addr_len; //sizeof addr
    struct sockaddr_in sock_addr;
};

String getIP()
{
  struct ifaddrs *ap;
  struct ifaddrs *cur;
  int r;

  char host[NI_MAXHOST];
  String ip;

  r = getifaddrs(&ap);
  for(cur = ap; cur != NULL && cur->ifa_addr != NULL; cur = cur->ifa_next)
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

