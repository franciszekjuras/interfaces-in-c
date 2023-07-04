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
	outpath="i_$name.h"

	if [ -z "$name" ] || [ "$extension" != "iic.c" ]
	then
		continue
	fi

	readarray -t prototypes < $filepath || continue

	echo "'$filepath'" ">" "'$outpath'"

	unset tails heads funcs
	for prototype in "${prototypes[@]}"
	do
		# skip empty lines
		if [ ! -z "$prototype" ]
		then
			# everything after function name
			tail="${prototype##*(}"
			tail="${tail%)*}"
			# return value and function name (trimmed)
			head="$(trim "${prototype%(*}")"
			# just function name (works with 'void *foo' case)
			func="${head##*[ \*]}"
			# just return value
			head="${head%"$func"}"
			# append to arrays
			tails+=("$tail")
			heads+=("$head")
			funcs+=("$func")
		fi
	done

	# prepare struct field holding function pointers
	unset struct_fields
	for i in "${!funcs[@]}"
	do
		tail="${tails[$i]}"
		if [ -z "$tail" ] || [ "$tail" == "void" ]; then
			tail=""
		else
			tail=", $tail"
		fi
		struct_fields+="    ${heads[$i]}(*${funcs[$i]})(void *${name}_o${tail});
"
	done

	# prepare assignments for construction macro
	unset new_assignments new_ns_assignments from_assignments none_assignments
	for func in "${funcs[@]}"
	do
		new_assignments+=", .$func = TYPE##_$func"
		new_ns_assignments+=", .$func = NAMESPACE##_$func"
		from_assignments+=", .$func = (ITF).$func"
		none_assignments+=", .$func = 0"
	done

	# write struct definition and construction macro to a header file
	cat << EOF > "$outpath"
#ifndef I_${name^^}_H
#define I_${name^^}_H

#include <stdint.h>
#include "iic.h"

typedef struct i_$name
{
    void *o;
    uint64_t _typeid;
${struct_fields[*]}} i_$name;

#define i_${name}_new(OBJ, TYPE) ((i_$name){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE)${new_assignments}})

#define i_${name}_new_ns(OBJ, TYPE, NAMESPACE) ((i_$name){.o = _Generic((OBJ), TYPE *: (OBJ)), ._typeid = iic_hash(#TYPE)${new_ns_assignments}})

#define i_${name}_from(ITF) ((i_${name}){.o = (ITF).o, ._typeid = (ITF)._typeid${from_assignments}})

#define i_${name}_none() ((i_${name}){.o = 0, ._typeid = 0${none_assignments}})

#endif
EOF

done
