#include "particles.h"
#include "graphics.h"
#include "sprite.h"

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
      p.mess.t += d; if(p.mess.t > 1.0f) return false;
      p.mess.y = p.mess.start_y+((p.mess.end_y-p.mess.start_y)*p.mess.t);
      p.mess.x = p.mess.start_x+((p.mess.end_x-p.mess.start_x)*p.mess.t);
      return true;
      break;
    case P_TYPE_DEFEND:
      p.defend.t += d; if(p.defend.t > 1.0f) return false;
      p.defend.w = p.defend.start_w+((p.defend.end_w-p.defend.start_w)*p.defend.t);
      p.defend.h = p.defend.start_h+((p.defend.end_h-p.defend.start_h)*p.defend.t);
      return true;
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
  switch(p.type)
  {
    case P_TYPE_MESSENGER:
      tmp.x = p.mess.x;
      tmp.y = p.mess.y;
      tmp.w = p.mess.w;
      tmp.h = p.mess.h;
      g->draw(Sprite::messenger(),tmp);
      break;
    case P_TYPE_DEFEND:
      tmp.x = p.defend.x-p.defend.w/2;
      tmp.y = p.defend.y-p.defend.h/2;
      tmp.w = p.defend.w;
      tmp.h = p.defend.h;
      g->draw(Sprite::circle(),tmp);
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

