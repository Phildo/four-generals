#ifndef _GAME_H_
#define _GAME_H_

#include "var_array.h"

class Graphics;
class UI;
class Input;
class Network;
class Model;
class Scene;

#define FG_NUM_SCENES 1

class Game
{
  private:
    Graphics* graphics;
    UI* ui;
    Input *input;
    Network* network;
    Model* model;
    Scene* scenes[FG_NUM_SCENES];

  public:
    Game();
    ~Game();

    void run();
};

#endif

