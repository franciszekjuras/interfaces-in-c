#ifndef I_DYNCAST_H
#define I_DYNCAST_H
#include "i__hash.h"

#define i__dyncast(TYPE, OBJ) ((OBJ._typeid == i__hash(#TYPE)) ? (TYPE *)OBJ.o : (TYPE *)0)

#endif
