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

  read_sabotage_0 = UI::ImageButtonRound(Sprite::knife,      10,wh-50,20,20);
  read_sabotage_1 = UI::ImageButtonRound(Sprite::knife,      10,wh-80,20,20);
  read_message    = UI::ImageButtonRound(Sprite::envelope,ww-30,wh-50,20,20);

  loading = UI::Anim(Sprite::loading_anim, 3, 1.f, ww/2-250, wh/2-120,40,40);
  waiting_on_players_label = UI::Label("waiting on players...", wh/2-20, 200, 40);
  reset_game_button        = UI::TextButton("reset game", ww/2-100, wh/2-20, 200, 40);

  win_img  = UI::Image(Sprite::sun, ww/2-100, wh/2-100, 200, 200);
  lose_img = UI::Image(Sprite::ex,  ww/2-100, wh/2-100, 200, 200);
  tie_img  = UI::Image(Sprite::sun, ww/2-100, wh/2-100, 200, 200);

  picker = TurnPicker(UI::Box(0,0,ww,wh));
  picker.init();
  messager = Messager();

  current_days = 0;
  setViewState(IDLE);
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

void PlayScene::setViewState(PLAY_SCENE_STATE s)
{
  state = s;

  shown_days = current_days;

  sunDragging = false;
  picker.clearViewState();
  messager.clearViewState();
}

bool PlayScene::chooseShownDay(In &in)
{
  if(sunBtn.query(in)) sunDragging = true;

  if(sunDragging)
  {
    int firstX = dayRects[0].x+(dayRects[0].w/2);
    int lastX  = dayRects[6].x+(dayRects[6].w/2);
    shown_days = ((float)(in.x-firstX)/(float)(lastX-firstX))*6.0f;
  }

  return sunDragging;
}

void PlayScene::drawWaiting()
{
  loading.draw(graphics);
  waiting_on_players_label.draw(graphics);
}

void PlayScene::drawReset()
{
  reset_game_button.draw(graphics);
}

void PlayScene::touch(In &in)
{
  if(shown_days != current_days) //history
  {
    if((in.type == In::DOWN || in.type == In::MOVE)) chooseShownDay(in);
    else if(in.type == In::UP)                       sunDragging = false;
    return;
  }

  Action a, a0, a1;
  Turn t;
  switch(state)
  {
    case IDLE:
      if(in.type == In::DOWN && chooseShownDay(in)) break; //go to history mode
      if(in.type == In::DOWN)
      {
        c->myMessage(a);
        c->mySabotage(a0,a1);
        if(picker.query(in)) { setViewState(TURN_PICKING); picker.touch(in); }
        else if(a.what == 'm'  && read_message.query(in))    { setViewState(MESSAGE); messager.setMessage(a); }
        else if(a0.what == 's' && read_sabotage_0.query(in)) { setViewState(MESSAGE); messager.setMessage(a0); }
        else if(a1.what == 's' && read_sabotage_1.query(in)) { setViewState(MESSAGE); messager.setMessage(a1); }
      }
      break;
    case MESSAGE:
      if(in.type == In::UP)
        setViewState(IDLE);
      break;
    case TURN_PICKING:
      picker.touch(in);
      if(picker.getTurn(t))
      {
        c->commitTurn(t);
        setViewState(WAITING);
      }
      break;
    case WAITING:
      if(in.type == In::DOWN && chooseShownDay(in)) break; //go to history mode
      break;
    case SHOWING:
      //interaction disabled
      break;
    case OVER:
      if(in.type == In::DOWN && chooseShownDay(in)) break; //go to history mode
      if(s && in.type == In::DOWN && reset_game_button.query(in))
        c->requestReset();
      break;
    default: break;
  }
}

int PlayScene::tick()
{
  if(s) s->tick();
  c->tick();

  for(int i = 0; i < 4; i++)
    cardImgs[i].tick(0.4f);
  picker.tick();
  messager.tick();
  loading.tick(0.4f);

  if(goal_days != c->model.days)
  {
    if(c->model.days == -1) return -1; //game was reset- go back to room
    goal_days = c->model.days;
  }

  if(current_days < goal_days)
  {
    setViewState(SHOWING);
    current_days += 0.01f;
    if(current_days >= goal_days)
    {
      current_days = goal_days;
      setViewState(IDLE);
    }
    shown_days = current_days; //force showing of "currently progressing day"
  }

  if(state != WAITING && c->iHaveTurn())
    setViewState(WAITING);
  if(state != WAITING && state != SHOWING && c->model.roundOver())
    setViewState(OVER);

  return 0;
}

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
void PlayScene::draw()
{
  float snapped_shown_days = clamp(snapToInt(shown_days), 0, current_days);
  int base_day = (int)snapped_shown_days;
  float t = snapped_shown_days-((float)base_day);

  SDL_Rect sunr = rectForTransition(Week::day(base_day%7), Week::day((base_day+1)%7), t);
  graphics->draw(Sprite::sun,sunr);
  sunBtn.rect.rect = sunr;
  for(int i = 0; i < 7; i++)
    dayLbls[i].draw(graphics);

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
    char card = Compass::cardinal(i);
    Turn turn;
    Turn cwturn;
    Turn ccwturn;
    Turn pturn;

    cardImgs[i].draw(graphics);
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

    if(t != 0.0f)
    {
      const float n = 6.0f;
      const float x = 1.0f/n;
      float lt; //0.0-1.0 for phase
      turn    = c->model.cardinalDayTurn(card, base_day);
      cwturn  = c->model.cardinalDayTurn(Compass::cwcardinal(card), base_day);
      ccwturn = c->model.cardinalDayTurn(Compass::ccwcardinal(card), base_day);
      pturn   = c->model.cardinalDayTurn(Compass::opcardinal(card), base_day);

      bool imDefending = turn.actions[0].what == 'd' || turn.actions[1].what == 'd';
      bool beingAttackedCW =
          ((cwturn.actions[0].what == 'a' && cwturn.actions[0].who == card) ||
           (cwturn.actions[1].what == 'a' && cwturn.actions[1].who == card));
      bool beingAttackedCCW =
          ((ccwturn.actions[0].what == 'a' && ccwturn.actions[0].who == card) ||
           (ccwturn.actions[1].what == 'a' && ccwturn.actions[1].who == card));
      int beingAttacked = beingAttackedCW + beingAttackedCCW;


      if(t < x*1.0f) //defend
      {
        lt = (t-(x*1.0f))*n;
        if(imDefending)
          graphics->draw(Sprite::shield, heart_2);
        graphics->draw(Sprite::heart, heart_0);
        graphics->draw(Sprite::heart, heart_1);
      }
      else if(t < x*2.0f) //attack
      {
        lt = (t-(x*2.0f))*n;
        if(imDefending && (lt < 0.3f || !beingAttacked))
          graphics->draw(Sprite::shield, heart_2);
        if(lt < 0.3f ||
          (lt < 0.6f && (imDefending || !beingAttacked)) ||
          (lt < 1.0f && ((imDefending && beingAttacked <= 1)))
          )
          graphics->draw(Sprite::heart, heart_1);
        else if(turn.actions[0].what == 'a' ||
           turn.actions[1].what == 'a')
          graphics->draw(Sprite::sword, heart_2);
        graphics->draw(Sprite::heart, heart_0);
      }
      else if(t < x*3.0f) //retaliate
      {
        lt = (t-(x*3.0f))*n;
        if(turn.actions[0].what == 'd' ||
           turn.actions[1].what == 'd')
          graphics->draw(Sprite::shield, heart_2);
      }
      else if(t < x*4.0f) //sabotage
      {
        lt = (t-(x*4.0f))*n;
        if(turn.actions[0].what == 's' ||
           turn.actions[1].what == 's')
          graphics->draw(Sprite::knife, heart_2);
      }
      else if(t < x*5.0f) //message
      {
        lt = (t-(x*5.0f))*n;
        if(turn.actions[0].what == 'm' ||
           turn.actions[1].what == 'm')
          graphics->draw(Sprite::envelope, heart_2);
      }
      else if(t < x*6.0f) //scout
      {
        lt = (t-(x*6.0f))*n;
        if(turn.actions[0].what == 'c' ||
           turn.actions[1].what == 'c')
          graphics->draw(Sprite::knife, heart_2);
      }

    /*
      Action a = c->model.cardinalDayTurn(card, base_day);
      if(a.what == 'a') graphics->draw(sword_s, rectForTraversal(card,e.who,t));
      if(a.what == 'd') graphics->draw(shield_full_s, rectForExpansion(card,t));
      if(a.what == 's') graphics->draw(knife, rectForExpansion(card,t));
      if(a.what == 'm') graphics->draw(envelope_s, rectForTraversal(card,e.route,t));
      a = c->model.cardinalDayTurn(card, base_day-1);
      if(a.what == 'm')
      {
        char at = a.route;
        char to = a.to;
        a = c->model.cardinalDayTurn(at, base_day-1);
        if(!(a.what == 's' && a.how == 'b'))
          graphics->draw(envelope_s, rectForTraversal(at,to,t));
      }
    */
    }
  }

  Action a, a0, a1;
  switch(state)
  {
    case IDLE:
      c->myMessage(a);
      c->mySabotage(a0,a1);
           if(a.what == 'm')  read_message.draw(graphics);
      else if(a0.what == 's') read_sabotage_0.draw(graphics);
      else if(a1.what == 's') read_sabotage_1.draw(graphics);
      picker.draw(graphics);
      break;
    case MESSAGE:
      messager.draw(graphics);
      break;
    case TURN_PICKING:
      picker.draw(graphics);
      break;
    case WAITING:
      loading.draw(graphics);
      break;
    case SHOWING:
      //interaction disabled
      break;
    case OVER:
           if(c->iWin())  win_img.draw(graphics);
      else if(c->iLose()) lose_img.draw(graphics);
      else if(c->iTie())  tie_img.draw(graphics);
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

