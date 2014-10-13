#ifndef _FG_STRING_H_
#define _FG_STRING_H_

#define FG_STRING_LOCAL_LENGTH 32
class String
{
  private:
    char local[FG_STRING_LOCAL_LENGTH];
    const char *handle;
    int length;
  public:
    String(); //<- dumb

    //copy
    String(const String &other);
    String &operator=(const String &other);
    //move
    String(String &&other);

    String(const char* s, int l);
    ~String();
    int len() const;
    const char *ptr() const;
};

#endif

