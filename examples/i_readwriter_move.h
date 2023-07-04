#ifndef I_READWRITER_MOVE_H
#define I_READWRITER_MOVE_H

#include <stdint.h>
#include <string.h>
#include "i_readwriter.h"

static inline i_readwriter _i_readwriter_pass_and_clear(i_readwriter pass, void *clear, size_t clear_size)
{
    memset(clear, 0, clear_size);
    return pass;
}

#define i_readwriter_move(ITF) (_i_readwriter_pass_and_clear(i_readwriter_from(ITF), &(ITF), sizeof(ITF)))

#endif
