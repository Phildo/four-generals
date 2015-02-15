#ifndef _FG_UI_H_
#define _FG_UI_H_

#include "string.h"
#include "circ_q.h"
#include "sprite.h"
#include "SDL.h"

//convenient bro
// total (l)ength, (p)adding, (u)nit length, total (n)umber of members, (i)ndex

// n = 3
// /----------------------l---------------------\
// |-----[------xxx------xxx-----xxx------]-----|
// \-p--/       \u/

#define space(l,p,u,n,i) ((((((float)l-(2.0f*p))/((float)n+1.0f))*((float)i+1.0f))-((float)u/2.0f))+p)

struct In;
class Graphics;

namespace UI
{
  #include "box.h"
  #include "image.h"
  #include "anim.h"
  #include "label.h"
  #include "button.h"
  #include "imagebutton.h"
  #include "imagebuttonround.h"
  #include "textbutton.h"
  #include "keyboard.h"
  #include "textbox.h"
  #include "list.h"
  #include "scroller.h"
}

#endif

