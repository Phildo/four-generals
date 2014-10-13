#include "string.h"

String::String(const char* s, int l)
{
  length = l;
  if(l <= FG_STRING_OBJECT_LENGTH) handle = &object[0];
  else                             handle = new char[l];

  for(int i = 0; i < l; i++)
    handle[i] = s[i];
}

String::~String()
{
  if(handle != &object[0])
  delete[] handle;
}

int String::len()
{
  return length;
}

char *String::ptr()
{
  return handle;
}

