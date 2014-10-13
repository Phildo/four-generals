#ifndef _FG_INTRO_SCENE_H_
#define _FG_INTRO_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class IntroScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Label hostLabel; char host[5]; UI::Button hostButton;
    UI::Label joinLabel; char join[5]; UI::Button joinButton;
    UI::Label howLabel;  char how[12]; UI::Button howButton;

    int SCENE_CHANGE_HACK;
  public:
    IntroScene(Graphics *g);
    ~IntroScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

