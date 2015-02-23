#include "room_scene.h"
#include "graphics.h"
#include "sprite.h"
#include "network.h"
#include "input.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "conids.h"

#include "SDL.h"

RoomScene::RoomScene(Graphics *g, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  s_ptr = &sm;
  c_ptr = &cm;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //(clockwise)    top          right      bottom(you)     left
  int ws[] = {          60,              80,             160,              80};
  int hs[] = {          60,              80,             160,              80};
  int xs[] = {ww/2-ws[0]/2,     ww-ws[1]-20,    ww/2-ws[2]/2,              20};
  int ys[] = {          20, wh/2-hs[1]/2-40,  wh-hs[2]-20-40, wh/2-hs[3]/2-40};

  for(int i = 0; i < 4; i++)
  {
    posRects[i]      = UI::Box(xs[i],       ys[i], ws[i],   hs[i]).rect;
    posLabelRects[i] = UI::Box(xs[i], ys[i]+hs[i], ws[i], hs[i]/3).rect;
  }

  for(int i = 0; i < 4; i++)
  {
    cardRects[i].set(posRects[i],posRects[i]);
    cardLabelRects[i].set(posLabelRects[i],posLabelRects[i]);
  }

  //Hacks
  char cardnamehacks[] = {'N','o','r','t','h','\0','E','a','s','t','\0','\0','S','o','u','t','h','\0','W','e','s','t','\0','\0'};

  //Views
  for(int i = 0; i < 4; i++)
  {
    cardImgs[i] = UI::Anim(Sprite::general_anims[i], 4, 1.f, posRects[i]);
    cardLbls[i] = UI::Label(&cardnamehacks[i*6], posLabelRects[i]);
  }

  backButton = UI::Button(20,20,40,40);

  youAreLabel = UI::Label("You Are ", ww-200, 20, 30);
  youPTag = UI::Image(Sprite::p1, ww-70, 0, 60, 60);
  showIpQ = UI::Image(Sprite::question_bubble, ww-200, 60, 30, 30);
  netroleLabel = UI::Label("host", ww-160, 60, 30);

  showIpButton = UI::Button(ww-200, 0, 200, 90);
  ipLabel     = UI::Label(Network::getIP().ptr(), ww/2-250, wh/2-170,40);
  portLabel   = UI::Label("4040", ww/2+150, wh/2-170,40);

  for(int i = 0; i < 4; i++)
  {
    pScoreLabels[i] = UI::Image(Sprite::pTags[i], space(ww, 200, 60, 4, i), 140, 60, 60);
    pWinLabels[i]   = UI::Label("0",      space(ww, 200, 30, 4, i), 190, 30);
    pLossLabels[i]  = UI::Label("0",      space(ww, 200, 30, 4, i), 220, 30);
    pTieLabels[i]   = UI::Label("0",      space(ww, 200, 30, 4, i), 250, 30);
  }

  beginGameButton = UI::TextButton("Begin Game!", ww/2-250, wh/2-20, 500, 40);

  SDL_Rect r = posRects[2]; //rect = 's' pos
  chooseBtn = UI::Button(r);
  r.x -= 2*r.w;
  cwBtn  = UI::ImageButton(Sprite::arrow_left_up, r);
  r.x += 4*r.w;
  ccwBtn = UI::ImageButton(Sprite::arrow_right_up, r);

  reset();
}

void RoomScene::reset()
{
  s = 0;
  c = 0;

  cardCWOffset = 0;
  showingIp = false;

  SCENE_CHANGE_HACK = 0;
}

void RoomScene::enter()
{
  s = *s_ptr;
  c = *c_ptr;

  if(s) netroleLabel.text = String("host");
  else  netroleLabel.text = String("client");
       if(c->myConnection() == '1') youPTag.sprite = Sprite::p1;
  else if(c->myConnection() == '2') youPTag.sprite = Sprite::p2;
  else if(c->myConnection() == '3') youPTag.sprite = Sprite::p3;
  else if(c->myConnection() == '4') youPTag.sprite = Sprite::p4;

  //should never change while in this room... I don't think
  for(int i = 0; i < 4; i++)
  {
    char con = ConIds::conid(i);
    VictoryRecord r = c->model.connectionVictoryRecord(con);
    pWinLabels[i].text = String::decimalRep(r.win);
    pLossLabels[i].text = String::decimalRep(r.loss);
    pTieLabels[i].text = String::decimalRep(r.tie);
  }

}

void RoomScene::touch(In &in)
{
  if(in.type == In::UP) showingIp = false;
  if(in.type != In::DOWN) return;
  if(backButton.query(in))
  {
    SCENE_CHANGE_HACK = -3;
  }

  if(showIpButton.query(in)) showingIp = true;

  if( cwBtn.query(in)) cardCWOffset = (cardCWOffset+1)%4;
  if(ccwBtn.query(in)) cardCWOffset = (cardCWOffset+3)%4;

  if(chooseBtn.query(in))
  {
    char card = Compass::cardinal((2-cardCWOffset+4)%4); //'s'+offset
    if(c->imCardinal(card)) c->requestRevokeCardinal();
    else                    c->requestCardinal(card);
  }

  if(s && c->model.rolesAssigned() && beginGameButton.query(in))
    c->requestBeginPlay();
}

int RoomScene::tick()
{
  if(s) s->tick();
  c->tick();

  for(int i = 0; i < 4; i++)
    cardImgs[i].tick(0.4f);

  if(c->model.days != -1) SCENE_CHANGE_HACK = 1;

  for(int i = 0; i < 4; i++)
  {
    cardRects[i].set(          cardRects[i].v(),     posRects[((i+cardCWOffset)%4)],0.0f);
    cardLabelRects[i].set(cardLabelRects[i].v(),posLabelRects[((i+cardCWOffset)%4)],0.0f);
    cardRects[i].tick(0.2); //bs delta
    cardLabelRects[i].tick(0.2); //bs delta
    cardImgs[i].rect = cardRects[i].v();
    cardLbls[i].rect = cardLabelRects[i].v();
  }

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void RoomScene::draw()
{
  //draw cardinals
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    char con = c->model.cardinalConnection(card);
    cardImgs[i].draw(graphics);
    cardLbls[i].draw(graphics);
    SDL_Rect tmp = cardImgs[i].rect.rect;
    tmp.y += tmp.h/2;
    tmp.x += tmp.w/2;
    tmp.w /= 2;
    tmp.h /= 2;
    if(con != '0')
    {
           if(card == 'n' || card == 's') graphics->draw(Sprite::pTagsB[ConIds::iconid(con)],tmp);
      else if(card == 'e' || card == 'w') graphics->draw(Sprite::pTagsR[ConIds::iconid(con)],tmp);
    }
  }

  backButton.draw(graphics);
  youAreLabel.draw(graphics);
  youPTag.draw(graphics);
  showIpQ.draw(graphics);
  netroleLabel.draw(graphics);

  if(showingIp)
  {
    ipLabel.draw(graphics);
    portLabel.draw(graphics);
  }

  //draw score labels
  for(int i = 0; i < 4; i++)
  {
    if(c->model.connectionConnection(ConIds::conid(i)) == '0')
      pScoreLabels[i].sprite = Sprite::pTags[i];
    else
    {
      char gen = c->model.connectionCardinal(ConIds::conid(i));
      if(gen == 'n' || gen == 's')
        pScoreLabels[i].sprite = Sprite::pTagsB[i];
      else if(gen == 'e' || gen == 'w')
        pScoreLabels[i].sprite = Sprite::pTagsR[i];
      else
        pScoreLabels[i].sprite = Sprite::pTagsW[i];
    }
    pScoreLabels[i].draw(graphics);
  }

  //draw scores
  for(int i = 0; i < 4; i++)
  {
    pWinLabels[i].draw(graphics);
    pLossLabels[i].draw(graphics);
    pTieLabels[i].draw(graphics);
  }

  cwBtn.draw(graphics);
  ccwBtn.draw(graphics);

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
  reset();
}

RoomScene::~RoomScene()
{

}

