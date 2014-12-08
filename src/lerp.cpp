#include "lerp.h"

float Lerp::v()
{
  return s+(e-s)*t;
}

float Lerp::v(float _t)
{
  if(_t > 1.0f) _t = 1.0f; else if(_t < 0.0f) _t = 0.0f; //clamp
  return s+(e-s)*_t;
}

void Lerp::set(float _s, float _e)
{
  s = _s;
  e = _e;
}

void Lerp::set(float _s, float _e, float _t)
{
  s = _s;
  e = _e;
  t = _t;
}

void Lerp::tick(float d)
{
  t += d;
  if(t > 1.0f) t = 1.0f;
}

void Lerp::reset()
{
  t = 0.f;
}

