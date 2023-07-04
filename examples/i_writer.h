#ifndef I_WRITER_H
#define I_WRITER_H

#include <stdint.h>
#include "iic.h"

typedef struct i_writer
{
    void *o;
    uint64_t _typeid;
    void (*write)(void *writer_o, const char *str);
    void (*close)(void *writer_o);
} i_writer;

#define i_writer_new(OBJ, TYPE) ((i_writer){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE), .write = TYPE##_write, .close = TYPE##_close})

#define i_writer_new_ns(OBJ, TYPE, NAMESPACE) ((i_writer){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE), .write = NAMESPACE##_write, .close = NAMESPACE##_close})

#define i_writer_from(ITF) ((i_writer){.o = (ITF).o, ._typeid = (ITF)._typeid, .write = (ITF).write, .close = (ITF).close})

#define i_writer_none() ((i_writer){.o = 0, ._typeid = 0, .write = 0, .close = 0})

#endif
