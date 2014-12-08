#include "lerp.h"

float Lerp::v()
{
  return lerp(s, e, t);
}

float Lerp::v(float _t)
{
  t = clamp(_t);
  return v();
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
  t = clamp(_t);
}

void Lerp::tick(float d)
{
  t = clamp(t+d);
}

void Lerp::reset()
{
  t = 0.f;
}

float Lerp::clamp(float v)
{
  if(v > 1.0f) v = 1.0f;
  else if(v < 0.0f) v = 0.0f;
  return v;
}

float Lerp::lerp(float s, float e, float t)
{
  return s+(e-s)*t;
}

