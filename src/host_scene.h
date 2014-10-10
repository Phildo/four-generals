#ifndef _HOST_SCENE_H_
#define _HOST_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class HostScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Label nLabel;
    UI::Label sLabel;
    UI::Label eLabel;
    UI::Label wLabel;
    UI::Button nButton;
    UI::Button sButton;
    UI::Button eButton;
    UI::Button wButton;

    char ip[16]; int iplen;
    UI::Label ipLabel;
  public:
    HostScene(Graphics *g);
    ~HostScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

