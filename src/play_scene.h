#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"
#include "event.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label sunLabel; UI::Box sunBox;
    UI::Label monLabel; UI::Box monBox;
    UI::Label tueLabel; UI::Box tueBox;
    UI::Label wedLabel; UI::Box wedBox;
    UI::Label thuLabel; UI::Box thuBox;
    UI::Label friLabel; UI::Box friBox;
    UI::Label satLabel; UI::Box satBox;

    UI::Label nLabel; UI::Button nButton;
    UI::Label eLabel; UI::Button eButton;
    UI::Label sLabel; UI::Button sButton;
    UI::Label wLabel; UI::Button wButton;
    UI::Box youBox;

    Network::Event e; //populates with tentative event (holds state for what to show on screen)

    UI::Label actionAttackLabel;  UI::Button actionAttackButton;
    UI::Label actionMessageLabel; UI::Button actionMessageButton;
    UI::Label actionDefendLabel;  UI::Button actionDefendButton;

    UI::Label whatLabel;  UI::Button whatAttackButton; UI::Button whatDefendButton;
    UI::Label whoLabel;   UI::Button whoNButton; UI::Button whoEButton; UI::Button whoSButton; UI::Button whoWButton;
    UI::Label whenLabel;  UI::Button whenSunButton; UI::Button whenMonButton; UI::Button whenTueButton; UI::Button whenWedButton; UI::Button whenThuButton;UI::Button whenFriButton;UI::Button whenSatButton;
    UI::Label whereLabel; UI::Button whereNButton; UI::Button whereEButton; UI::Button whereSButton; UI::Button whereWButton;

    UI::Label confirmLabel; UI::Button confirmButton;
    UI::Label cancelLabel;  UI::Button cancelButton;

    UI::Label debreifLabel;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s_model;
    ServerModel **s_model_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c_model;
    ClientModel **c_model_ptr; //Pointers to the client model pointer 'owned' by game

  public:
    PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm);
    ~PlayScene();
    void enter();
    void touch(In &in);
    int tick();
    void draw();
    void leave();
    void pass();
    void pop();
};

#endif

