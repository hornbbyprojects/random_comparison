#include "random.c"
#include "random_batch.cpp"
#include "random_batch_shifts.cpp"
#include "random_batch_multithreaded.cpp"
#include "random_xoroshiro.cpp"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <sys/time.h>

long millis_between(struct timeval *start, struct timeval *finish) {
  return (finish->tv_sec - start->tv_sec) * 1000 +
         (finish->tv_usec - start->tv_usec) / 1000;
}
void info_for(const char *name, void seed_func(), int result_func()) {
  struct timeval time_start, time_seeded, time_complete;
  gettimeofday(&time_start, NULL);
  seed();
  gettimeofday(&time_seeded, NULL);
  int summed = result_func();
  printf("%s result: %i\n", name, summed);
  gettimeofday(&time_complete, NULL);
  printf("%s took %lu ms to seed, %lu ms to calculate\n", name,
         millis_between(&time_start, &time_seeded),
         millis_between(&time_seeded, &time_complete));
}

int main(int argc, char *argv[]) {
  info_for("base rand()", srand_with_urandom, rand_sequence);
  info_for("Xoroshiro", seed, xoroshiro_sequence);
  info_for("Xoroshiro (batched)", seed, xoroshiro_batch_sequence);
  info_for("Xoroshiro (batched, with shifts before masking)", seed, xoroshiro_batch_shifts_sequence);
  info_for("Xoroshiro (batched and multithreaded)", seed, xoroshiro_batch_multithreaded_sequence);
}
