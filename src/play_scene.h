#ifndef _FG_PLAY_SCENE_H_
#define _FG_PLAY_SCENE_H_

#include "scene.h"
#include "ui.h"
#include "particles.h"
#include "lerp_rect.h"
#include "network.h"
#include "event.h"

class Graphics;
class ServerModel;
class ClientModel;

class PlayScene : public Scene
{
  private:
    Graphics *graphics;
    ParticleSys psys;

    //sprites
    SDL_Rect generals_s[4];
    SDL_Rect force_field_s;
    SDL_Rect shield_full_s;
    SDL_Rect shield_broken_s;
    SDL_Rect sword_s;
    SDL_Rect red_x_s;
    SDL_Rect envelope_s;
    SDL_Rect sun_s;

    //rects
    SDL_Rect positionRects[4];
    SDL_Rect positionStatusRects[4];
    SDL_Rect positionHealthRects[4];
    SDL_Rect dayRects[7];
    SDL_Rect sunRects[7];

    UI::Label dayLbls[7];

    UI::Image cardImgs[4];

    UI::TextButton actionAttackButton;
    UI::TextButton actionMessageButton;
    UI::TextButton actionDefendButton;
    UI::TextButton actionSabotageButton;

    UI::Label howLabel;   UI::TextButton howBlockButton; UI::TextButton howReadButton; UI::TextButton howSabotageButton;
    UI::Label whichLabel; UI::TextButton whichWhatButton; UI::TextButton whichWhoButton; UI::TextButton whichWhenButton; UI::TextButton whichWhereButton;
    UI::Label whatLabel;  UI::TextButton whatAttackButton; UI::TextButton whatDefendButton;
    UI::Label whoLabel;   UI::Button whoBtns[4];
    UI::Label whenLabel;  UI::Button whenBtns[7];
    UI::Label whereLabel; UI::Button whereBtns[4];

    UI::Button sunBtn;
    bool sunDragging;

    UI::TextButton confirmButton;
    UI::TextButton cancelButton;
    UI::TextButton resetButton;

    UI::Label debreifLabel;
    UI::Label waitingLabel;
    UI::Label messageLabel;
    UI::Label sabotageLabel;
    UI::Label winLabel;
    UI::Label loseLabel;

    Network::Client *client;
    Network::Client **client_ptr; //Pointers to the client pointer 'owned' by game
    ServerModel *s;
    ServerModel **s_ptr; //Pointers to the server model pointer 'owned' by game
    ClientModel *c;
    ClientModel **c_ptr; //Pointers to the client model pointer 'owned' by game

    char known_day;
    float anim_day;
    float shown_day;

    Event e; //populates with tentative event (holds state for what to show on screen)
    void zeroE();
    void chooseAction(In &in);
    void chooseShownDay(In &in);
    void chooseHow(In &in);
    void chooseWhich(In &in);
    void chooseWhat(In &in);
    void chooseWho(In &in);
    void chooseWhen(In &in);
    void chooseWhere(In &in);
    void seekConfirmation(In &in);

    void drawAction();
    void drawHow();
    void drawWhich();
    void drawWhat();
    void drawWho();
    void drawWhen();
    void drawWhere();
    void drawConfirmation();
    void drawMessage();
    void drawSabotage();
    void drawDebreif();
    void drawWaiting();
    void drawWin();
    void drawLose();

    SDL_Rect rectForPosition(char c);
    SDL_Rect rectForCardinal(char c);
    SDL_Rect rectForDay(char d);
    SDL_Rect rectForSun(char d);
    SDL_Rect rectForTraversal(char fcard, char tcard, float t);
    SDL_Rect rectForExpansion(char card, float t);
    SDL_Rect rectForTransition(char fd, char td, float t);
    SDL_Rect rectForPositionHealth(char c);
    SDL_Rect rectForCardinalHealth(char c);
    SDL_Rect rectForPositionStatus(char c);
    SDL_Rect rectForCardinalStatus(char c);

    SDL_Rect spriteForAction(char a);

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

