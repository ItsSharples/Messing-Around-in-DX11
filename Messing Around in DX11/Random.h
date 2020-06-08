#pragma once

#include <stdint.h>


namespace Random
{


struct xorshift32_state { uint32_t a; };
struct xorshift128_state { uint32_t a, b, c, d; };

struct xoshiro256ss_state { uint64_t s[4]; };

struct splitmix64_state { uint64_t s; };

static uint64_t rol64(uint64_t x, int k)
{
	return (x << k) | (x >> (64 - k));
}

uint32_t xorshift128(struct xorshift128_state* state);
uint64_t xoshiro256ss(struct xoshiro256ss_state* state);


// Seeding Functions
uint64_t splitmix64(struct splitmix64_state* state);
struct xorshift128_state xorshift128_init(uint64_t seed);


class xor128
{
public:
	xor128() = default;
	~xor128() = default;

	void seed(uint64_t seed) { state = xorshift128_init(seed); is_seeded = true; }
	// Returns if the generator has been seeded.
	[[nodiscard]] const bool isSeeded() { return is_seeded; }
	
	// Returns a value between 0 and UINT32_MAX  
	[[nodiscard]] uint32_t rand() { return xorshift128(&state); }

	// Returns a value between 0 and max
	[[nodiscard]] uint32_t rand(uint32_t max) { return rand(0, max); }

	// Returns a value between min and max
	[[nodiscard]] uint32_t rand(uint32_t min, uint32_t max) {return rand() % (max - min) + min; }
private:
	struct xorshift128_state state;
	bool is_seeded;
};


}//namespace Random