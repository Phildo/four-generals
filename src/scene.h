#ifndef _FG_SCENE_H_
#define _FG_SCENE_H_

struct In;

class Scene
{
  private:
  public:
    virtual ~Scene() = 0;
    virtual void enter() = 0;
    virtual void touch(In &in) = 0;
    virtual int tick() = 0;
    virtual void draw() = 0;
    virtual void leave() = 0;
    virtual void pass() = 0;
    virtual void pop() = 0;
};

#endif

