#include "network.h"

#include "logger.h"
#include <cstring>

using namespace Network;

String Network::getIP()
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
      if(strcmp(cur->ifa_name, "en0") == 0 || strcmp(cur->ifa_name, "eth0") == 0) ip = String(host, strlen(host));
    }
  }

 freeifaddrs(ap);
 return ip;
}

