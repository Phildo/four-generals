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
/*
  SDL_Rect tmp;

  tmp = Sprite::border_tl();
  tmp.x = rect.x;
  tmp.y = rect.y;
  g->draw(Sprite::border_tl(),tmp);

  tmp = Sprite::border_t();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.w = rect.w;
  g->draw(Sprite::border_t(), tmp);

  tmp = Sprite::border_tr();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  g->draw(Sprite::border_tr(),tmp);

  tmp = Sprite::border_r();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_r(), tmp);

  tmp = Sprite::border_br();
  tmp.x = rect.x+rect.w-tmp.w;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_br(),tmp);

  tmp = Sprite::border_b();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  tmp.w = rect.w;
  g->draw(Sprite::border_b(), tmp);

  tmp = Sprite::border_bl();
  tmp.x = rect.x;
  tmp.y = rect.y+rect.h-tmp.h;
  g->draw(Sprite::border_bl(),tmp);

  tmp = Sprite::border_l();
  tmp.x = rect.x;
  tmp.y = rect.y;
  tmp.h = rect.h;
  g->draw(Sprite::border_l(), tmp);
  */
}

