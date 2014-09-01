#ifndef _GAME_H_
#define _GAME_H_

#include "var_array.h"
#include "scene.h"

class Graphics;
class Model;

class Game
{
  private:
    Graphics* graphics;
    Model* model;
    Scene** scenes;

  public:
    Game();
    ~Game();

    void run();
};

#endif

