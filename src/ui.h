#ifndef _UI_H_
#define _UI_H_

#include <SDL.h>
#include "var_array.h"

/*
struct Button
{
  int owner; //god I'm a terrible programmer
  int id; //this is actually a terrible system
  SDL_Rect rect;
  bool query(const In &in);
  Button(SDL_Rect r, int own, int id);
}

class UI
{
  private:
    int ownerCount;
    int buttonCount;
    vArray<Button> buttons;
    vArray<int> queryResults; //managed by queryTouch. Not thread safe.
  public:
    UI();
    ~UI();
    int getOwnerId();
    int registerButton(int owner, SDL_Rect r); //returns id
    void unregisterButton(int id);
    void unregisterOwner(int owner);
    const &vArray<int> queryTouch(const In &in); //returns reference to queryResults. not thread safe.
    const &vArray<int> queryOwnedTouch(const In &in, int owner); //returns reference to queryResults. not thread safe.
};
*/

#endif

