#ifndef _FG_PARTICLES_H_
#define _FG_PARTICLES_H_

#include "array.h"
#include "tween.h"

class Graphics;

enum P_Type
{
  P_TYPE_MESSENGER,
  P_TYPE_DEFEND,
  P_TYPE_ATTACK,
  P_TYPE_COUNT
};

struct Particle
{
  Particle() {};
  P_Type type;
  union
  {
    struct
    {
      Tween x;
      Tween y;
      int w;
      int h;
      char begin_card;
      char end_card;
    } mess;
    struct
    {
      Tween w;
      Tween h;
      int x; //center
      int y; //center
    } defend;
    struct
    {
      Tween x;
      Tween y;
      Tween anim;
      int w;
      int h;
      char begin_card;
      char end_card;
    } attack;
    struct
    {
      Tween x;
      Tween y;
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
