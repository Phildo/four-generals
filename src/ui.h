#ifndef _FG_UI_H_
#define _FG_UI_H_

#include "string.h"
#include "circ_q.h"
#include "sprite.h"
#include "SDL.h"

//convenient bro
// total (l)ength, (p)adding, (u)nit length, total (n)umber of members, (i)ndex
//#define space(l,p,u,n,i) ((p+((l-(2.0*p))/n)*i)+(((l-(2.0*p))/n)/2.0)-(u/2.0))
#define space(l,p,u,n,i) ((p+(((float)l-(2.0*p))/(float)n)*i)+((((float)l-(2.0*p))/(float)n)/2.0)-(u/2.0))

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
}

#endif

