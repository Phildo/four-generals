Label::Label() { }
Label::Label(SDL_Rect r, const char *s, int l) : string(s, l)
{
  rect = r;
  rect.w = n_w*((float)r.h/(float)n_h);
}
Label::Label(SDL_Rect r, const char *s) : string(s)
{
  rect = r;
  rect.w = n_w*((float)r.h/(float)n_h);
}
Label::Label(int x, int y, int h, const char *s, int l) : string(s, l)
{
  rect.x = x;
  rect.y = y;
  rect.w = n_w*((float)h/(float)n_h);
  rect.h = h;
}
Label::Label(int x, int y, int h, const char *s) : string(s)
{
  rect.x = x;
  rect.y = y;
  rect.w = n_w*((float)h/(float)n_h);
  rect.h = h;
}

void Label::draw(Graphics *g)
{
  SDL_Rect tmp = rect;
  for(int i = 0; i < string.len(); i++)
  {
    g->draw(Sprite::alpha(*(string.ptr()+i)),tmp);
    tmp.x += tmp.w;
  }
}

