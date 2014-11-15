Label::Label() { }
Label::Label(SDL_Rect r, const char *t, int l) : text(t, l)
{
  rect = r;
  rect.w = n_w*((float)r.h/(float)n_h);
}
Label::Label(SDL_Rect r, const char *t) : text(t)
{
  rect = r;
  rect.w = n_w*((float)r.h/(float)n_h);
}
Label::Label(int x, int y, int h, const char *t, int l) : text(t, l)
{
  rect.x = x;
  rect.y = y;
  rect.w = n_w*((float)h/(float)n_h);
  rect.h = h;
}
Label::Label(int x, int y, int h, const char *t) : text(t)
{
  rect.x = x;
  rect.y = y;
  rect.w = n_w*((float)h/(float)n_h);
  rect.h = h;
}

void Label::draw(Graphics *g)
{
  SDL_Rect tmp = rect;
  for(int i = 0; i < text.len(); i++)
  {
    g->draw(Sprite::alpha(*(text.ptr()+i)),tmp);
    tmp.x += tmp.w;
  }
}

