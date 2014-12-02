#ifndef _FG_TWEEN_H_
#define _FG_TWEEN_H_

class Tween
{
  private:
  public:
    float s;
    float e;
    float t;

    Tween() : s(0.f), e(0.f), t(0.f) {};
    Tween(float _s, float _e): s(_s), e(_e), t(0.f) {};
    float v();
    float v(float _t);
    void set(float _s, float _e);
    void set(float _s, float _e, float _t);
    void tick(float d);
    void reset();
};

#endif

