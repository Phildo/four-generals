#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "network.h"
#include "event.h"

#include "compass.h"
#include "week.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
{
  private:
    Graphics *graphics;

    UI::Button backButton;

    UI::Label dayLbls[7];
    UI::Box   dayBoxs[7];

    UI::Label cardLbls[4];
    UI::Button cardBtns[4];
    UI::Box youBox;

    UI::Label actionAttackLabel;  UI::Button actionAttackButton;
    UI::Label actionMessageLabel; UI::Button actionMessageButton;
    UI::Label actionDefendLabel;  UI::Button actionDefendButton;

    UI::Label whatLabel;  UI::Button whatAttackButton; UI::Label whatAttackLabel; UI::Button whatDefendButton; UI::Label whatDefendLabel;
    UI::Label whoLabel;   UI::Button whoBtns[4];
    UI::Label whenLabel;  UI::Button whenBtns[7];
    UI::Label whereLabel; UI::Button whereBtns[4];

    UI::Label confirmLabel; UI::Button confirmButton;
    UI::Label cancelLabel;  UI::Button cancelButton;

    UI::Label debreifLabel;
    UI::Label waitingLabel;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    char known_day;

    Event e; //populates with tentative event (holds state for what to show on screen)
    void zeroE();
    void chooseAction(In &in);
    void chooseWhat(In &in);
    void chooseWho(In &in);
    void chooseWhen(In &in);
    void chooseWhere(In &in);
    void seekConfirmation(In &in);

    void drawAction();
    void drawWhat();
    void drawWho();
    void drawWhen();
    void drawWhere();
    void drawConfirmation();
    void drawWaiting();
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

