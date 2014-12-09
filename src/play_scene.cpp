#include "play_scene.h"
#include "graphics.h"
#include "sprite.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "week.h"

#include "SDL.h"

PlayScene::PlayScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
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

  //(clockwise)           top                right              bottom(you)          left
  int posRectW[] = {               120,                120,                120,                120};
  int posRectH[] = {               120,                120,                120,                120};
  int posRectX[] = {ww/2-posRectW[0]/2,  ww-posRectW[1]-20, ww/2-posRectW[2]/2,                 20};
  int posRectY[] = {               120, wh/2-posRectH[1]/2,  wh-posRectH[2]-80, wh/2-posRectH[3]/2};

  for(int i = 0; i < 4; i++)
  {
    positionRects[i].x = posRectX[i];
    positionRects[i].y = posRectY[i];
    positionRects[i].w = posRectW[i];
    positionRects[i].h = posRectH[i];
  }

  for(int i = 0; i < 7; i++)
  {
    dayRects[i].x = space(ww,60,40,7,i);
    dayRects[i].y = 20;
    dayRects[i].w = 40;
    dayRects[i].h = 40;

    sunRects[i].x = space(ww,50,60,7,i);
    sunRects[i].y = 10;
    sunRects[i].w = 60;
    sunRects[i].h = 60;
  }

  dayLbls[0] = UI::Label(dayRects[0], "Su");
  dayLbls[1] = UI::Label(dayRects[1], "Mo");
  dayLbls[2] = UI::Label(dayRects[2], "Tu");
  dayLbls[3] = UI::Label(dayRects[3], "We");
  dayLbls[4] = UI::Label(dayRects[4], "Th");
  dayLbls[5] = UI::Label(dayRects[5], "Fr");
  dayLbls[6] = UI::Label(dayRects[6], "Sa");
  sun = UI::Image(Sprite::sun(), sunRects[0]);

  for(int i = 0; i < 7; i++)
    whenBtns[i] = UI::Button(dayRects[i]);

  actionAttackButton   = UI::TextButton(space(ww,60,200,4,0), wh-60, 200, 40, "attack");
  actionDefendButton   = UI::TextButton(space(ww,60,200,4,1), wh-60, 200, 40, "defend");
  actionMessageButton  = UI::TextButton(space(ww,60,200,4,2), wh-60, 200, 40, "message");
  actionSabotageButton = UI::TextButton(space(ww,60,200,4,3), wh-60, 200, 40, "sabotage");

  howBlockButton    = UI::TextButton(space(ww,60,200,3,0), wh-60, 200, 40, "Block");
  howReadButton     = UI::TextButton(space(ww,60,200,3,1), wh-60, 200, 40, "Read");
  howSabotageButton = UI::TextButton(space(ww,60,200,3,2), wh-60, 200, 40, "Sabotage");

  whichWhatButton  = UI::TextButton(space(ww,60,200,4,0), wh-60, 200, 40, "What");
  whichWhoButton   = UI::TextButton(space(ww,60,200,4,1), wh-60, 200, 40, "Who");
  whichWhenButton  = UI::TextButton(space(ww,60,200,4,2), wh-60, 200, 40, "When");
  whichWhereButton = UI::TextButton(space(ww,60,200,4,3), wh-60, 200, 40, "Where");

  whatAttackButton = UI::TextButton(space(ww,60,200,2,0), wh-60, 200, 40, "attack");
  whatDefendButton = UI::TextButton(space(ww,60,200,2,1), wh-60, 200, 40, "defend");

  howLabel      = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "how");
  whichLabel    = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "which");
  whatLabel     = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "what");
  whoLabel      = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "who");
  whenLabel     = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "when");
  whereLabel    = UI::Label(space(ww,0,200,1,0), wh/2-60, 40, "where");
  debreifLabel  = UI::Label(space(ww,0,600,1,0), wh/2-20, 30, "debreif");
  waitingLabel  = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "waiting");
  messageLabel  = UI::Label(space(ww,0,600,1,0), wh/2+20, 30, "message");
  sabotageLabel = UI::Label(space(ww,0,600,1,0), wh/2+50, 30, "sabotage");
  winLabel      = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "WIN");
  loseLabel     = UI::Label(space(ww,0,200,1,0), wh/2-20, 40, "LOSE");

  confirmButton = UI::TextButton(space(ww,30,200,2,0), wh-60, 200, 40, "confirm");
  cancelButton  = UI::TextButton(space(ww,30,200,2,1), wh-60, 200, 40, "cancel");
  resetButton   = UI::TextButton(space(ww,30,200,1,0), wh-60, 200, 40, "reset game");

  messengerImg = UI::Image(Sprite::messenger(),0,0,0,0);
  swordImg = UI::Image(Sprite::sword(),0,0,0,0);

  known_day = '0';
  anim_day = 0.0f;
  shown_day = 0.0f;
}

void PlayScene::enter()
{
  //need to wait until c_model is obtained

  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;
  SDL_Rect rect;

  char scard[2]; scard[1] = '\0'; //holder to pass string to labels
  int icard;
  char card;

  //bottom (you)
  card = c->myCardinal();
  icard = Compass::icardinal(card);
  rect = rectForPosition('s');
  scard[0] = card;
  if(card == 'n') cardImgs[icard] = UI::Image(Sprite::n_general(), rect);
  if(card == 'e') cardImgs[icard] = UI::Image(Sprite::e_general(), rect);
  if(card == 's') cardImgs[icard] = UI::Image(Sprite::s_general(), rect);
  if(card == 'w') cardImgs[icard] = UI::Image(Sprite::w_general(), rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);
  rect.x = rect.x+rect.w/2+rect.w/4;
  rect.y = rect.y-rect.h/4;
  rect.w = rect.w/2;
  rect.h = rect.h/2;
  cardHealth[icard]   = UI::Image(Sprite::null(), rect);
  rect.y = rect.y+2*rect.h;
  cardActIcons[icard] = UI::Image(Sprite::null(), rect);

  //left (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('w');
  scard[0] = card;
  if(card == 'n') cardImgs[icard]  = UI::Image(Sprite::n_general(), rect);
  if(card == 'e') cardImgs[icard]  = UI::Image(Sprite::e_general(), rect);
  if(card == 's') cardImgs[icard]  = UI::Image(Sprite::s_general(), rect);
  if(card == 'w') cardImgs[icard]  = UI::Image(Sprite::w_general(), rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);
  rect.x = rect.x+rect.w/2+rect.w/4;
  rect.y = rect.y-rect.h/4;
  rect.w = rect.w/2;
  rect.h = rect.h/2;
  cardHealth[icard]   = UI::Image(Sprite::null(), rect);
  rect.y = rect.y+2*rect.h;
  cardActIcons[icard] = UI::Image(Sprite::null(), rect);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('n');
  scard[0] = card;
  if(card == 'n') cardImgs[icard]  = UI::Image(Sprite::n_general(), rect);
  if(card == 'e') cardImgs[icard]  = UI::Image(Sprite::e_general(), rect);
  if(card == 's') cardImgs[icard]  = UI::Image(Sprite::s_general(), rect);
  if(card == 'w') cardImgs[icard]  = UI::Image(Sprite::w_general(), rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);
  rect.x = rect.x+rect.w/2+rect.w/4;
  rect.y = rect.y-rect.h/4;
  rect.w = rect.w/2;
  rect.h = rect.h/2;
  cardHealth[icard]   = UI::Image(Sprite::null(), rect);
  rect.y = rect.y+2*rect.h;
  cardActIcons[icard] = UI::Image(Sprite::null(), rect);

  //top (clockwise)
  card = Compass::cwcardinal(card);
  icard = Compass::icardinal(card);
  rect = rectForPosition('e');
  scard[0] = card;
  if(card == 'n') cardImgs[icard]  = UI::Image(Sprite::n_general(), rect);
  if(card == 'e') cardImgs[icard]  = UI::Image(Sprite::e_general(), rect);
  if(card == 's') cardImgs[icard]  = UI::Image(Sprite::s_general(), rect);
  if(card == 'w') cardImgs[icard]  = UI::Image(Sprite::w_general(), rect);
  whoBtns[icard]   = UI::Button(rect);
  whereBtns[icard] = UI::Button(rect);
  rect.x = rect.x+rect.w/2+rect.w/4;
  rect.y = rect.y-rect.h/4;
  rect.w = rect.w/2;
  rect.h = rect.h/2;
  cardHealth[icard]   = UI::Image(Sprite::null(), rect);
  rect.y = rect.y+2*rect.h;
  cardActIcons[icard] = UI::Image(Sprite::null(), rect);

  zeroE();
}

void PlayScene::zeroE()
{
  e.zero();
  e.connection = c->myConnection();
  e.cardinal = c->myCardinal();
  e.type = e_type_commit_action;
}

void PlayScene::chooseAction(In &in)
{
  if(actionAttackButton.query(in))    e.action = 'a';
  if(actionDefendButton.query(in))    e.action = 'd';
  if(actionMessageButton.query(in)) { e.action = 'm'; e.to = Compass::opcardinal(e.cardinal); } //auto assign 'to'
  if(actionSabotageButton.query(in))  e.action = 's';
}

void PlayScene::chooseHow(In &in)
{
  if(howBlockButton.query(in))    e.how = 'b';
  if(howReadButton.query(in))     e.how = 'r';
  if(howSabotageButton.query(in)) e.how = 's';
}

void PlayScene::chooseWhich(In &in)
{
  if(whichWhatButton.query(in))  e.which = 'a';
  if(whichWhoButton.query(in))   e.which = 'o';
  if(whichWhenButton.query(in))  e.which = 'e';
  if(whichWhereButton.query(in)) e.which = 'r';
}

void PlayScene::chooseWhat(In &in)
{
  if(whatAttackButton.query(in)) e.what = 'a';
  if(whatDefendButton.query(in)) e.what = 'd';
}

void PlayScene::chooseWho(In &in)
{
  for(int i = 0; i < 4; i++)
    if(whoBtns[i].query(in)) e.who = Compass::cardinal(i);
}

void PlayScene::chooseWhen(In &in)
{
  for(int i = 0; i < 7; i++)
    if(whenBtns[i].query(in)) e.when = Week::day(i);
}

void PlayScene::chooseWhere(In &in)
{
  for(int i = 0; i < 4; i++)
  {
    if(whereBtns[i].query(in))
    {
      if(Compass::cardinal(i) == Compass::cwcardinal( c->myCardinal()) ||
         Compass::cardinal(i) == Compass::ccwcardinal(c->myCardinal()))
        e.where = Compass::cardinal(i);
    }
  }
}

void PlayScene::seekConfirmation(In &in)
{
  if(confirmButton.query(in)) c->commitAction(e);
  if(cancelButton.query(in)) zeroE();
}

void PlayScene::drawAction()
{
  actionAttackButton.draw(graphics);
  actionDefendButton.draw(graphics);
  actionMessageButton.draw(graphics);
  actionSabotageButton.draw(graphics);
}

void PlayScene::drawHow()
{
  howLabel.draw(graphics);
  howBlockButton.draw(graphics);
  howReadButton.draw(graphics);
  howSabotageButton.draw(graphics);
}

void PlayScene::drawWhich()
{
  whichLabel.draw(graphics);
  whichWhatButton.draw(graphics);
  whichWhoButton.draw(graphics);
  whichWhenButton.draw(graphics);
  whichWhereButton.draw(graphics);
}

void PlayScene::drawWhat()
{
  whatLabel.draw(graphics);
  whatAttackButton.draw(graphics);
  whatDefendButton.draw(graphics);
}

void PlayScene::drawWho()
{
  whoLabel.draw(graphics);
}

void PlayScene::drawWhen()
{
  whenLabel.draw(graphics);
}

void PlayScene::drawWhere()
{
  whereLabel.draw(graphics);
}

void PlayScene::drawConfirmation()
{
  confirmButton.draw(graphics);
  cancelButton.draw(graphics);
}

void PlayScene::drawDebreif()
{
  debreifLabel.text = e.humanAction();
  debreifLabel.draw(graphics);
}

void PlayScene::drawWaiting()
{
  waitingLabel.draw(graphics);
}

void PlayScene::drawMessage()
{
  messageLabel.text = c->myMessage().message();
  messageLabel.draw(graphics);
}

void PlayScene::drawSabotage()
{
  sabotageLabel.text = c->mySabotage().message();
  sabotageLabel.draw(graphics);
}

void PlayScene::drawWin()
{
  winLabel.draw(graphics);
  if(s) resetButton.draw(graphics);
}

void PlayScene::drawLose()
{
  loseLabel.draw(graphics);
  if(s) resetButton.draw(graphics);
}

void PlayScene::touch(In &in)
{
  //oh god terrible tree traversal touch propagation

  if(!c->iWin() && !c->iLose())
  {
    if(!c->iHaveAction())
    {
      if(e.action == '0') chooseAction(in);
      else if(e.action == 'a') //attack
      {
        if(e.who == '0') chooseWho(in);
        else seekConfirmation(in);
      }
      else if(e.action == 'm') //message
      {
        if(e.what == '0') chooseWhat(in);
        else if(e.what == 'a') //message attack
        {
          if(e.who == '0') chooseWho(in);
          else if(e.when == '0') chooseWhen(in);
          else if(e.where == '0') chooseWhere(in);
          else seekConfirmation(in);
        }
        else if(e.what == 'd') //message defend
        {
          if(e.when == '0') chooseWhen(in);
          else if(e.where == '0') chooseWhere(in);
          else seekConfirmation(in);
        }
      }
      else if(e.action == 'd') //defend
        seekConfirmation(in);
      else if(e.action == 's') //sabotage
      {
        if(e.how == '0') chooseHow(in);
        else if(e.how == 'b') seekConfirmation(in);
        else if(e.how == 'r') seekConfirmation(in);
        else if(e.how == 's')
        {
          if(e.which == '0') chooseWhich(in);
          else if(e.which == 'a')
          {
            if(e.what == '0') chooseWhat(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'o')
          {
            if(e.who == '0') chooseWho(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'e')
          {
            if(e.when == '0') chooseWhen(in);
            else seekConfirmation(in);
          }
          else if(e.which == 'r')
          {
            if(e.where == '0') chooseWhere(in);
            else seekConfirmation(in);
          }
        }
      }
    }
  }
  else if(s && resetButton.query(in))
  {
    c->requestReset();
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  psys.tick(0.01f);
  if(anim_day < c->model.days) anim_day += 0.01f;
  if(anim_day > c->model.days) anim_day = (float)c->model.days;
  shown_day = anim_day;
  if(known_day != c->model.currentDay())
  {
    if(c->model.days == -1)
      return -1; //game was reset- go back to room

/*
    for(int i = 0; i < c->model.messengers.len(); i++)
    {
      Messenger m = c->model.messengers[i];
      SDL_Rect spos;
      SDL_Rect epos;

      Particle p;
      p.type = P_TYPE_MESSENGER;
      p.mess.w = 40;
      p.mess.h = 40;
      p.mess.begin_card = m.was;
      p.mess.end_card = m.at;

      spos = rectForCardinal(m.was);
      epos = rectForCardinal(m.at);
      p.mess.x.set(spos.x,epos.x,0.f);
      p.mess.y.set(spos.y,epos.y,0.f);

      psys.registerP(p);
    }
    for(int i = 0; i < 4; i++)
    {
      char card = Compass::cardinal(i);
      if(c->model.cardinalPrevAction(card).action == 'd')
      {
        SDL_Rect pos;

        Particle p;
        p.type = P_TYPE_DEFEND;
        pos = rectForCardinal(card);
        p.defend.y = pos.y+pos.h/2; //center
        p.defend.x = pos.x+pos.w/2; //center
        p.defend.w.set(pos.w,pos.w*2,0.f);
        p.defend.h.set(pos.h,pos.h*2,0.f);

        psys.registerP(p);
      }
      if(c->model.cardinalPrevAction(card).action == 'a')
      {
        Messenger m = c->model.messengers[i];
        SDL_Rect spos;
        SDL_Rect epos;

        Particle p;
        p.type = P_TYPE_ATTACK;
        p.attack.w = 40;
        p.attack.h = 40;
        p.attack.begin_card = card;
        p.attack.end_card = c->model.cardinalPrevAction(card).who;

        spos = rectForCardinal(p.attack.begin_card);
        epos = rectForCardinal(p.attack.end_card);
        p.attack.x.set(spos.x,epos.x,0.f);
        p.attack.y.set(spos.y,epos.y,0.f);
        p.attack.anim.set(0,1);

        psys.registerP(p);
      }
    }
    */


    for(int i = 0; i < 4; i++)
    {
      char card = Compass::cardinal(i);
      SDL_Rect pos = rectForCardinal(card);
      pos.x = pos.x+(pos.w/2)+(pos.w/4);
      pos.y = pos.y-(pos.h/4);
      pos.w = pos.w/2;
      pos.h = pos.h/2;
      if(c->model.cardinalPrevAction(card).action == 'd')
        cardActIcons[i].sprite = Sprite::force_field();
      if(c->model.cardinalPrevAction(card).action == 'a')
        cardActIcons[i].sprite = Sprite::sword();
      if(c->model.cardinalPrevAction(card).action == 'm')
        cardActIcons[i].sprite = Sprite::envelope();
      if(c->model.cardinalPrevAction(card).action == 's')
        cardActIcons[i].sprite = Sprite::red_x();
      cardHealth[i].sprite = Sprite::shield_full();
    }

    zeroE();
    known_day = c->model.currentDay();
  }
  return 0;
}

void PlayScene::draw()
{
  psys.draw(graphics);

  float t = 1.f-((float)c->model.days-shown_day);
  if(t < 0.9)
  {
    for(int i = 0; i < c->model.messengers.len(); i++)
    {
      Messenger m = c->model.messengers[i];
      messengerImg.rect = rectForTraversal(m.was, m.at,t);
      messengerImg.draw(graphics);
    }

    for(int i = 0; i < 4; i++)
    {
      char card = Compass::cardinal(i);
      if(c->model.cardinalPrevAction(card).action == 'a')
      {
        Messenger m = c->model.messengers[i];
        swordImg.rect = rectForTraversal(card,c->model.cardinalPrevAction(card).who,t);
        swordImg.draw(graphics);
      }

      /*
      if(c->model.cardinalPrevAction(card).action == 'd')
      {
        SDL_Rect pos;

        Particle p;
        p.type = P_TYPE_DEFEND;
        pos = rectForCardinal(card);
        p.defend.y = pos.y+pos.h/2; //center
        p.defend.x = pos.x+pos.w/2; //center
        p.defend.w.set(pos.w,pos.w*2,0.f);
        p.defend.h.set(pos.h,pos.h*2,0.f);

        psys.registerP(p);
      }
      */
    }
  }
  sun.rect = rectForTransition(Week::day((c->model.days-1)%7), known_day, t);
  sun.draw(graphics);
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  if(c->iWin())
  {
    drawWin();
  }
  else if(c->iLose())
  {
    drawLose();
  }
  else
  {
    if(c->iHaveMessage())
      drawMessage();
    if(c->iHaveSabotage())
      drawSabotage();
    if(!c->iHaveAction())
    {
      drawDebreif();
      if(e.action == '0') drawAction();
      else if(e.action == 'a') //attack
      {
        if(e.who == '0') drawWho();
        else drawConfirmation();
      }
      else if(e.action == 'm') //message
      {
        if(e.what == '0') drawWhat();
        else if(e.what == 'a') //message attack
        {
          if(e.who == '0') drawWho();
          else if(e.when == '0') drawWhen();
          else if(e.where == '0') drawWhere();
          else drawConfirmation();
        }
        else if(e.what == 'd') //message defend
        {
          if(e.when == '0') drawWhen();
          else if(e.where == '0') drawWhere();
          else drawConfirmation();
        }
      }
      else if(e.action == 'd') //defend
        drawConfirmation();
      else if(e.action == 's') //sabotage
      {
        if(e.how == '0') drawHow();
        else if(e.how == 'b') drawConfirmation();
        else if(e.how == 'r') drawConfirmation();
        else if(e.how == 's')
        {
          if(e.which == '0') drawWhich();
          else if(e.which == 'a')
          {
            if(e.what == '0') drawWhat();
            else drawConfirmation();
          }
          else if(e.which == 'o')
          {
            if(e.who == '0') drawWho();
            else drawConfirmation();
          }
          else if(e.which == 'e')
          {
            if(e.when == '0') drawWhen();
            else drawConfirmation();
          }
          else if(e.which == 'r')
          {
            if(e.where == '0') drawWhere();
            else drawConfirmation();
          }
        }
      }
    }
    else drawWaiting(); //wait
  }

  for(int i = 0; i < 4; i++)
  {
    cardImgs[i].draw(graphics);
    cardHealth[i].draw(graphics);
    cardActIcons[i].draw(graphics);
  }

}

void PlayScene::leave()
{
}
void PlayScene::pass()
{

}
void PlayScene::pop()
{
}

PlayScene::~PlayScene()
{

}



SDL_Rect PlayScene::rectForPosition(char c)
{
  return positionRects[Compass::icardinal(c)];
}

SDL_Rect PlayScene::rectForCardinal(char card)
{
  char me = c->myCardinal();
  while(me != 's')
  {
    me   = Compass::cwcardinal(me);
    card = Compass::cwcardinal(card);
  }
  return rectForPosition(card);
}

SDL_Rect PlayScene::rectForDay(char d)
{
  return dayRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForSun(char d)
{
  return sunRects[Week::iday(d)];
}

SDL_Rect PlayScene::rectForTraversal(char fcard, char tcard, float t)
{
  return LerpRect::lerp(rectForCardinal(fcard), rectForCardinal(tcard), t);
}

SDL_Rect PlayScene::rectForExpansion(char card, float t)
{
  SDL_Rect tmp;
  return tmp;
}

SDL_Rect PlayScene::rectForTransition(char fd, char td, float t)
{
  LerpRect x(rectForDay(fd),rectForDay(td));
  return x.v(t);
}

