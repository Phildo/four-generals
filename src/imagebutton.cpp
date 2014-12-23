ImageButton::ImageButton() { }
ImageButton::ImageButton(SDL_Rect r, SDL_Rect s)                 : rect(r), sprite(s) {}
ImageButton::ImageButton(int x, int y, int w, int h, SDL_Rect s) : sprite(s)
{
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
}

bool ImageButton::query(const In &in)
{
  return (in.x > rect.x && in.x < rect.x+rect.w &&
          in.y > rect.y && in.y < rect.y+rect.h);
}


void ImageButton::draw(Graphics *g)
{
  g->draw(sprite, rect);
}

