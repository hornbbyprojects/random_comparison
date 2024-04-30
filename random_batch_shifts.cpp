#include "xoroshiro.h"
#include <cstdint>
int xoroshiro_batch_shifts_sequence() {
  int summed = 0;
  int count = 1000000 * 523;
  int batch_count = count / 8;
  int remainder = count % 8;
  for (int i = 0; i < batch_count; i++) {
    std::uint64_t eight_bytes = xoshiro256();
    summed += (eight_bytes & 0xff) % 7;
    summed += ((eight_bytes >> 8) & 0xff) % 7;
    summed += ((eight_bytes >> 16) & 0xff) % 7;
    summed += ((eight_bytes >> 24) & 0xff) % 7;
    summed += ((eight_bytes >> 32) & 0xff) % 7;
    summed += ((eight_bytes >> 40) & 0xff) % 7;
    summed += ((eight_bytes >> 48) & 0xff) % 7;
    summed += ((eight_bytes >> 56) & 0xff) % 7;
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
