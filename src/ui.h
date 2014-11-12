#ifndef _FG_UI_H_
#define _FG_UI_H_

#include <SDL.h>
#include "string.h"

//convenient bro
// total (l)ength, (p)adding, (u)nit length, total (n)umber of members, (i)ndex
#define space(l,p,u,n,i) ((p+((l-(2*p))/n)*i)+(((l-(2*p))/n)/2)-u)

struct In;
class Graphics;

namespace UI
{
  #include "box.h"
  #include "label.h"
  #include "button.h"
  #include "keyboard.h"
  #include "textbox.h"
}

#endif

