#ifndef __NONCOPYABLE__
#define __NONCOPYABLE__

class noncopyable
{
 public:
  noncopyable(const noncopyable&) = delete;
  void operator=(const noncopyable&) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

#endif  // __NONCOPYABLE__
