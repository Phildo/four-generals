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
      if(strcmp(cur->ifa_name, "en0") == 0) ip = String(host, strlen(host));
    }
  }

 freeifaddrs(ap);
 return ip;
}


String Network::decimalRep(int i, int digs)
{
  char rep[32];
  char *r;
  if(digs > 32) r = new char[digs]; //only put it on heap if necessary
  else r = &rep[0];

  int d = 0;
  for(int j = digs-1; j >= 0; j--)
  {
    d = i%10;
    r[j] = '0'+d;
    i -= d; //prob don't need to do because of truncation in division
    i /= 10;
  }

  String s(r, digs);
  if(r != &rep[0]) delete[] r;
  return s;
}

int Network::intVal(char *s, int digs)
{
  int r = 0;
  int d;
  for(int i = digs-1; i >= 0; i--)
  {
    d = ((int)(s[i]-'0'));
    r *= 10;
    r += d;
  }
  return r;
}

