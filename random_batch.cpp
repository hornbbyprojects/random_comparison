#include "xoroshiro.h"
#include <cstdint>
int xoroshiro_batch_sequence() {
  int summed = 0;
  int count = 1000000 * 523;
  int batch_count = count / 8;
  int remainder = count % 8;
  for (int i = 0; i < batch_count; i++) {
    std::uint64_t eight_bytes = xoshiro256();
    summed += (eight_bytes & 0xff) % 7;
    summed += ((eight_bytes & 0xff00) >> 8) % 7;
    summed += ((eight_bytes & 0xff0000) >> 16) % 7;
    summed += ((eight_bytes & 0xff000000) >> 24) % 7;
    summed += ((eight_bytes & 0xff00000000) >> 32) % 7;
    summed += ((eight_bytes & 0xff0000000000) >> 40) % 7;
    summed += ((eight_bytes & 0xff000000000000) >> 48) % 7;
    summed += ((eight_bytes & 0xff00000000000000) >> 56) % 7;
  }
  if (remainder > 0) {
    std::uint64_t eight_bytes = xoshiro256();
    for (int i = 0; i < remainder; i++) {
      summed += (eight_bytes & 0xff) % 7;
      eight_bytes >>= 8;
    }
  }
  return summed;
}
