ImageButtonRound::ImageButtonRound() { }
ImageButtonRound::ImageButtonRound(SDL_Rect s, SDL_Rect r)                 : sprite(s), rect(r) { }
ImageButtonRound::ImageButtonRound(SDL_Rect s, int x, int y, int w, int h) : sprite(s), rect(x,y,w,h) { }

bool ImageButtonRound::query(const In &in)
{
  return (in.x > rect.x && in.x < rect.x+rect.w &&
          in.y > rect.y && in.y < rect.y+rect.h);
}

void ImageButtonRound::draw(Graphics *g)
{
  SDL_Rect r;
  r.x = rect.x;
  r.y = rect.y;
  r.w = Sprite::border_corner_round_tl.w;
  r.h = Sprite::border_corner_round_tl.h;
  g->draw(Sprite::border_corner_round_tl,r);
  r.x = rect.x+rect.w-Sprite::border_corner_round_tr.w;
  g->draw(Sprite::border_corner_round_tr,r);
  r.y = rect.y+rect.h-Sprite::border_corner_round_br.h;
  g->draw(Sprite::border_corner_round_br,r);
  r.x = rect.x;
  g->draw(Sprite::border_corner_round_bl,r);

  r.x = rect.x+Sprite::border_corner_round_tl.w;
  r.y = rect.y;
  r.w = rect.w-Sprite::border_corner_round_tl.w-Sprite::border_corner_round_tr.w;
  r.h = Sprite::border_top.h;
  g->draw(Sprite::border_top, r);
  r.y = rect.y+rect.h-Sprite::border_bottom.h;
  g->draw(Sprite::border_bottom, r);

  r.x = rect.x;
  r.y = rect.y+Sprite::border_corner_round_tl.h;
  r.w = Sprite::border_left.w;
  r.h = rect.h-Sprite::border_corner_round_tl.h-Sprite::border_corner_round_bl.h;
  g->draw(Sprite::border_left, r);
  r.x = rect.x+rect.w-Sprite::border_right.w;
  g->draw(Sprite::border_right, r);

  g->draw(sprite, rect.rect);
}

