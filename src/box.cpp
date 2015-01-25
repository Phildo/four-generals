Box::Box() { }
Box::Box(SDL_Rect r)
{
  rect = r;
}
Box::Box(int x, int y, int w, int h)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
}

void Box::draw(Graphics *g)
{
  SDL_Rect tmp;

  tmp.w = 16;
  tmp.h = 16;
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = rect.w;
  g->draw(Sprite::border_top,tmp);

  tmp.w = 16;
  tmp.h = 16;
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  tmp.w = rect.w;
  g->draw(Sprite::border_bottom,tmp);

  tmp.w = 16;
  tmp.h = 16;
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_left,tmp);

  tmp.w = 16;
  tmp.h = 16;
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_right,tmp);
}

