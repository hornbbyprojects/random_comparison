
#include "xoroshiro.h"
#include <cstdint>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
void one_thread(int count, uint64_t *state, std::mutex *the_mutex, int *result) {
  int summed = 0;
  int batch_count = count / 8;
  int remainder = count % 8;
  for (int i = 0; i < batch_count; i++) {
    std::uint64_t eight_bytes = xoshiro256(state);
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
    std::uint64_t eight_bytes = xoshiro256(state);
    for (int i = 0; i < remainder; i++) {
      summed += (eight_bytes & 0xff) % 7;
      eight_bytes >>= 8;
    }
  }
  const std::lock_guard<std::mutex> the_guard(*the_mutex);
  *result += summed;
}
int xoroshiro_batch_multithreaded_sequence() {
  int result;
  int count = 1000000 * 523;
  int thread_count = 8;
  int per_thread = count / thread_count;
  int remainder = count % thread_count;
  std::mutex the_mutex;
  std::vector<std::thread> to_join;
  for (int i = 0; i < thread_count; i++) {
    uint64_t *thread_state = new uint64_t[4];
    std::memcpy(thread_state, global_state, sizeof(global_state));
    xoroshiro_jump();
    std::thread the_thread(one_thread, per_thread, thread_state, &the_mutex, &result);
    to_join.push_back(std::move(the_thread));
  }
  {
    std::thread the_thread(one_thread, remainder, global_state, &the_mutex, &result);
    to_join.push_back(std::move(the_thread));
  }
  for (std::thread &t : to_join) {
    t.join();
  }
  return result;
}
