#include <exception>
#include "HiResTimer.h"

namespace HRTimer
{
  //----< constructor may throw exception >--------------------------

  HiResTimer::HiResTimer()
  {
    a = b = 0UL;
    if ( QueryPerformanceFrequency((LARGE_INTEGER*)&f) == 0) 
      throw std::exception("no high resolution counter on this platform");
  }
  //----< return number of platform's hi-res counter ticks >---------

  __int64 HiResTimer::ElapsedTicks()
  {
    return (b-a);
  }
  //----< return elapsed time in microseconds >----------------------

  __int64 HiResTimer::ElapsedMicroseconds()
  {
    // pick more precise time calculation

    __int64 d = (b-a); 
    if (d < 0x10c6f7a0b5edUL) // 2^64 / 1e6
      return (d*1000000UL)/f; 
    else
      return (d/f)*1000000UL;
  }
  //----< return frequence of hi-res counter >-----------------------

  __int64 HiResTimer::Frequency()
  {
    return f;
  }
//
  //----< start timer counting >-------------------------------------

  void HiResTimer::Start()
  {
    ::Sleep(0);
    QueryPerformanceCounter((LARGE_INTEGER*)&a);
  }
  //----< stop timer counting >--------------------------------------

  __int64 HiResTimer::Stop()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&b);
    return ElapsedTicks();
  }
}
//----< test stub >--------------------------------------------------
