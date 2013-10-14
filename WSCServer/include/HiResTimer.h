#ifndef HI_RES_TIMER_H
#define HI_RES_TIMER_H
#include <windows.h>

namespace HRTimer
{
  class HiResTimer
  {
  protected:
    __int64 a, b, f;
     
  public:
    HiResTimer();
    __int64 ElapsedTicks();
    __int64 ElapsedMicroseconds();
    __int64 Frequency();
    void Start();
    __int64 Stop();
  };
};

#endif