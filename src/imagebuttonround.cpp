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
  g->draw(sprite, rect.rect);
  rect.draw(g);
}

