TextBox::TextBox() {}
TextBox::TextBox(SDL_Rect r)
{
  txt_len = 0;
  box = Box(r);
  label = Label(box.rect, txt, 0);
}
TextBox::TextBox(int x, int y, int w, int h)
{
  txt_len = 0;
  box.rect.x = x;
  box.rect.y = y;
  box.rect.w = w;
  box.rect.h = h;
  label = Label(box.rect, txt, 0);
}

void TextBox::setText(String t)
{
  txt_len = t.len();
  strcpy(&txt[0], t.ptr());
  label.text = String(txt, txt_len);
}

String TextBox::getText()
{
  return String(txt, txt_len);
}

void TextBox::input(char c)
{
  if(txt_len >= FG_MAX_TEXTBOX_LEN-1) return; //ignore input (save one for '\0')
  txt[txt_len] = c;
  txt_len++;
  label.text = String(txt, txt_len);
}

void TextBox::backspace()
{
  if(txt_len == 0) return;
  txt_len--;
  label.text = String(txt, txt_len);
}

void TextBox::clear()
{
  txt_len = 0;
  label.text = String(txt, txt_len);
}

void TextBox::draw(Graphics *g)
{
  box.draw(g);
  label.draw(g);
}

