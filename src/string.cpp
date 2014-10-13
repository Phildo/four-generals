#include "string.h"

String::String() : local("\0"), handle(0), length(0) { }

String::String(const String &other) //copy
{
  char *n;
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  n[length] = '\0';
  handle = n;
}
String &String::operator=(const String &other)
{
  char *n;
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = other.ptr()[i];
  n[length] = '\0';
  handle = n;
  return *this;
}

String::String(String &&other) //move
{
  length = other.len();
  if(length >= FG_STRING_LOCAL_LENGTH)
  {
    handle = other.ptr();
    other.handle = &other.local[0];
  }
  else
  {
    for(int i = 0; i < length; i++)
      local[i] = other.ptr()[i];
    local[length] = '\0';
    handle = &local[0];
  }
}
String::String(const char* s, int l)
{
  char *n;
  length = l;
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = s[i];
  n[length] = '\0';
  handle = n;
}
String::String(const char* s) //derive length from string
{
  char *n;
  for(length = 0; length < 1000 && s[length] != '\0'; length++) ; //find length, max out @ 1000
  if(length >= FG_STRING_LOCAL_LENGTH) n = new char[length+1];
  else                                 n = &local[0];

  for(int i = 0; i < length; i++)
    n[i] = s[i];
  n[length] = '\0';
  handle = n;
}

String::~String()
{
  if(handle != &local[0])
    delete[] handle;
}

int String::len() const
{
  return length;
}

const char *String::ptr() const
{
  return handle;
}

