#include "room_scene.h"
#include "graphics.h"
#include "sprite.h"
#include "network.h"
#include "input.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "conids.h"

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

  youAreLabel = UI::Label("You Are ", ww-200, 20, 30);
  youPTag = UI::Image(Sprite::p1(), ww-70, 0, 60, 60);
  showIpButton = UI::ImageButton(Sprite::question_bubble(), ww-200, 60, 30, 30);
  netroleLabel = UI::Label("host", ww-160, 60, 30);

  ipLabel     = UI::Label(Network::getIP().ptr(), ww/2-250, wh/2-120,40);
  portLabel   = UI::Label("4040", ww/2+150, wh/2-120,40);

  pTags[0] = Sprite::p1();
  pTags[1] = Sprite::p2();
  pTags[2] = Sprite::p3();
  pTags[3] = Sprite::p4();
  pTagsW[0] = Sprite::p1_w();
  pTagsW[1] = Sprite::p2_w();
  pTagsW[2] = Sprite::p3_w();
  pTagsW[3] = Sprite::p4_w();
  pTagsB[0] = Sprite::p1_b();
  pTagsB[1] = Sprite::p2_b();
  pTagsB[2] = Sprite::p3_b();
  pTagsB[3] = Sprite::p4_b();
  pTagsR[0] = Sprite::p1_r();
  pTagsR[1] = Sprite::p2_r();
  pTagsR[2] = Sprite::p3_r();
  pTagsR[3] = Sprite::p4_r();

  pScoreLabels[0] = UI::Image(pTags[0], space(ww, 200, 60, 4, 0), 140, 60, 60);
  pScoreLabels[1] = UI::Image(pTags[1], space(ww, 200, 60, 4, 1), 140, 60, 60);
  pScoreLabels[2] = UI::Image(pTags[2], space(ww, 200, 60, 4, 2), 140, 60, 60);
  pScoreLabels[3] = UI::Image(pTags[3], space(ww, 200, 60, 4, 3), 140, 60, 60);

  beginGameButton = UI::TextButton("Begin Game!", ww/2-250, wh/2-20, 500, 40);

  SDL_Rect r = whoBoxForPosition('s');
  chooseBtn = UI::Button(r);
  r.x -= 2*r.w;
  cwBtn  = UI::ImageButton(Sprite::arrow_left_up(), r);
  r.x += 2*r.w;
  r.x += 2*r.w;
  ccwBtn = UI::ImageButton(Sprite::arrow_right_up(), r);

  cardCWOffset = 0;

  cardRects[Compass::icardinal('n')].set(whoBoxForPosition('n'),whoBoxForPosition('n'));
  cardRects[Compass::icardinal('e')].set(whoBoxForPosition('e'),whoBoxForPosition('e'));
  cardRects[Compass::icardinal('s')].set(whoBoxForPosition('s'),whoBoxForPosition('s'));
  cardRects[Compass::icardinal('w')].set(whoBoxForPosition('w'),whoBoxForPosition('w'));

  UI::AnimSprites s;
  s = UI::AnimSprites(Sprite::gen_n_0(), Sprite::gen_n_1(), Sprite::gen_n_2(), Sprite::gen_n_3());
  cardImgs[Compass::icardinal('n')] = UI::Anim(s, 4, 1.f, cardRects[Compass::icardinal('n')].v());
  s = UI::AnimSprites(Sprite::gen_e_0(), Sprite::gen_e_1(), Sprite::gen_e_2(), Sprite::gen_e_3());
  cardImgs[Compass::icardinal('e')] = UI::Anim(s, 4, 1.f, cardRects[Compass::icardinal('e')].v());
  s = UI::AnimSprites(Sprite::gen_s_0(), Sprite::gen_s_1(), Sprite::gen_s_2(), Sprite::gen_s_3());
  cardImgs[Compass::icardinal('s')] = UI::Anim(s, 4, 1.f, cardRects[Compass::icardinal('s')].v());
  s = UI::AnimSprites(Sprite::gen_w_0(), Sprite::gen_w_1(), Sprite::gen_w_2(), Sprite::gen_w_3());
  cardImgs[Compass::icardinal('w')] = UI::Anim(s, 4, 1.f, cardRects[Compass::icardinal('w')].v());

  cardLbls[Compass::icardinal('n')] = UI::Label("north", cardRects[Compass::icardinal('n')].v()); cardLbls[0].rect = cardRects[0].v(); cardLbls[0].rect.y += cardLbls[0].rect.h; cardLbls[0].rect.h /= 3;
  cardLbls[Compass::icardinal('e')] = UI::Label("east",  cardRects[Compass::icardinal('e')].v()); cardLbls[1].rect = cardRects[1].v(); cardLbls[1].rect.y += cardLbls[1].rect.h; cardLbls[1].rect.h /= 3;
  cardLbls[Compass::icardinal('s')] = UI::Label("south", cardRects[Compass::icardinal('s')].v()); cardLbls[2].rect = cardRects[2].v(); cardLbls[2].rect.y += cardLbls[2].rect.h; cardLbls[2].rect.h /= 3;
  cardLbls[Compass::icardinal('w')] = UI::Label("west",  cardRects[Compass::icardinal('w')].v()); cardLbls[3].rect = cardRects[3].v(); cardLbls[3].rect.y += cardLbls[3].rect.h; cardLbls[3].rect.h /= 3;

  SCENE_CHANGE_HACK = 0;
}

SDL_Rect RoomScene::whoBoxForPosition(char c)
{
  int i = Compass::icardinal(c);

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  //(clockwise)    top          right      bottom(you)     left
  int ws[] = {          60,              80,             160,              80};
  int hs[] = {          60,              80,             160,              80};
  int xs[] = {ww/2-ws[0]/2,     ww-ws[1]-20,    ww/2-ws[2]/2,              20};
  int ys[] = {          20, wh/2-hs[1]/2-40,  wh-hs[2]-20-40, wh/2-hs[3]/2-40};

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

  if(s) netroleLabel.text = String("host");
  else  netroleLabel.text = String("client");
       if(c->myConnection() == '1') youPTag.sprite = Sprite::p1();
  else if(c->myConnection() == '2') youPTag.sprite = Sprite::p2();
  else if(c->myConnection() == '3') youPTag.sprite = Sprite::p3();
  else if(c->myConnection() == '4') youPTag.sprite = Sprite::p4();
}

void RoomScene::touch(In &in)
{
  if(in.type != In::DOWN) return;
  if(backButton.query(in)) { }

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
    cardRects[i].set(cardRects[i].v(),whoBoxForPosition(Compass::cardinal((i+cardCWOffset)%4)),0.0f);
    cardRects[i].tick(0.2); //bs delta
    cardImgs[i].rect = cardRects[i].v();
    cardLbls[i].rect = cardRects[i].v();
    cardLbls[i].rect.y += cardLbls[i].rect.h;
    cardLbls[i].rect.h /= 3;
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
           if(card == 'n' || card == 's') graphics->draw(pTagsB[ConIds::iconid(con)],tmp);
      else if(card == 'e' || card == 'w') graphics->draw(pTagsR[ConIds::iconid(con)],tmp);
    }
  }

  backButton.draw(graphics);
  youAreLabel.draw(graphics);
  youPTag.draw(graphics);
  showIpButton.draw(graphics);
  netroleLabel.draw(graphics);
  //ipLabel.draw(graphics);
  //portLabel.draw(graphics);

  //draw score labels
  for(int i = 0; i < 4; i++)
  {
    if(c->model.connectionConnection(ConIds::conid(i)) == '0')
      pScoreLabels[i].sprite = pTags[i];
    else
    {
      char gen = c->model.connectionGeneral(ConIds::conid(i)).cardinal;
      if(gen == 'n' || gen == 's')
        pScoreLabels[i].sprite = pTagsB[i];
      else if(gen == 'e' || gen == 'w')
        pScoreLabels[i].sprite = pTagsR[i];
      else
        pScoreLabels[i].sprite = pTagsW[i];
    }
    pScoreLabels[i].draw(graphics);
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
}

RoomScene::~RoomScene()
{

}

