#ifndef _GAME_H_
#define _GAME_H_

#include "var_array.h"

class Graphics;
class Input;
class Model;
class Scene;

#define FG_NUM_SCENES 1

class Game
{
  private:
    Graphics* graphics;
    Input *input;
    Model* model;
    Scene* scenes[FG_NUM_SCENES];

  public:
    Game();
    ~Game();

    void run();
};

#endif

