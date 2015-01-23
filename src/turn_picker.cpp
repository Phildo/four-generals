#include "turn_picker.h"
#include "graphics.h"
#include "input.h"

TurnPicker::TurnPicker() : box(100,100,100,100)
{
}

TurnPicker::~TurnPicker()
{
}

bool TurnPicker::getTurn(Turn &t)
{
  return false;
}

void TurnPicker::reset()
{

}

void TurnPicker::touch(In &in)
{

}

void TurnPicker::tick()
{

}

void TurnPicker::draw(Graphics *g)
{
  box.draw(g);
}

