#ifndef _FG_HOST_SCENE_H_
#define _FG_HOST_SCENE_H_

#include "scene.h"
#include "ui.h"
class Graphics;

class HostScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label hostGameLabel; char hostGame[10];
    UI::Label ipLabel;       char ip[16];  int iplen;
    UI::Label portLabel;     char port[6]; int portlen;

    UI::Label nLabel; char n; UI::Label npLabel; char np; UI::Button nButton;
    UI::Label sLabel; char s; UI::Label spLabel; char sp; UI::Button sButton;
    UI::Label wLabel; char w; UI::Label wpLabel; char wp; UI::Button wButton;
    UI::Label eLabel; char e; UI::Label epLabel; char ep; UI::Button eButton;

    UI::Label startSessLabel;  char startSess[14];
    UI::Label cancelSessLabel; char cancelSess[15];
    UI::Label joinSessLabel;   char joinSess[13];
    UI::Label leaveSessLabel;  char leaveSess[14];
    UI::Button sessionButton;

    UI::Label inRoomLabel; char inRoom[8];

  public:
    HostScene(Graphics *g);
    ~HostScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

