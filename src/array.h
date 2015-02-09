/*
Array - simply a class wrapper for static array. Allows pass by value.
ultra-naive implementation. use at own idiocy.
*/

#ifndef _ARRAY_
#define _ARRAY_

template <typename T, unsigned int N>
class Array
{
  private:
    T content[N];
  public:
    Array()  {}
    ~Array() {};

    T& operator[](int index)             { return content[index]; };
    const T& operator[](int index) const { return content[index]; };
};

#endif

