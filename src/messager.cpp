#include "messager.h"
#include "graphics.h"
#include "input.h"

Messager::Messager() : box(100,100,100,100)
{
}

Messager::~Messager()
{
}

void Messager::tick()
{

}

void Messager::draw(Graphics *g)
{
  box.draw(g);
}

void Messager::clearViewState()
{

}

void Messager::setMessage(Action m)
{
  message = m;
}

void Messager::reset()
{

}

