#include "room_scene.h"
#include "graphics.h"
#include "sprite.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"

#include "SDL.h"

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

  SDL_Rect r = whoBoxForPosition('s');
  chooseBtn = UI::Button(r);
  r.x -= 2*r.w;
  cwBtn  = UI::Button(r);
  r.x += 2*r.w;
  r.x += 2*r.w;
  ccwBtn = UI::Button(r);

  cardCWOffset = 0;

  cardRects[Compass::icardinal('n')].set(whoBoxForPosition('n'),whoBoxForPosition('n'));
  cardRects[Compass::icardinal('e')].set(whoBoxForPosition('e'),whoBoxForPosition('e'));
  cardRects[Compass::icardinal('s')].set(whoBoxForPosition('s'),whoBoxForPosition('s'));
  cardRects[Compass::icardinal('w')].set(whoBoxForPosition('w'),whoBoxForPosition('w'));

  cardImgs[Compass::icardinal('n')] = UI::Image(Sprite::n_general(), cardRects[Compass::icardinal('n')].v());
  cardImgs[Compass::icardinal('e')] = UI::Image(Sprite::e_general(), cardRects[Compass::icardinal('e')].v());
  cardImgs[Compass::icardinal('s')] = UI::Image(Sprite::s_general(), cardRects[Compass::icardinal('s')].v());
  cardImgs[Compass::icardinal('w')] = UI::Image(Sprite::w_general(), cardRects[Compass::icardinal('w')].v());

  cardLbls[Compass::icardinal('n')] = UI::Label(cardRects[Compass::icardinal('n')].v(), "N");
  cardLbls[Compass::icardinal('e')] = UI::Label(cardRects[Compass::icardinal('e')].v(), "E");
  cardLbls[Compass::icardinal('s')] = UI::Label(cardRects[Compass::icardinal('s')].v(), "S");
  cardLbls[Compass::icardinal('w')] = UI::Label(cardRects[Compass::icardinal('w')].v(), "W");

  SCENE_CHANGE_HACK = 0;
}

SDL_Rect RoomScene::whoBoxForPosition(char c)
{
  int i = Compass::icardinal(c);

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //(clockwise)    top          right      bottom(you)     left
  int ws[] = {          60,           80,          160,           80};
  int hs[] = {          60,           80,          160,           80};
  int xs[] = {ww/2-ws[0]/2,  ww-ws[1]-20, ww/2-ws[2]/2,           20};
  int ys[] = {          20, wh/2-hs[1]/2,  wh-hs[2]-20, wh/2-hs[3]/2};

  SDL_Rect tmp;
  tmp.x = xs[i];
  tmp.y = ys[i];
  tmp.w = ws[i];
  tmp.h = hs[i];

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

  if( cwBtn.query(in)) cardCWOffset = (cardCWOffset+1)%4;
  if(ccwBtn.query(in)) cardCWOffset = (cardCWOffset+3)%4;

  if(chooseBtn.query(in))
  {
    char card = Compass::cardinal((2-cardCWOffset+4)%4); //'s'+offset
    if(c->imCardinal(card)) c->requestRevokeCardinal();
    else                    c->requestCardinal(card);
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

  for(int i = 0; i < 4; i++)
  {
    cardRects[i].set(cardRects[i].v(),whoBoxForPosition(Compass::cardinal((i+cardCWOffset)%4)),0.0f);
    cardRects[i].tick(0.2); //bs delta
    cardImgs[i].rect = cardRects[i].v();
    cardLbls[i].rect = cardRects[i].v();
  }

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
    cardImgs[i].draw(graphics);
    if(c->model.cardinalConnected(card))
      cardLbls[i].draw(graphics);
  }

  cwBtn.draw(graphics);
  ccwBtn.draw(graphics);

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

