#ifndef _INRO_SCENE_H_
#define _INRO_SCENE_H_

#include "scene.h"

class IntroScene : public Scene
{
  private:
    Graphics *graphics;
  public:
    IntroScene(Graphics *g);
    void touch(In &in);
    void tick();
    void draw();
};

#endif

