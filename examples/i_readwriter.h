#ifndef I_READWRITER_H
#define I_READWRITER_H

#include <stdint.h>
#include "iic.h"

typedef struct i_readwriter
{
    void *o;
    uint64_t _typeid;
    void (*write)(void *readwriter_o, const char *str);
    const char *(*read)(void *readwriter_o);
    void (*close)(void *readwriter_o);
} i_readwriter;

#define i_readwriter_new(OBJ, TYPE) ((i_readwriter){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE), .write = TYPE##_write, .read = TYPE##_read, .close = TYPE##_close})

#define i_readwriter_new_ns(OBJ, TYPE, NAMESPACE) ((i_readwriter){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE), .write = NAMESPACE##_write, .read = NAMESPACE##_read, .close = NAMESPACE##_close})

#define i_readwriter_from(ITF) ((i_readwriter){.o = (ITF).o, ._typeid = (ITF)._typeid, .write = (ITF).write, .read = (ITF).read, .close = (ITF).close})

#define i_readwriter_none() ((i_readwriter){.o = 0, ._typeid = 0, .write = 0, .read = 0, .close = 0})

#endif
