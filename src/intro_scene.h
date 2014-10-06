#ifndef _INRO_SCENE_H_
#define _INRO_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class IntroScene : public Scene
{
  private:
    Graphics *graphics;
    UI::String ipString;
    UI::Button connectServerButton;
    UI::Button connectClientButton;
  public:
    IntroScene(Graphics *g);
    void touch(In &in);
    void tick();
    void draw();
};

#endif

