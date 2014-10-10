Label::Label() { }
Label::Label(SDL_Rect r, char *s, int l)
{
  rect = r;
  rect.w = n_w*((float)r.h/(float)n_h);
  string = s;
  length = l;
}
Label::Label(int x, int y, int h, char *s, int l)
{
  rect.x = x;
  rect.y = y;
  rect.w = n_w*((float)h/(float)n_h);
  rect.h = h;
  string = s;
  length = l;
}

void Label::draw(Graphics *g)
{
  SDL_Rect tmp = rect;
  for(int i = 0; i < length; i++)
  {
    g->draw(Sprite::alpha(*(string+i)),tmp);
    tmp.x += tmp.w;
  }
}

