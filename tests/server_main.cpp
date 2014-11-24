#include "../src/server.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  Network::Server s;
  s.connect(8080);
  sleep(10000);
}
