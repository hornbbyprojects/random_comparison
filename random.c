#include "stdio.h"
#include "stdlib.h"
void srand_with_urandom() {
  FILE *file = fopen("/dev/urandom", "r");
  unsigned int seed;
  if(!file) {
    printf("ERROR: failed to open /dev/urandom");
    exit(-2);
  }
  size_t num_read = fread(&seed, sizeof(seed), 1, file);
  if(num_read != 1) {
    printf("ERROR: failed to read seed data from /dev/urandom");
    exit(-2);
  }
  srand(seed);
}
int rand_sequence() {
  int summed = 0;
  for(int i = 0; i < 1000000 * 523; i++) {
    summed += (rand() % 7);
  }
  return summed;
}
