Anim::Anim() { }
Anim::Anim(Sprite::AnimSprites s, int c, float f, SDL_Rect r) : sprite(s), count(c), frame_len(f), rect(r), i(0), f(0.0f) {}
Anim::Anim(Sprite::AnimSprites s, int c, float f, int x, int y, int w, int h) : sprite(s), count(c), frame_len(f), rect(x,y,w,h), i(0), f(0.0f) { }
void Anim::draw(Graphics *g)
{
  g->draw(sprite.sprite[i],rect.rect);
}

void Anim::tick(float d)
{
  f += d;
  if(f >= frame_len)
  {
    f = 0.0f;
    i = (i+1)%count;
  }
}

