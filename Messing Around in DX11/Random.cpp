#include "pch.h"
#include "Random.h"

// BASIC IMPL
namespace Random
{
/* The state array must be initialized to not be all zero */
uint32_t xorshift128(struct xorshift128_state* state)
{
	/* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
	uint32_t t = state->d;

	uint32_t const s = state->a;
	state->d = state->c;
	state->c = state->b;
	state->b = s;

	t ^= t << 11;
	t ^= t >> 8;
	return state->a = t ^ s ^ (s >> 19);
}


// XOSHIRO 256
/*  Adapted from the code included on Sebastian Vigna's website */
uint64_t xoshiro256ss(struct xoshiro256ss_state* state)
{
	uint64_t* s = state->s;
	uint64_t const result = rol64(s[1] * 5, 7) * 9;
	uint64_t const t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;
	s[3] = rol64(s[3], 45);

	return result;
}


// Seeding Stuff
#pragma region Seeding
uint64_t splitmix64(struct splitmix64_state* state)
{
	uint64_t result = state->s;

	state->s = result + 0x9E3779B97f4A7C15;
	result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
	result = (result ^ (result >> 27)) * 0x94D049BB133111EB;
	return result ^ (result >> 31);
}

// as an example; one could do this same thing for any of the other generators
struct xorshift128_state xorshift128_init(uint64_t seed)
{
	struct splitmix64_state smstate = { seed };
	struct xorshift128_state result = { 0 };

	uint64_t tmp = splitmix64(&smstate);
	result.a = (uint32_t)tmp;
	result.b = (uint32_t)(tmp >> 32);

	tmp = splitmix64(&smstate);
	result.c = (uint32_t)tmp;
	result.d = (uint32_t)(tmp >> 32);

	return result;
}
#pragma endregion
}