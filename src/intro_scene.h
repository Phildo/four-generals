#ifndef _FG_INTRO_SCENE_H_
#define _FG_INTRO_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class IntroScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Label hostLabel; UI::Button hostButton;
    UI::Label joinLabel; UI::Button joinButton;
    UI::Label howLabel;  UI::Button howButton;

    int SCENE_CHANGE_HACK;
  public:
    IntroScene(Graphics *g);
    ~IntroScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

