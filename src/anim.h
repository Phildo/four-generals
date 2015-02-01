struct Anim
{
  Sprite::AnimSprites sprite;
  int count;
  float frame_len;
  Box rect;

  int i;
  float f;

  void draw(Graphics *g);
  void drawInMask(Graphics *g, SDL_Rect m);
  void tick(float d);
  Anim();
  Anim(Sprite::AnimSprites s, int c, float f, SDL_Rect r);
  Anim(Sprite::AnimSprites s, int c, float f, int x, int y, int w, int h);
  SDL_Rect curSprite();
};

