#!/bin/bash

for filepath in "$@"
do
	filename="${filepath##*/}"
	extension="${filename#*.}"	
	name="${filename%%.*}"
	outpath="i_${name}_move.h"

	if [ -z "$name" ] || [ "$extension" != "iic.c" ]
	then
		continue
	fi

	readarray -t prototypes < $filepath || continue

	echo "'$filepath'" ">" "'$outpath'"

	cat << EOF > "$outpath"
#ifndef I_${name^^}_MOVE_H
#define I_${name^^}_MOVE_H

#include <stdint.h>
#include <string.h>
#include "i_$name.h"

static inline i_$name _i_${name}_pass_and_clear(i_$name pass, void *clear, size_t clear_size)
{
    memset(clear, 0, clear_size);
    return pass;
}

#define i_${name}_move(ITF) (_i_${name}_pass_and_clear(i_${name}_from(ITF), &(ITF), sizeof(ITF)))

#endif
EOF

done
