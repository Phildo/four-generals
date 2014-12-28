struct AnimSprites //bs to allow default constructor copy
{
  SDL_Rect sprite[4];
  AnimSprites() {};
  AnimSprites(SDL_Rect s0, SDL_Rect s1, SDL_Rect s2, SDL_Rect s3)
  {
    sprite[0] = s0;
    sprite[1] = s1;
    sprite[2] = s2;
    sprite[3] = s3;
  }
};
struct Anim
{
  AnimSprites sprite;
  int count;
  float frame_len;
  Box rect;

  int i;
  float f;

  void draw(Graphics *g);
  void tick(float d);
  Anim();
  Anim(AnimSprites s, int c, float f, SDL_Rect r);
  Anim(AnimSprites s, int c, float f, int x, int y, int w, int h);
};

