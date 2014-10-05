#include "game.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>
#include <signal.h>

//So lame
void signal_callback_handler(int signum){ fg_log("Caught signal SIGPIPE %d\n",signum); }

int main(int argc, char* argv[])
{
  signal(SIGPIPE, signal_callback_handler); //necessary for network

  SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Game g;
  g.run();

  SDL_Quit();
  return 0;
}

