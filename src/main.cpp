#include "game.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL.h>

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS);

  Game g;
  g.run();

  SDL_Quit();
  return 0;
}

