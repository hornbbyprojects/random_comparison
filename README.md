# Random number generation performance comparison
We need a large amount of random data - how can we get it?
## Compares the following methods:
stdlib.h rand()
Implementation of Xoroshiro256

We find Xoroshiro is much faster, so then we try to see how it can be further sped up with batching and multithreading. 
## Results on my laptop:
base rand() result: 1568962669
base rand() took 0 ms to seed, 7309 ms to calculate
Xoroshiro result: 1568949153
Xoroshiro took 0 ms to seed, 2322 ms to calculate
Xoroshiro (batched) result: 1556690312
Xoroshiro (batched) took 0 ms to seed, 620 ms to calculate
Xoroshiro (batched, with shifts before masking) result: 1556762923
Xoroshiro (batched, with shifts before masking) took 0 ms to seed, 617 ms to calculate
Xoroshiro (batched and multithreaded) result: 1556742886
Xoroshiro (batched and multithreaded) took 0 ms to seed, 75 ms to calculate

A ten times speedup!


## Usage
./compile.sh to compile with g++
./harness to run


