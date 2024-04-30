/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "xoroshiro.h"

/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
   generators. It has excellent (sub-ns) speed, a state (256 bits) that is
   large enough for any parallel application, and it passes all tests we
   are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

uint64_t global_state[4];
static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}


uint64_t xoshiro256(uint64_t state[4]) {
	const uint64_t result = rotl(state[1] * 5, 7) * 9;

	const uint64_t t = state[1] << 17;

	state[2] ^= state[0];
	state[3] ^= state[1];
	state[1] ^= state[2];
	state[0] ^= state[3];

	state[2] ^= t;

	state[3] = rotl(state[3], 45);

	return result;
}


/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to xoshiro256(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */

void xoroshiro_jump(uint64_t state[4]) {
        static const uint64_t JUMP[] = { 0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(unsigned i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (JUMP[i] & UINT64_C(1) << b) {
				s0 ^= state[0];
				s1 ^= state[1];
				s2 ^= state[2];
				s3 ^= state[3];
			}
			xoshiro256();	
		}
		
	state[0] = s0;
	state[1] = s1;
	state[2] = s2;
	state[3] = s3;
}

/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to xoshiro256(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */

void long_jump(uint64_t state[4]) {
        static const uint64_t LONG_JUMP[] = { 0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241, 0x39109bb02acbe635 };

	uint64_t s0 = 0;
	uint64_t s1 = 0;
	uint64_t s2 = 0;
	uint64_t s3 = 0;
	for(unsigned i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
		for(int b = 0; b < 64; b++) {
			if (LONG_JUMP[i] & UINT64_C(1) << b) {
				s0 ^= state[0];
				s1 ^= state[1];
				s2 ^= state[2];
				s3 ^= state[3];
			}
			xoshiro256();	
		}
		
	state[0] = s0;
	state[1] = s1;
	state[2] = s2;
	state[3] = s3;
}

void seed() {
  FILE *file = fopen("/dev/urandom", "r");
  if(!file) {
    printf("ERROR: failed to open /dev/urandom");
    exit(-2);
  }
  size_t num_read = fread(global_state, sizeof(global_state), 1, file);
  if(num_read != 1) {
    printf("ERROR: failed to read seed data from /dev/urandom");
    exit(-2);
  }
}
