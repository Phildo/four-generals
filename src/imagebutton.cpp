ImageButton::ImageButton() { }
ImageButton::ImageButton(SDL_Rect s, SDL_Rect r)                 : sprite(s), rect(r) { }
ImageButton::ImageButton(SDL_Rect s, int x, int y, int w, int h) : sprite(s), rect(x,y,w,h) { }

bool ImageButton::query(const In &in)
{
  return (in.x > rect.x && in.x < rect.x+rect.w &&
          in.y > rect.y && in.y < rect.y+rect.h);
}

void ImageButton::draw(Graphics *g)
{
  rect.draw(g);
  g->draw(sprite, rect.rect);
}

void ImageButton::drawInMask(Graphics *g, SDL_Rect m)
{
  rect.drawInMask(g,m);
  g->drawInMask(sprite, rect.rect,m);
}


