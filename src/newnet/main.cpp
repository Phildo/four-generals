#include "id_store.h"
#include "stdio.h"

int main(int argc, char **argv)
{
  IdStore<100> d;
  if(d.test()) printf("Hooray");
  else printf("boo");
}
