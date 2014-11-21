#ifndef _FG_INTRO_SCENE_H_
#define _FG_INTRO_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class IntroScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Box border;
    UI::TextButton hostButton;
    UI::TextButton joinButton;
    UI::TextButton howButton;

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

