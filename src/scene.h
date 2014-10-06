#ifndef _SCENE_H_
#define _SCENE_H_

struct In;

class Scene
{
  private:
  public:
    virtual ~Scene() = 0;
    virtual void touch(In &in) = 0;
    virtual void tick() = 0;
    virtual void draw() = 0;
};

#endif

