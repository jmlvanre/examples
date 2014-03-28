#pragma once

#include <time.h>

#include <no_copy_semantics.h>

/* A tool for benchmarking wall-clock and cpu time to nanosecond accuracy.
   Construct a new instance as either RealTime (wall-clock, the default) or CPU (active cpu time).
   The timer starts running upon construction. If you want to start at a later point, use the Start() function.

   TTimer timer;
   // Do some stuff
   size_t sum = 0UL;
   for (size_t i = 0; i < 50; ++i) {
     sum += i;
   }
   timer.Stop();
   printf("Total Time [%f]\n", timer.Total());
   */
class TTimer {
  NO_COPY_SEMANTICS(TTimer);
  public:

  /* Constants used in calculating seconds elapsed. */
  static constexpr size_t NSPerSec = 1000000000L;
  static constexpr double DoubleNSPerSec = 1000000000.0;

  /* Used to specify whether this instance is measuring wall-clock or active cpu time. */
  enum Kind {
    RealTime,
    CPU
  };

  /* Start the timer upon construction. */
  inline TTimer(Kind kind = RealTime)
    : LapTime(0L),
      TotalTime(0L),
      Type(kind == RealTime ? CLOCK_REALTIME : CLOCK_THREAD_CPUTIME_ID) {
    clock_gettime(Type, &StartTime);
  }

  /* Start the timer. This over-rides any previous start value. */
  inline void Start() {
    clock_gettime(Type, &StartTime);
  }

  /* Stop the timer. Store the elapsed time since the last Start() (or construction). */
  inline void Stop() {
    timespec cur;
    clock_gettime(Type, &cur);
    LapTime = ((cur.tv_sec - StartTime.tv_sec) * NSPerSec) + (cur.tv_nsec - StartTime.tv_nsec);
    StartTime = cur;
    TotalTime += LapTime;
  }

  /* Return in seconds the amount of time elapsed since Start() (or construction) without stopping the timer. */
  inline double Elapsed() const {
    timespec cur;
    clock_gettime(Type, &cur);
    return (((cur.tv_sec - StartTime.tv_sec) * NSPerSec) + (cur.tv_nsec - StartTime.tv_nsec)) / DoubleNSPerSec;
  }

  /* Return in second the amount of time elapsed between the last Start() (or construction) and Stop() calls. */
  inline double Lap() const {
    return LapTime / DoubleNSPerSec;
  }

  /* Return in seconds the amount of time elapsed between all Start() (or construction) and Stop() calls since the last Reset(). */
  inline double Total() const {
    return TotalTime / DoubleNSPerSec;
  }

  /* Reset the Lap and Total timers. */
  inline void Reset() {
    LapTime = 0L;
    TotalTime = 0L;
  }

  private:

  /* Keeps track of seconds + nanoseconds of the Start() or construction call. */
  timespec StartTime;

  /* Keeps track of the nanoseconds elapsed between the last Start() (or construction) and Stop() calls. */
  long LapTime;

  /* Keeps track of the nanoseconds elapsed between all Start() (or construction) and Stop() calls since the last Reset(). */
  long TotalTime;

  /* A constant member used to specify whether this instance is keeping track of wall-clock or cpu time. */
  const clockid_t Type;

};
