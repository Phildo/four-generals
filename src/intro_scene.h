#ifndef _INRO_SCENE_H_
#define _INRO_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class IntroScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Label hostLabel;
    UI::Button hostButton;

    UI::Label joinLabel;
    UI::Button joinButton;

    UI::Label howLabel;
    UI::Button howButton;
  public:
    IntroScene(Graphics *g);
    void touch(In &in);
    void tick();
    void draw();
};

#endif

