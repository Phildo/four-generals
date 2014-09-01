#ifndef _SCENE_H_
#define _SCENE_H_

#include "units.h"
class Graphics;

class Scene
{
  private:
    Graphics* graphics;
  public:
    Scene(Graphics* g);

    virtual void touch(screen_p x,screen_p y);
    virtual void tick();
    virtual void draw();
};

#endif

