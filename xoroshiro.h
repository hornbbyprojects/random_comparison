

#ifndef __XOROSHIRO_H
#define __XOROSHIRO_H

#include <cstdint>
extern uint64_t global_state[4];
uint64_t xoshiro256(uint64_t state[4] = global_state);
void xoroshiro_jump(uint64_t state[4] = global_state);
void seed();

#endif
