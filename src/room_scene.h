#ifndef _FG_ROOM_SCENE_H_
#define _FG_ROOM_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"

class Graphics;
class ServerModel;
class ClientModel;

class RoomScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label inRoomLabel;
    UI::Label ipLabel;
    UI::Label portLabel;

    UI::Button cardBtns[4];
    UI::Image  cardImgs[4];
    UI::Label  cardLbls[4];

    UI::TextButton leaveSessButton;
    UI::TextButton beginGameButton;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    int SCENE_CHANGE_HACK;
    SDL_Rect whoBoxForPosition(char c);
  public:
    RoomScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm);
    ~RoomScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

