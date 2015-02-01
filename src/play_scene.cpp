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
    if(in.type == In::DOWN || in.type == In::MOVE) chooseShownDay(in);
    else if(in.type == In::UP)                     sunDragging = false;
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
    default: break;
  }

  if(s && c->model.roundOver() && in.type == In::DOWN && reset_game_button.query(in))
    c->requestReset();
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
    current_days += 0.005f;
    if(current_days >= goal_days)
    {
      current_days = goal_days;
      setViewState(IDLE);
    }
    shown_days = current_days; //force showing of "currently progressing day"
  }

  if(state != WAITING && c->iHaveTurn())
    setViewState(WAITING);

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

  //compute intermediate state
  int health[4] = {2,2,2,2};
  if(t != 0.0f)
  {
    circQ<Action,4> defendActions;    circQ<int,4> defendActionsWho;
    circQ<Action,4> attackActions;    circQ<int,4> attackActionsWho;
    circQ<Action,4> retaliateActions; circQ<int,4> retaliateActionsWho;
    circQ<Action,4> sabotageActions;  circQ<int,4> sabotageActionsWho;
    circQ<Action,4> messageActions;   circQ<int,4> messageActionsWho;
    Action *action;
    Turn turns[4];
    for(int i = 0; i < 4; i++)
      turns[i] = c->model.cardinalDayTurn(Compass::cardinal(i), base_day);

    for(int i = 0; i < 4; i++) //defend
      if((action = turns[i].action('d'))) { defendActions.enqueue(*action); defendActionsWho.enqueue(i); }
    for(int i = 0; i < 4; i++) //attack
      if((action = turns[i].action('a'))) { attackActions.enqueue(*action); attackActionsWho.enqueue(i); }
    for(int i = 0; i < 4; i++) //retaliate
      if((action = turns[i].action('d'))) { retaliateActions.enqueue(*action); retaliateActionsWho.enqueue(i); }
    for(int i = 0; i < 4; i++) //sabotage
      if((action = turns[i].action('s'))) { sabotageActions.enqueue(*action); sabotageActionsWho.enqueue(i); }
    for(int i = 0; i < 4; i++) //message
      if((action = turns[i].action('m'))) { messageActions.enqueue(*action); messageActionsWho.enqueue(i); }

    const float n = 5.0f;
    const float x = 1.0f/n;

    float lt = t; //0.0-1.0 for phase
    while(lt > x) lt -= x;
    lt /= x;

    action = 0;

    if(x*(1.0f-0.25f) < t) //already defended
    {
      while((action = defendActions.next())) health[*defendActionsWho.next()]++;

      if(x*(2.0f-0.25f) < t) //already attacked
      {
        while((action = attackActions.next())) { health[*attackActionsWho.next()]--; health[Compass::icardinal(action->who)]--; }

        if(x*(3.0f-0.25f) < t) //already retaliated
        {
          while((action = retaliateActions.next()))
          {
            int c = *retaliateActionsWho.next();
            int e;
            if(health[Compass::icardinal(action->who)] > 1)
            {
              Action *a;

              //cw attack
              e  = Compass::icardinal(Compass::cwcardinal(Compass::cardinal(c)));
              if((a = turns[e].action('a')) && a->who == Compass::cardinal(c))
              { health[c]--; health[e]--; }

              //ccw attack
              e = Compass::icardinal(Compass::ccwcardinal(Compass::cardinal(c)));
              if((a = turns[e].action('a')) && a->who == Compass::cardinal(c))
              { health[c]--; health[e]--; }
            }
          }
        }
        else //retaliate phase
        {
        }
      }
      else //attack phase
      {
      }
    }
    else //defend phase
    {
    }
  }

  //draws cardinals and actions
  for(int i = 0; i < 4; i++)
  {
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
      else if(a0.what == 's') read_sabotage_0.draw(graphics);
      else if(a1.what == 's') read_sabotage_1.draw(graphics);
      if(!c->model.roundOver()) picker.draw(graphics);
      else if(c->iWin())  win_img.draw(graphics);
      else if(c->iLose()) lose_img.draw(graphics);
      else if(c->iTie())  tie_img.draw(graphics);

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

