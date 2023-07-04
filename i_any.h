#ifndef I_ANY_H
#define I_ANY_H

#include <stdint.h>
#include "iic.h"

typedef struct i_any
{
    void *o;
    uint64_t _typeid;
} i_any;

#define i_any_new(OBJ, TYPE) ((i_any){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE)})

#define i_any_new_ns(OBJ, TYPE, NAMESPACE) ((i_any){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE)})

#define i_any_from(ITF) ((i_any){.o = (ITF).o, ._typeid = (ITF)._typeid})

#define i_any_none() ((i_any){.o = 0, ._typeid = 0})

#endif
