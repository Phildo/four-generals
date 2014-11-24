#include "../src/server.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  Network::Server s;
  s.connect(8080);
  for(int i = 0; i < 10000; i++)
  {
    sleep(1);
    if(s.con_state == Network::CONNECTION_STATE_STALE ||
       s.con_state == Network::CONNECTION_STATE_DISCONNECTED)
       i = 10000;
  }
}
