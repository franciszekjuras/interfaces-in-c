#!/bin/bash

trim() {
    local var="$*"
    # remove leading whitespace characters
    var="${var#"${var%%[![:space:]]*}"}"
    # remove trailing whitespace characters
    var="${var%"${var##*[![:space:]]}"}"
    printf '%s' "$var"
}

for filepath in "$@"
do
	filename="${filepath##*/}"
	extension="${filename#*.}"
	name="${filename%%.*}"

	if [ -z "$name" ] || [ "$extension" != "itf.c" ]
	then
		continue
	fi

	readarray -t prototypes < $filepath || continue

	echo $filepath

	tails=("$tail")
			heads+=("$head")
			funcs+=("$func")

	unset tails heads funcs
	for prototype in "${prototypes[@]}"
	do
		# skip empty lines
		if [ ! -z "$prototype" ]
		then
			# everything after function name
			tail="(${prototype##*(}"
			# return value and function name (trimmed)
			head="$(trim "${prototype%(*}")"
			# just function name (working with 'void *foo' case)
			func="${head##*[ \*]}"
			# just return value
			head="${head%"$func"}"
			#append to arrays
			tails+=("$tail")
			heads+=("$head")
			funcs+=("$func")
		fi
	done

	# prepare struct field holding function pointers
	unset struct_fields
	for i in "${!funcs[@]}"
	do
		struct_fields+="    ${heads[$i]}(*${funcs[$i]})${tails[$i]}
"
	done

	# prepare assignments for construction macro
	unset new_assignments from_assignments
	for func in "${funcs[@]}"
	do
		new_assignments+=", .$func = TYPE##_$func"
		from_assignments+=", .$func = ITF.$func"
	done

	# write struct definition and construction macro to a header file
	cat << EOF > "i_$name.h"
#ifndef I_${name^^}_H
#define I_${name^^}_H

#include <stdint.h>
#include "i__hash.h"

typedef struct i_$name
{
    void *o;
    uint64_t _typeid;
${struct_fields[*]}} i_$name;

#define i_${name}_new(OBJ, TYPE) ((i_$name){.o = _Generic(OBJ, TYPE *: OBJ), ._typeid = i__hash(#TYPE)${new_assignments}})

#define i_${name}_from(ITF) ((i_${name}){.o = ITF.o, ._typeid = ITF._typeid${from_assignments}})

#endif
EOF

done
