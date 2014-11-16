#ifndef _FG_PARTICLES_H_
#define _FG_PARTICLES_H_

#include "array.h"

class Graphics;

enum P_Type
{
  P_TYPE_MESSENGER,
  P_TYPE_COUNT
};

struct Particle
{
  P_Type type;
  union
  {
    struct
    {
      char begin_card;
      char end_card;
      float start_x;
      float start_y;
      float x;
      float y;
      float end_x;
      float end_y;
      float t;
    } mess;
    struct
    {
      float x;
      float y;
      float t;
    } part;
  };
};

class ParticleSys
{
  private:
    Array<Particle,256> particles;
  public:
    ParticleSys();
    ~ParticleSys();

    void registerP(Particle p);

    void tick(float d);
    void draw(Graphics *g);
};

#endif
