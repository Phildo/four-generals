#ifndef _FG_PARTICLES_H_
#define _FG_PARTICLES_H_

#include "darray.h"
#include "lerp.h"

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
  Particle() { for(int i = 0; i < 256; i++) data[i] = '\0'; };
  P_Type type;
  union
  {
    char data[256]; //for the sole purpose of easily zeroing
    struct
    {
      Lerp x;
      Lerp y;
      int w;
      int h;
      char begin_card;
      char end_card;
    } mess;
    struct
    {
      Lerp w;
      Lerp h;
      int x; //center
      int y; //center
    } defend;
    struct
    {
      Lerp x;
      Lerp y;
      Lerp anim;
      int w;
      int h;
      char begin_card;
      char end_card;
    } attack;
    struct
    {
      Lerp x;
      Lerp y;
    } part;
  };
};

class ParticleSys
{
  private:
    DArray<Particle,256> particles;
  public:
    ParticleSys();
    ~ParticleSys();

    void registerP(Particle p);

    void tick(float d);
    void draw(Graphics *g);
};

#endif

