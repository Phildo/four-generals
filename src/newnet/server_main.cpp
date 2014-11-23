#include "server.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  Server s;
  s.connect(8080);
}
