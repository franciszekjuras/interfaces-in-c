#ifndef IIC_H
#define IIC_H

#include <stdint.h>

static inline uint64_t iic_hash(const char *name)
{
	static const uint64_t fnv_prime = 0x00000100000001b3;
	uint64_t hash = 0xcbf29ce484222325;

	for (size_t i = 0; i < strlen(name); ++i)
	{
		hash ^= name[i];
		hash *= fnv_prime;
	}
	return hash;
}

#define iic_dyncast(TYPE, OBJ) (((OBJ)._typeid == iic_hash(#TYPE)) ? (TYPE *)(OBJ).o : (TYPE *)0)

#define iic_is_some(OBJ) ((OBJ)._typeid != 0)

#define iic_is_none(OBJ) ((OBJ)._typeid == 0)

#endif
