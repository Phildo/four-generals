#ifndef _FG_JOIN_SCENE_H_
#define _FG_JOIN_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;

class JoinScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label joinGameLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Label joinSessLabel;
    UI::Button sessionButton;

    UI::Label inRoomLabel;

    Network::Client *client;

    //Pointer to the client pointer 'owned' by game
    Network::Client **clientPtr;

    int SCENE_CHANGE_HACK;
  public:
    JoinScene(Graphics *g, Network::Client *& c);
    ~JoinScene();
    void touch(In &in);
    int tick();
    void draw();
};

#endif

