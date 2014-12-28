Image::Image() { }
Image::Image(SDL_Rect s, SDL_Rect r) : sprite(s), rect(r) {}
Image::Image(SDL_Rect s, int x, int y, int w, int h) : sprite(s), rect(x,y,w,h) { }
void Image::draw(Graphics *g)
{
  g->draw(sprite,rect.rect);
}

