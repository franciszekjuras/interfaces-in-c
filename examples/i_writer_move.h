#ifndef I_WRITER_MOVE_H
#define I_WRITER_MOVE_H

#include <stdint.h>
#include <string.h>
#include "i_writer.h"

static inline i_writer _i_writer_pass_and_clear(i_writer pass, void *clear, size_t clear_size)
{
    memset(clear, 0, clear_size);
    return pass;
}

#define i_writer_move(ITF) (_i_writer_pass_and_clear(i_writer_from(ITF), &(ITF), sizeof(ITF)))

#endif
