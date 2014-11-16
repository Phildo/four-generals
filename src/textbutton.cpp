TextButton::TextButton() { }
TextButton::TextButton(SDL_Rect r)                                       : box(r)                         {}
TextButton::TextButton(int x, int y, int w, int h)                       : box(x,y,w,h)                   {}
TextButton::TextButton(SDL_Rect r, Label l)                              : box(r),       label(l)         {}
TextButton::TextButton(int x, int y, int w, int h, Label l)              : box(x,y,w,h), label(l)         {}
TextButton::TextButton(SDL_Rect r, const char *t)                        : box(r),       label(r,t)       {}
TextButton::TextButton(int x, int y, int w, int h, const char *t)        : box(x,y,w,h), label(x,y,h,t)   {}
TextButton::TextButton(SDL_Rect r, const char *t, int l)                 : box(r),       label(r,t,l)     {}
TextButton::TextButton(int x, int y, int w, int h, const char *t, int l) : box(x,y,w,h), label(x,y,h,t,l) {}

bool TextButton::query(const In &in)
{
  return (in.x > box.rect.x && in.x < box.rect.x+box.rect.w &&
          in.y > box.rect.y && in.y < box.rect.y+box.rect.h);
}

void TextButton::draw(Graphics *g)
{
  box.draw(g);
  label.draw(g);
}
