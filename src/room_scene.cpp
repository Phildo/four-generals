#include "room_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

#include "compass.h"

RoomScene::RoomScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  client_ptr = &c;
  s_ptr = &sm;
  c_ptr = &cm;
  client = 0;
  s = 0;
  c = 0;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(20,20,40,40);

  inRoomLabel = UI::Label(ww/2-200,wh/2-200,40,"In Room");

  ipLabel = UI::Label(ww/2-200,wh/2-160,40,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+60,wh/2-160,40,"8080");

  leaveSessButton = UI::TextButton(ww/2-200, wh/2-100, 400, 40, "Leave Session");
  beginGameButton = UI::TextButton(ww/2-200, wh/2+100, 400, 40, "Begin Game!");

  cardLbls[Compass::icardinal('n')] = UI::Label(ww/2-20,      20, 40, "N");
  cardLbls[Compass::icardinal('e')] = UI::Label(ww  -60, wh/2-20, 40, "E");
  cardLbls[Compass::icardinal('s')] = UI::Label(ww/2-20, wh  -60, 40, "S");
  cardLbls[Compass::icardinal('w')] = UI::Label(     20, wh/2-20, 40, "W");

  cardBtns[Compass::icardinal('n')] = UI::Button(ww/2-20,      20, 40, 40);
  cardBtns[Compass::icardinal('e')] = UI::Button(ww  -60, wh/2-20, 40, 40);
  cardBtns[Compass::icardinal('s')] = UI::Button(ww/2-20, wh  -60, 40, 40);
  cardBtns[Compass::icardinal('w')] = UI::Button(     20, wh/2-20, 40, 40);

  cardBoxs[Compass::icardinal('n')] = UI::Box(ww/2-20-10,      20-10, 60, 60);
  cardBoxs[Compass::icardinal('e')] = UI::Box(ww  -60-10, wh/2-20-10, 60, 60);
  cardBoxs[Compass::icardinal('s')] = UI::Box(ww/2-20-10, wh  -60-10, 60, 60);
  cardBoxs[Compass::icardinal('w')] = UI::Box(     20-10, wh/2-20-10, 60, 60);

  SCENE_CHANGE_HACK = 0;
}

void RoomScene::enter()
{
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;
}

void RoomScene::touch(In &in)
{
  if(backButton.query(in)) { }

  //check cardinal buttons
  for(int i = 0; i < 4; i++)
  {
    if(cardBtns[i].query(in))
    {
      char card = Compass::cardinal(i);
      if(c->imCardinal(card)) c->requestRevokeCardinal();
      else                    c->requestCardinal(card);
    }
  }

  if(leaveSessButton.query(in)) { }
  if(s && c->model.rolesAssigned() && beginGameButton.query(in))
    c->requestBeginPlay();
}

int RoomScene::tick()
{
  if(s) s->tick();
  c->tick();

  if(c->model.days != -1) SCENE_CHANGE_HACK = 1;

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void RoomScene::draw()
{
  backButton.draw(graphics);
  inRoomLabel.draw(graphics);
  ipLabel.draw(graphics);
  portLabel.draw(graphics);

  //draw cardinals
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    if(c->model.cardinalConnected(card))
    {
      cardLbls[i].draw(graphics);
      if(c->imCardinal(card))
        cardBoxs[i].draw(graphics);
    }

    cardBtns[i].draw(graphics);
  }

  leaveSessButton.draw(graphics);

  if(s && c->model.rolesAssigned())
    beginGameButton.draw(graphics);
}

void RoomScene::leave()
{
}
void RoomScene::pass()
{

}
void RoomScene::pop()
{
}

RoomScene::~RoomScene()
{

}

