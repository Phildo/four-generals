#ifndef _INRO_SCENE_H_
#define _INRO_SCENE_H_

#include "scene.h"

class IntroScene : public Scene
{
  private:
    Graphics *graphics;
    Network *network;
  public:
    IntroScene(Graphics *g, Network *n);
    void touch(In &in);
    void tick();
    void draw();
};

#endif

