#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "xoroshiro.h"
#include <sys/time.h>

int xoroshiro_sequence() {
  int summed = 0;
  for (int i = 0; i < 1000000 * 523; i++) {
    summed += (xoshiro256() % 7);
  }
  return summed;
}
