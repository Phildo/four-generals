#include "tween.h"

float Tween::v()
{
  return s+(e-s)*t;
}

float Tween::v(float _t)
{
  if(_t > 1.0f) _t = 1.0f; else if(_t < 0.0f) _t = 0.0f; //clamp
  return s+(e-s)*_t;
}

void Tween::set(float _s, float _e)
{
  s = _s;
  e = _e;
}

void Tween::set(float _s, float _e, float _t)
{
  s = _s;
  e = _e;
  t = _t;
}

void Tween::tick(float d)
{
  t += d;
  if(t > 1.0f) t = 1.0f;
}

void Tween::reset()
{
  t = 0.f;
}

