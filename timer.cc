/* g++ -std=c++11 timer.cc -I. -lrt */

#include <timer.h>

#include <stdio.h>

int main() {
  TTimer timer;
  size_t sum = 0UL;
  for (size_t i = 0; i < 50; ++i) {
    sum += i;
  }
  timer.Stop();
  printf("Total Time [%f]\n", timer.Total());
}
