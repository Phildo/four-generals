#include "join_scene.h"
#include "graphics.h"
#include "network.h"
#include "input.h"
#include "client.h"
#include "client_model.h"
#include "sprite.h"

#include "logger.h"

#include "SDL.h"

JoinScene::JoinScene(Graphics *g, Network::Client *& c, ClientModel *& cm)
{
  graphics = g;

  int ww = graphics->winWidth();
  int wh = graphics->winHeight();

  backButton = UI::Button(20,20,40,40);

  joinGameLabel = UI::Label("Join Game", ww/2-250, wh/2-170,40);

  manual    = UI::TextButton("Manual Entry", ww/2-250, 50,500,40);
  automatic = UI::TextButton("Automatic Entry", ww/2-250, 50,500,40);

  searchJoinButton = UI::TextButton("Search For Game", ww/2-250, wh/2-120,500,40);
  ipPrefix = Network::getIP();
  char ipPrefBuff[3+1+3+1+3+1];
  int dotCt = 0;
  int i;
  for(i = 0; i < ipPrefix.len() && dotCt < 3; i++)
  {
    if(*(ipPrefix.ptr()+i) == '.') dotCt++;
    ipPrefBuff[i] = *(ipPrefix.ptr()+i);
  }
  if(dotCt == 3) ipPrefix = String(ipPrefBuff,i);

  joinButton = UI::TextButton( "Join Session", ww/2-250, wh/2-70, 500, 40);
  portLabel  = UI::Label("4040", ww/2+150, wh/2-120,40);
  ipInput    = UI::TextBox(ww/2-250, wh/2-120,400,40);
  keyboard   = UI::Keyboard(0,wh-200,ww,200);
  loading    = UI::Anim(Sprite::loading_anim, 3, 1.f, ww/2-250, wh/2-120,40,40);

  ipInput.setText(Network::getIP());

  client_ptr = &c;
  c_model_ptr = &cm;

  reset();
}

void JoinScene::reset()
{
  client = 0;
  c_model = 0;

  manualEntry = false;
  searching = 0;

  SCENE_CHANGE_HACK = 0;
}

void JoinScene::enter()
{
  client = *client_ptr;
  c_model = *c_model_ptr;
}

void JoinScene::touch(In &in)
{
  if(in.type != In::DOWN) return;

  if(backButton.query(in))
  {
    searching = 0;
    SCENE_CHANGE_HACK = -2;
  }

  if(manualEntry)
  {
    keyboard.touch(in);
    if(joinButton.query(in))
    {
      if(!client) { client = new Network::Client(); *client_ptr = client; }
      if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(ipInput.getText(),4040);
    }
    if(automatic.query(in))
    {
      manualEntry = false;
      searching = 0;
    }
  }
  else
  {
    if(searchJoinButton.query(in))
    {
      if(!searching)
      {
        searching = 1; //kick off actual search here, pick up in tick
        if(!client) { client = new Network::Client(); *client_ptr = client; }
        if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED) client->connect(ipPrefix.concat(String::decimalRep(searching)),4040);
      }
    }
    if(manual.query(in))
    {
      manualEntry = true;
    }
  }
}

int JoinScene::tick()
{
  loading.tick(0.2f);
  if(client)
  {
    if(client->con_state == Network::CONNECTION_STATE_CONNECTED)
    {
      c_model = new ClientModel(client);
      *c_model_ptr = c_model;
      SCENE_CHANGE_HACK = 1;
    }
    if(client->con_state == Network::CONNECTION_STATE_STALE)
      client->disconnect();
    if(searching)
    {
      if(client->con_state == Network::CONNECTION_STATE_DISCONNECTED)
      {
        searching++;
        if(searching == 256) searching = 0;
        else client->connect(ipPrefix.concat(String::decimalRep(searching)),4040); //PHIL HACK
        //else client->connect(String("localhost"),4040); //PHIL HACK
      }
    }
  }

  if(manualEntry)
  {
    char c = keyboard.poll();
    if(c == '<') ipInput.backspace();
    else if(c != 0) ipInput.input(c);
  }

  int tmp = SCENE_CHANGE_HACK;
  SCENE_CHANGE_HACK = 0;
  return tmp;
}

void JoinScene::draw()
{
  backButton.draw(graphics);
  joinGameLabel.draw(graphics);

  if(manualEntry)
  {
    if(client && client->con_state != Network::CONNECTION_STATE_DISCONNECTED)
    {
      loading.draw(graphics);
    }
    else
    {
      joinButton.draw(graphics);
      keyboard.draw(graphics);
      automatic.draw(graphics);
    }
    portLabel.draw(graphics);
    ipInput.draw(graphics);
  }
  else
  {
    if(searching)
    {
      loading.draw(graphics);
    }
    else
    {
      searchJoinButton.draw(graphics);
      manual.draw(graphics);
    }
  }
}

void JoinScene::leave()
{
  reset();
}
void JoinScene::pass()
{

}
void JoinScene::pop()
{
}

JoinScene::~JoinScene()
{
}

