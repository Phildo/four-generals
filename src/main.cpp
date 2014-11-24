#include "game.h"
#include "logger.h"

#include <SDL.h>
#include <signal.h>
#include <stdio.h>

//So lame
void signal_callback_handler(int signum)
{
  fg_log("Caught signal %d\n",signum);
  switch(signum)
  {
    case SIGSEGV:
      fflush(stdout);
      exit(1);
      break;
    case SIGPIPE:
      fflush(stdout);
      //do nothing
      break;
    default:
      //also do nothing
      break;
  }

}

int main(int argc, char* argv[])
{
  signal(SIGPIPE, signal_callback_handler);
  signal(SIGSEGV, signal_callback_handler);

  SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Game g;
  g.run();

  SDL_Quit();
  return 0;
}

