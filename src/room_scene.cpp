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

  backButton = UI::Button(10,10,20,20);

  inRoomLabel = UI::Label(ww/2-100,wh/2-100,20,"In Room");

  ipLabel = UI::Label(ww/2-100,wh/2-80,20,Network::getIP().ptr());

  portLabel = UI::Label(ww/2+30,wh/2-80,20,"8080");

  leaveSessLabel  = UI::Label( ww/2-100, wh/2-50, 20, "Leave Session");
  leaveSessButton = UI::Button(ww/2-100, wh/2-50, 200, 20);

  beginGameLabel  = UI::Label( ww/2-100, wh/2+50, 20, "Begin Game!");
  beginGameButton = UI::Button(ww/2-100, wh/2+50, 200, 20);

  cardLbls[Compass::icardinal('n')] = UI::Label(ww/2-10,      10, 20, "N");
  cardLbls[Compass::icardinal('e')] = UI::Label(ww  -30, wh/2-10, 20, "E");
  cardLbls[Compass::icardinal('s')] = UI::Label(ww/2-10, wh  -30, 20, "S");
  cardLbls[Compass::icardinal('w')] = UI::Label(     10, wh/2-10, 20, "W");

  cardBtns[Compass::icardinal('n')] = UI::Button(ww/2-10,      10, 20, 20);
  cardBtns[Compass::icardinal('e')] = UI::Button(ww  -30, wh/2-10, 20, 20);
  cardBtns[Compass::icardinal('s')] = UI::Button(ww/2-10, wh  -30, 20, 20);
  cardBtns[Compass::icardinal('w')] = UI::Button(     10, wh/2-10, 20, 20);

  cardBoxs[Compass::icardinal('n')] = UI::Box(ww/2-10-5,      10-5, 30, 30);
  cardBoxs[Compass::icardinal('e')] = UI::Box(ww  -30-5, wh/2-10-5, 30, 30);
  cardBoxs[Compass::icardinal('s')] = UI::Box(ww/2-10-5, wh  -30-5, 30, 30);
  cardBoxs[Compass::icardinal('w')] = UI::Box(     10-5, wh/2-10-5, 30, 30);

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
      if(c->imCardinal(card)) client->broadcast(card, e_type_revoke_card);
      else                    client->broadcast(card, e_type_assign_card);
    }
  }

  if(leaveSessButton.query(in)) { }
  if(s && c->model.rolesAssigned() && beginGameButton.query(in))
    client->broadcast(c->myCardinal(), e_type_begin_play);
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

  leaveSessLabel.draw(graphics);
  leaveSessButton.draw(graphics);

  if(s && c->model.rolesAssigned())
  {
    beginGameLabel.draw(graphics);
    beginGameButton.draw(graphics);
  }
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


