#include "room_scene.h"
#include "graphics.h"
#include "network.h"
#include "client.h"
#include "server_model.h"
#include "client_model.h"

#include <SDL.h>

#include "logger.h"

RoomScene::RoomScene(Graphics *g, Network::Client *&c, ServerModel *&sm, ClientModel *&cm)
{
  graphics = g;

  client_ptr = &c;
  s_model_ptr = &sm;
  c_model_ptr = &cm;
  client = 0;
  s_model = 0;
  c_model = 0;

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

  nLabel = UI::Label(ww/2-10,      10, 20, "N");  nButton = UI::Button(ww/2-10,      10, 20, 20); nBox = UI::Box(ww/2-10-5,      10-5, 30, 30);
  eLabel = UI::Label(ww  -30, wh/2-10, 20, "E");  eButton = UI::Button(ww  -30, wh/2-10, 20, 20); eBox = UI::Box(ww  -30-5, wh/2-10-5, 30, 30);
  sLabel = UI::Label(ww/2-10, wh  -30, 20, "S");  sButton = UI::Button(ww/2-10, wh  -30, 20, 20); sBox = UI::Box(ww/2-10-5, wh  -30-5, 30, 30);
  wLabel = UI::Label(     10, wh/2-10, 20, "W");  wButton = UI::Button(     10, wh/2-10, 20, 20); wBox = UI::Box(     10-5, wh/2-10-5, 30, 30);

  SCENE_CHANGE_HACK = 0;
}

void RoomScene::enter()
{
  client = *client_ptr;
  s_model = *s_model_ptr;
  c_model = *c_model_ptr;
}

void RoomScene::touch(In &in)
{
  if(backButton.query(in)) { }
  if(nButton.query(in))
  {
    General *g = c_model->cardGeneral('n');
    if(g && g->connection == client->connection) client->broadcast('n', e_type_revoke_card);
    else                                         client->broadcast('n', e_type_assign_card);
  }
  if(eButton.query(in))
  {
    General *g = c_model->cardGeneral('e');
    if(g && g->connection == client->connection) client->broadcast('e', e_type_revoke_card);
    else                                         client->broadcast('e', e_type_assign_card);
  }
  if(sButton.query(in))
  {
    General *g = c_model->cardGeneral('s');
    if(g && g->connection == client->connection) client->broadcast('s', e_type_revoke_card);
    else                                         client->broadcast('s', e_type_assign_card);
  }
  if(wButton.query(in))
  {
    General *g = c_model->cardGeneral('w');
    if(g && g->connection == client->connection) client->broadcast('w', e_type_revoke_card);
    else                                         client->broadcast('w', e_type_assign_card);
  }
  if(leaveSessButton.query(in)) { }
  if(beginGameButton.query(in))
  {
    if(c_model->cardGeneral('n') &&
       c_model->cardGeneral('w') &&
       c_model->cardGeneral('s') &&
       c_model->cardGeneral('e')
        )
    {
      client->broadcast(c_model->conGeneral(client->connection)->cardinal, e_type_begin_play);
    }
  }
}

int RoomScene::tick()
{
  if(s_model) s_model->tick();
  c_model->tick();

  if(c_model->playing) SCENE_CHANGE_HACK = 1;

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

  if(c_model->cardGeneral('n'))
  {
    nLabel.draw(graphics);
    if(c_model->cardGeneral('n')->connection == client->connection)
      nBox.draw(graphics);
  }
  if(c_model->cardGeneral('e'))
  {
    eLabel.draw(graphics);
    if(c_model->cardGeneral('e')->connection == client->connection)
      eBox.draw(graphics);
  }
  if(c_model->cardGeneral('s'))
  {
    sLabel.draw(graphics);
    if(c_model->cardGeneral('s')->connection == client->connection)
      sBox.draw(graphics);
  }
  if(c_model->cardGeneral('w'))
  {
    wLabel.draw(graphics);
    if(c_model->cardGeneral('w')->connection == client->connection)
      wBox.draw(graphics);
  }

  nButton.draw(graphics);
  sButton.draw(graphics);
  wButton.draw(graphics);
  eButton.draw(graphics);

  leaveSessLabel.draw(graphics);
  leaveSessButton.draw(graphics);

  if(s_model &&
     c_model->cardGeneral('n') &&
     c_model->cardGeneral('w') &&
     c_model->cardGeneral('s') &&
     c_model->cardGeneral('e')
    )
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

