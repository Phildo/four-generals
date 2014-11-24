#include "../src/client.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  Network::Client c;
  c.connect("localhost", 8080);
  sleep(10000);
}
