#include "ui.h"

/*
Button::Button(SDL_Rect r, int own)
{
  rect = r;
  ower = own;
}

bool Button::query(In &in)
{
  return false;
}

UI::UI()
{
  ownerCount = 0;
  buttonCount = 0;
}

int UI::getOwnerId()
{
  return ownerCount++;
}

int UI::registerButton(int owner, SDL_Rect r); //returns i
{
  buttons.add(Button(r, owner, buttonCount));
  return buttonCount++;
}

void UI::unregisterButton(int id)
{
  for(int i = 0; i < buttons.length(); i++)
    if(buttons[i].id == id) buttons.remove(i--);
}

void UI::unregisterOwner(int owner)
{
  for(int i = 0; i < buttons.length(); i++)
    if(buttons[i].owner == owner) buttons.remove(i--);
}

const &vArray<int> UI::queryTouch(const In &in); //returns reference to queryResults. not thread safe
{
  queryResults.removeAll();
  for(int i = 0; i < buttons.length(); i++)
    if(buttons[i].query(in)) queryResults.add(buttons[i].id);
  return queryResults;
}

const &vArray<int> UI::queryOwnedTouch(const In &in, int owner); //returns reference to queryResults. not thread safe
{
  queryResults.removeAll();
  for(int i = 0; i < buttons.length(); i++)
    if(buttons[i].owner == owner && buttons[i].query(in)) queryResults.add(buttons[i].id);
  return queryResults;
}

UI::~UI()
{
}
*/

