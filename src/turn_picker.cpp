#include "turn_picker.h"
#include "graphics.h"
#include "input.h"

TurnPicker::TurnPicker() : box(100,100,100,100)
{
}

TurnPicker::~TurnPicker()
{
}

void TurnPicker::touch(In &in)
{

}

bool TurnPicker::query(In &in)
{
  return false;
}

void TurnPicker::tick()
{

}

void TurnPicker::draw(Graphics *g)
{
  box.draw(g);
}

void TurnPicker::activate()
{

}

void TurnPicker::clearViewState()
{

}

bool TurnPicker::getTurn(Turn &t)
{
  return false;
}

void TurnPicker::reset()
{

}

