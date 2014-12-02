#include "particles.h"
#include "graphics.h"
#include "sprite.h"
#include "logger.h"

ParticleSys::ParticleSys()
{

}

ParticleSys::~ParticleSys()
{

}

void ParticleSys::registerP(Particle p)
{
  particles.add(p);
}

bool tickParticle(Particle& p, float d) //returns survival
{
  switch(p.type)
  {
    case P_TYPE_MESSENGER:
      p.mess.x.tick(d);
      p.mess.y.tick(d);
      return p.mess.x.t != 1.0f;
      break;
    case P_TYPE_DEFEND:
      p.defend.w.tick(d);
      p.defend.h.tick(d);
      return p.defend.w.t != 1.0f;
      break;
    case P_TYPE_ATTACK:
      p.attack.x.tick(d);
      p.attack.y.tick(d);
      p.attack.anim.tick(d);
      return p.attack.x.t != 1.0f;
      break;
    case P_TYPE_COUNT: break; //nothing
    default: break; //nothing
  }
  return false;
}

void ParticleSys::tick(float d)
{
  for(int i = 0; i < particles.len(); i++)
  {
    if(!tickParticle(particles[i], d))
    {
      particles.remove(i); i--;
    }
  }
}

void drawParticle(Particle& p, Graphics *g)
{
  SDL_Rect tmp;
  int f;
  switch(p.type)
  {
    case P_TYPE_MESSENGER:
      tmp.x = p.mess.x.v();
      tmp.y = p.mess.y.v();
      tmp.w = p.mess.w;
      tmp.h = p.mess.h;
      g->draw(Sprite::messenger(),tmp);
      break;
    case P_TYPE_DEFEND:
      tmp.x = p.defend.x-p.defend.w.v()/2;
      tmp.y = p.defend.y-p.defend.h.v()/2;
      tmp.w = p.defend.w.v();
      tmp.h = p.defend.h.v();
      g->draw(Sprite::circle(),tmp);
    break;
    case P_TYPE_ATTACK:
      tmp.x = p.attack.x.v();
      tmp.y = p.attack.y.v();
      tmp.w = p.attack.w;
      tmp.h = p.attack.h;
      f = (int)(p.attack.anim.v()/0.05f)%4;
      if(f == 0) g->draw(Sprite::stick_bs(),tmp);
      if(f == 1) g->draw(Sprite::stick_vert(),tmp);
      if(f == 2) g->draw(Sprite::stick_fs(),tmp);
      if(f == 3) g->draw(Sprite::stick_horiz(),tmp);
      break;
    case P_TYPE_COUNT: break; //nothing
    default: break; //nothing
  }
}

void ParticleSys::draw(Graphics *g)
{
  for(int i = 0; i < particles.len(); i++)
    drawParticle(particles[i],g);
}

