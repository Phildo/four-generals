#ifndef _FG_LERP_H_
#define _FG_LERP_H_

class Lerp
{
  private:
  public:
    float s;
    float e;
    float t;

    Lerp() : s(0.f), e(0.f), t(0.f) {};
    Lerp(float _s, float _e): s(_s), e(_e), t(0.f) {};
    float v();
    float v(float _t);
    void set(float _s, float _e);
    void set(float _s, float _e, float _t);
    void tick(float d);
    void reset();

    //static funcs. helpers. whatever.
    static float clamp(float v);
    static float lerp(float s, float e, float t);
};

#endif

