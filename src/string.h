#ifndef _FG_STRING_H_
#define _FG_STRING_H_

#define FG_STRING_OBJECT_LENGTH 32
class String
{
  private:
    char object[FG_STRING_OBJECT_LENGTH];
    char *handle;
    int length;
  public:
    String(const char* s, int l);
    ~String();
    int len();
    char *ptr();
};

#endif

