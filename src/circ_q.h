/*
circ_q - very simple circle queue
ultra-naive implementation. use at own idiocy.

some notes-
-if full, will overlap itself without warning. in fact, when this happens, it will think itself empty, so all data will be lost.
-pointers returned from queries are NOT thread safe. a returned pointer could be overwritten in the queue by external enqueues.
-enqueued objects might get shuffled.

*/

#ifndef _CIRC_Q_
#define _CIRC_Q_

template <typename T, unsigned int N>
class circQ
{
  private:
    T q[N];
    int front;
    int back;

    int inc(int i) { return (i+1)%N; }
    int dec(int i) { i--; if(i < 0) i = N-1; return i; }
  public:
    circQ() : front(0), back(0) {}
    ~circQ() {}

    T *next()
    {
      if(front == back) return 0;
      front = inc(front);
      return &q[dec(front)];
    }
    T *enqueue(T t)
    {
      q[back] = t;
      back = inc(back);
      return &q[dec(back)];
    }
    T *get(T t) //assumes operator==. moves found T to front of queue, then dequeues it
    {
      for(int i = front; i != back; i = inc(i))
      {
        if(q[i] == t)
        {
          T tmp = q[front];
          q[front] = q[i];
          q[i] = tmp;
          return next();
        }
      }
      return 0;
    }
    void empty() //easy!
    {
      front = 0;
      back = 0;
    }
};

#endif

