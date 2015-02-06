#include "play_scene.h"
#include "graphics.h"
#include "input.h"
#include "sprite.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include "logger.h"

#include "compass.h"
#include "week.h"

#include "SDL.h"

static float snapToInt(float v)
{
  float left = (float)((int)v);
  float right = (float)((int)(v+1.0f));
  if(v < left +0.01f) return left;
  if(v > right-0.01f) return right;
  return v;
}
static float clamp(float v, float min, float max)
{
  if(v < min) return min;
  if(v > max) return max;
  return v;
}

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

  //Rects
  int ws[] = {          90,          110,          120,          110};
  int hs[] = {          90,          110,          120,          110};
  int xs[] = {ww/2-ws[0]/2,  ww-ws[1]-20, ww/2-ws[2]/2,           20};
  int ys[] = {          60, wh/2-hs[1]/2,  wh-hs[2]-40, wh/2-hs[3]/2};

  for(int i = 0; i < 4; i++)
  {
    posRects[i]      = UI::Box(xs[i],       ys[i], ws[i],   hs[i]).rect;
    posLabelRects[i] = UI::Box(xs[i], ys[i]+hs[i], ws[i], hs[i]/3).rect;
  }

  for(int i = 0; i < 7; i++)
  {
    dayRects[i] = UI::Box(space(ww,60,40,7,i), 20, 40, 40).rect;
    sunRects[i] = UI::Box(space(ww,50,60,7,i), 10, 60, 60).rect;
  }

  //Hacks
  char daynamehacks[] = {'S','u','\0','M','o','\0','T','u','\0','W','e','\0','T','h','\0','F','r','\0','S','a','\0'};
  char cardnamehacks[] = {'N','o','r','t','h','\0','E','a','s','t','\0','\0','S','o','u','t','h','\0','W','e','s','t','\0','\0'};

  //Views
  for(int i = 0; i < 4; i++)
  {
    cardImgs[i] = UI::Anim(Sprite::general_anims[i], 4, 1.f, posRects[i]);
    cardLbls[i] = UI::Label(&cardnamehacks[i*6], posLabelRects[i]);
  }

  for(int i = 0; i < 7; i++)
    dayLbls[i] = UI::Label(&daynamehacks[i*3], dayRects[i]);

  sunBtn = UI::Button(dayRects[0]);

  read_sabotage_0 = UI::ImageButtonRound(Sprite::knife,       20,wh-100,100,30);
  read_sabotage_1 = UI::ImageButtonRound(Sprite::knife,       20,wh- 50,100,30);
  read_message    = UI::ImageButtonRound(Sprite::envelope,ww-150,wh- 50,100,30);

  loading = UI::Anim(Sprite::loading_anim, 3, 1.f, ww/2-250, wh/2-120,40,40);
  waiting_on_players_label = UI::Label("waiting on players...", wh/2-20, 200, 40);
  reset_game_button        = UI::TextButton("reset game", ww/2-100, wh/2-20, 200, 40);

  win_img  = UI::Image(Sprite::sun, ww/2-100, wh/2-100, 200, 200);
  lose_img = UI::Image(Sprite::ex,  ww/2-100, wh/2-100, 200, 200);
  tie_img  = UI::Image(Sprite::sun, ww/2-100, wh/2-100, 200, 200);

  picker = TurnPicker(UI::Box(0,0,ww,wh));
  picker.init();
  messager = Messager();

  known_days = 0.0f;
  shown_days = 0.0f;
  showing_days = 0.0f;
  state = IDLE;
}

void PlayScene::enter()
{
  //need to wait until c_model is obtained
  client = *client_ptr;
  s = *s_ptr;
  c = *c_ptr;

  //syncs position rects with assigned cardinal rects
  char card = c->myCardinal(); int ic = Compass::icardinal(card);
  char pos  = 's';             int ip = Compass::icardinal(pos);
  for(int i = 0; i < 4; i++)
  {
    cardRects[ic]      = posRects[ip];
    cardLabelRects[ic] = posLabelRects[ip];
    card = Compass::cwcardinal(card); ic = Compass::icardinal(card);
    pos  = Compass::cwcardinal(pos);  ip = Compass::icardinal(pos);
  }

  //places general images+labels
  for(int i = 0; i < 4; i++)
  {
    cardImgs[i] = UI::Anim(Sprite::general_anims[i], 4, 1.f, cardRects[i]);
    cardLbls[i].rect = cardLabelRects[i];
  }

  picker.setCardinal(c->myCardinal());
}

bool PlayScene::chooseShowingDay(In &in)
{
  if(sunBtn.query(in)) sunDragging = true;

  if(sunDragging)
  {
    int firstX = dayRects[0].x+(dayRects[0].w/2);
    int lastX  = dayRects[6].x+(dayRects[6].w/2);
    showing_days = ((float)(in.x-firstX)/(float)(lastX-firstX))*6.0f;
    showing_days = clamp(showing_days, 0, shown_days);
  }

  return sunDragging;
}

void PlayScene::touch(In &in)
{
  Action a, a0, a1;
  switch(state)
  {
    case IDLE:
      if(in.type == In::DOWN)
      {
        c->myMessage(a);
        c->mySabotage(a0,a1);
        if(chooseShowingDay(in)) state = VIEWING;
        if(!c->iHaveTurn() && !c->model.roundOver() && picker.touch(in)) { state = TURN_PICKING; }
        else
        {
          if(a.what == 'm'  && read_message.query(in))    { state = MESSAGE; messager.setMessage(a); }
          if(a0.what == 'm' && read_sabotage_0.query(in)) { state = MESSAGE; messager.setMessage(a0); }
          if(a1.what == 'm' && read_sabotage_1.query(in)) { state = MESSAGE; messager.setMessage(a1); }
        }
      }
      break;
    case MESSAGE:
      if(in.type == In::UP) state = IDLE;
      break;
    case TURN_PICKING:
      if(!picker.touch(in)) state = IDLE;
      break;
    case VIEWING:
      if(in.type == In::DOWN || (in.type == In::MOVE && sunDragging)) chooseShowingDay(in);
      if(in.type == In::UP) sunDragging = false;
      if(!sunDragging && showing_days == shown_days) state = IDLE;
      break;
    case SHOWING:
      //interaction disabled
      break;
    default: break;
  }

  if(s && c->model.roundOver() && in.type == In::DOWN && reset_game_button.query(in))
    c->requestReset();
}

//model->days  - latest known state
//known_days   - scene known state (syncs w/ model->days ASAP, uses negative change as evidence of round reset)
//shown_days   - latest state shown at least once
//showing_days - currently visible days

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  for(int i = 0; i < 4; i++)
    cardImgs[i].tick(0.4f);
  picker.tick();
  messager.tick();
  loading.tick(0.4f);

  if(known_days != c->model.days)
  {
    picker.reset();
    if(c->model.days == -1) return -1; //game was reset- go back to room
    known_days = c->model.days;
    if(known_days == 0)
    {
      shown_days = 0.0f;;
      showing_days = 0.0f;;
    }
    else
    {
      shown_days = known_days-1.0f;
      showing_days = shown_days;
    }
  }

  if(shown_days < known_days)
  {
    state = SHOWING;
    sunDragging = false;

    shown_days += 0.005f;
    if(shown_days >= known_days)
    {
      shown_days = known_days;
      state = IDLE;
    }
    showing_days = shown_days; //force showing of "currently progressing day"
  }

  Turn t;
  if(picker.getTurn(t)) c->commitTurn(t);

  switch(state)
  {
    case IDLE:
      //tick nothin
      break;
    case MESSAGE:
      //tick nothin
      break;
    case TURN_PICKING:
      //tick nothin
      break;
    case VIEWING:
      //tick nothin
      break;
    case SHOWING:
      //tick nothin
      break;
    default: break;
  }

  return 0;
}

void PlayScene::draw()
{
  float snapped_showing_days = clamp(snapToInt(showing_days), 0, shown_days);
  int base_showing_day = (int)snapped_showing_days;
  float t = snapped_showing_days-((float)base_showing_day);

  SDL_Rect sunr = rectForTransition(Week::day(base_showing_day%7), Week::day((base_showing_day+1)%7), t);
  graphics->draw(Sprite::sun,sunr);
  sunBtn.rect.rect = sunr;
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  //compute intermediate state
  int health[4] = {2,2,2,2};
  bool cardsDrawn[4] = {false,false,false,false};
  if(t != 0.0f)
  {

    circQ<Action,4> defendActions;    circQ<int,4> defendActionsWho;    int nDefends    = 0;
    circQ<Action,4> attackActions;    circQ<int,4> attackActionsWho;    int nAttacks    = 0;
    circQ<Action,4> retaliateActions; circQ<int,4> retaliateActionsWho; circQ<int,4> retaliateActionsAgainst; int nRetaliates = 0;
    circQ<Action,4> sabotageActions;  circQ<int,4> sabotageActionsWho;  int nSabotages  = 0;
    circQ<Action,4> messageActions;   circQ<int,4> messageActionsWho;   int nMessages   = 0;
    circQ<Action,4> ymessageActions;  circQ<int,4> ymessageActionsWho;  int nYMessages  = 0; //yesterday's messages

    { //to scope turns/action

      Turn turns[4];
      Action *action;
      for(int i = 0; i < 4; i++)
        turns[i] = c->model.cardinalDayTurn(Compass::cardinal(i), base_showing_day);

    /* defends    */ for(int i = 0; i < 4; i++) if((action = turns[i].action('d'))) { defendActions.enqueue(*action);    defendActionsWho.enqueue(i);    nDefends++;    }
    /* attacks    */ for(int i = 0; i < 4; i++) if((action = turns[i].action('a'))) { attackActions.enqueue(*action);    attackActionsWho.enqueue(i);    nAttacks++;    }
    /* retaliates */ for(int i = 0; i < 4; i++) if((action = turns[i].action('d'))) //special case because more complicated
    {
      Action *a;
      int eToRetaliate = 0; //gets set if exactly one enemy attacked
      int e;

      //cw attack
      e = Compass::icardinal(Compass::cwcardinal(Compass::cardinal(i)));
      if((a = turns[e].action('a')) && a->who == Compass::cardinal(i))
        eToRetaliate = e;

      //ccw attack
      e = Compass::icardinal(Compass::ccwcardinal(Compass::cardinal(i)));
      if((a = turns[e].action('a')) && a->who == Compass::cardinal(i))
        eToRetaliate = eToRetaliate ? 0 : e;

      if(eToRetaliate)
      {
        retaliateActions.enqueue(*action);
        retaliateActionsWho.enqueue(i);
        retaliateActionsAgainst.enqueue(eToRetaliate);
        nRetaliates++;
      }
    }
    /* sabotages  */ for(int i = 0; i < 4; i++) if((action = turns[i].action('s'))) { sabotageActions.enqueue(*action);  sabotageActionsWho.enqueue(i);  nSabotages++;  }
    /* messages   */ for(int i = 0; i < 4; i++) if((action = turns[i].action('m'))) { messageActions.enqueue(*action);   messageActionsWho.enqueue(i);   nMessages++;   }

    if(base_showing_day > 0) //check for yesterday's messages
    {
      for(int i = 0; i < 4; i++)
        turns[i] = c->model.cardinalDayTurn(Compass::cardinal(i), base_showing_day-1);

      /* ymessages   */ for(int i = 0; i < 4; i++) if((action = turns[i].action('m')))
      {
        Action *a;
        if(!(a = turns[Compass::icardinal(action->route)].action('s')) || a->how != 'b')
        {
          ymessageActions.enqueue(*action);
          ymessageActionsWho.enqueue(i);
          nYMessages++;
        }
      }
    }

    }

    int nActions = (nDefends   > 0 ? 1 : 0)   + //any defends happen simultaneously
                   (nAttacks)                 + //all attacks get played out individually
                   (nRetaliates)              + //all retaliations get played out individually
                   (nSabotages > 0 ? 1 : 0)   + //any sabotages happen simultaneously
                   (nMessages+nYMessages  > 0 ? 1 : 0)   + //any messages happen simultaneously
                   1; //for "heal" phase

    const float n = (float)nActions;
    const float plen = 1.0f/n;
    float st = 0.0; //simulated time

    Action *action = 0;
    int card;

    //Defends
    if(nDefends > 0 && st+plen < t)
    {
      while((action = defendActions.next())) health[*defendActionsWho.next()]++;

      nDefends = 0;
      st += plen;
    }
    if(nDefends > 0 && st < t)
    {
      while((action = defendActions.next())) health[*defendActionsWho.next()]++;

      nDefends = 0;
      st += plen;
    }

    //Attacks
    while(nAttacks > 0 && st+plen < t)
    {
      action = attackActions.next();
      health[*attackActionsWho.next()]--;
      health[Compass::icardinal(action->who)]--;

      nAttacks--;
      st += plen;
    }
    if(nAttacks > 0 && st < t)
    {
      action = attackActions.next();
      card = *attackActionsWho.next();
      graphics->draw(cardImgs[card].curSprite(),rectForTraversal(Compass::cardinal(card), action->who, (t-st)/plen));
      cardsDrawn[card] = true;

      nAttacks--;
      st += plen;
    }

    //Retaliate
    while(nRetaliates > 0 && st+plen < t)
    {
      action = retaliateActions.next();
      health[*retaliateActionsWho.next()]--;
      health[*retaliateActionsAgainst.next()]--;

      nRetaliates--;
      st += plen;
    }
    if(nRetaliates > 0 && st < t)
    {
      action = retaliateActions.next();
      card = *retaliateActionsWho.next();
      int e = *retaliateActionsAgainst.next();

      graphics->draw(cardImgs[card].curSprite(),rectForTraversal(Compass::cardinal(card), Compass::cardinal(e), (t-st)/plen));
      cardsDrawn[card] = true;

      nRetaliates--;
      st += plen;
    }

    //Sabotages
    if(nSabotages > 0 && st+plen < t)
    {
      //no need to simulate anything
      nSabotages = 0;
      st += plen;
    }
    if(nSabotages > 0 && st < t)
    {
      while(nSabotages > 0)
      {
        action = sabotageActions.next();
        card = *sabotageActionsWho.next();

        graphics->draw(Sprite::knife, cardRects[card]);
        nSabotages--;
      }
      st += plen;
    }

    //Messages
    if(nMessages+nYMessages > 0 && st+plen < t)
    {
      //no need to simulate anything
      nMessages = 0;
      nYMessages = 0;
      st += plen;
    }
    if(nMessages+nYMessages > 0 && st < t)
    {
      while(nMessages > 0)
      {
        action = messageActions.next();
        card = *messageActionsWho.next();

        graphics->draw(Sprite::envelope,rectForTraversal(Compass::cardinal(card), action->route, (t-st)/plen));
        nMessages--;
      }
      while(nYMessages > 0)
      {
        action = ymessageActions.next();
        card = *ymessageActionsWho.next();

        graphics->draw(Sprite::envelope,rectForTraversal(action->route, Compass::opcardinal(Compass::cardinal(card)), (t-st)/plen));
        nYMessages--;
      }
      st += plen;
    }
  }

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
    if(!cardsDrawn[i]) cardImgs[i].draw(graphics);
    cardLbls[i].draw(graphics);
    SDL_Rect heart_0;
    SDL_Rect heart_1;
    SDL_Rect heart_2;
    heart_0 = cardImgs[i].rect.rect;
    heart_0.w /= 4;
    heart_0.h /= 4;
    heart_0.x += heart_0.w*2;
    heart_1 = heart_0;
    heart_1.x += heart_1.w*0.75;
    heart_2 = heart_1;
    heart_2.x += heart_2.w*0.75;

    if(health[i] > 0) graphics->draw(Sprite::heart,  heart_0);
    if(health[i] > 1) graphics->draw(Sprite::heart,  heart_1);
    if(health[i] > 2) graphics->draw(Sprite::shield, heart_2);
  }

  Action a, a0, a1;
  switch(state)
  {
    case IDLE:
      c->myMessage(a);
      c->mySabotage(a0,a1);
      if(a.what == 'm')  read_message.draw(graphics);
      if(a0.what == 'm') read_sabotage_0.draw(graphics);
      if(a1.what == 'm') read_sabotage_1.draw(graphics);
      if(!c->model.roundOver())
      {
        if(!c->iHaveTurn()) picker.draw(graphics);
        else                loading.draw(graphics);
      }
      else
      {
        if(s) reset_game_button.draw(graphics);
             if(c->iWin())  win_img.draw(graphics);
        else if(c->iLose()) lose_img.draw(graphics);
        else if(c->iTie())  tie_img.draw(graphics);
      }
      break;
    case MESSAGE:
      messager.draw(graphics);
      break;
    case TURN_PICKING:
      picker.draw(graphics);
      break;
    case VIEWING:
      //already drawn- t != 0.0f
      break;
    case SHOWING:
      //already drawn- t != 0.0f
      break;
    default: break;
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

SDL_Rect PlayScene::rectForTraversal(char fcard, char tcard, float t)
{
  return LerpRect::lerp(cardRects[Compass::icardinal(fcard)], cardRects[Compass::icardinal(tcard)], t);
}

SDL_Rect PlayScene::rectForExpansion(char card, float t)
{
  SDL_Rect tmp = cardRects[Compass::icardinal(card)];
  tmp.x -= tmp.w/2;
  tmp.y -= tmp.h/2;
  tmp.w *= 2;
  tmp.h *= 2;
  return LerpRect::lerp(cardRects[Compass::icardinal(card)], tmp, t);
}

SDL_Rect PlayScene::rectForTransition(char fd, char td, float t)
{
  return LerpRect::lerp(sunRects[Week::iday(fd)], sunRects[Week::iday(td)], t);
}

