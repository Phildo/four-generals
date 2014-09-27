#include "network.h"

#include "defines.h"
#include "logger.h"

#ifdef FG_ANDROID
#include "android_network.cpp"
#endif
#ifdef FG_IOS
#include "ios_network.cpp"
#endif
#ifdef FG_PC
#include "pc_network.cpp"
#endif

Network::Network()
{
  #ifdef FG_ANDROID
    AndroidNetwork::construct();
  #endif
}

Network::~Network()
{
  #ifdef FG_ANDROID
    AndroidNetwork::destruct();
  #endif
}

void Network::connectAsServer()
{
  #ifdef FG_ANDROID
    AndroidNetwork::connectAsServer();
  #endif
}

void Network::connectAsClient()
{

}

void Network::broadcast(char *c, int l)
{
  #ifdef FG_ANDROID
    AndroidNetwork::broadcast(c,l);
  #endif
}

