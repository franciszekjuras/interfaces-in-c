#ifndef I_HASH_H
#define I_HASH_H
#include <stdint.h>

static inline uint64_t i__hash(const char *name)
{
	const uint64_t fnv_prime = 0x00000100000001b3;
	uint64_t hash = 0xcbf29ce484222325;

	for (size_t i = 0; i < strlen(name); ++i)
	{
		hash ^= name[i];
		hash *= fnv_prime;
	}
	return hash;
}

#endif
