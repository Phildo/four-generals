#include "../src/client.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  Network::Client c;
  c.connect("localhost", 8080);
  for(int i = 0; i < 10000; i++)
  {
    sleep(1);
    if(c.con_state == Network::CONNECTION_STATE_STALE ||
       c.con_state == Network::CONNECTION_STATE_DISCONNECTED)
       i = 10000;
  }
}
