#include "room_scene.h"
#include "graphics.h"
#include "sprite.h"
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

  inRoomLabel = UI::Label(ww/2-250, wh/2-80,40,"In Room");
  ipLabel     = UI::Label(ww/2-250, wh/2-30,40,Network::getIP().ptr());
  portLabel   = UI::Label(ww/2+100, wh/2-30,40,"8080");

  leaveSessButton = UI::TextButton(ww/2-250, wh/2+20, 500, 40, "Leave Session");
  beginGameButton = UI::TextButton(ww/2-250, wh/2+70, 500, 40, "Begin Game!");

  cardBtns[Compass::icardinal('n')] = UI::Button(whoBoxForPosition('n'));
  cardBtns[Compass::icardinal('e')] = UI::Button(whoBoxForPosition('e'));
  cardBtns[Compass::icardinal('s')] = UI::Button(whoBoxForPosition('s'));
  cardBtns[Compass::icardinal('w')] = UI::Button(whoBoxForPosition('w'));

  cardImgs[Compass::icardinal('n')] = UI::Image(Sprite::n_general(), whoBoxForPosition('n'));
  cardImgs[Compass::icardinal('e')] = UI::Image(Sprite::e_general(), whoBoxForPosition('e'));
  cardImgs[Compass::icardinal('s')] = UI::Image(Sprite::s_general(), whoBoxForPosition('s'));
  cardImgs[Compass::icardinal('w')] = UI::Image(Sprite::w_general(), whoBoxForPosition('w'));

  cardLbls[Compass::icardinal('n')] = UI::Label(whoBoxForPosition('n'), "N");
  cardLbls[Compass::icardinal('e')] = UI::Label(whoBoxForPosition('e'), "E");
  cardLbls[Compass::icardinal('s')] = UI::Label(whoBoxForPosition('s'), "S");
  cardLbls[Compass::icardinal('w')] = UI::Label(whoBoxForPosition('w'), "W");

  SCENE_CHANGE_HACK = 0;
}

SDL_Rect RoomScene::whoBoxForPosition(char c)
{
  int i = Compass::icardinal(c);

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  int bw = 120;
  int bh = 120;

  //(clockwise) top     right  bottom(you) left
  int xs[] = {ww/2-bw/2,  ww-bw-20, ww/2-bw/2,        20};
  int ys[] = {       20, wh/2-bh/2,  wh-bh-20, wh/2-bh/2};

  SDL_Rect tmp;
  tmp.x = xs[i];
  tmp.y = ys[i];
  tmp.w = bw;
  tmp.h = bh;

  return tmp;
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
    cardBtns[i].draw(graphics);
    char card = Compass::cardinal(i);
    if(c->model.cardinalConnected(card))
    {
      if(c->imCardinal(card))
        cardImgs[i].draw(graphics);
      cardLbls[i].draw(graphics);
    }

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

