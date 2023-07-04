# !/bin/bash

codeblockstart="\`\`\`c"
codeblockend="\`\`\`"

for filepath in "$@"
do
	outfilepath="${filepath%%.c}"
	[ ! -z "$outfilepath" ] || continue
	outfilepath+=".md"
	readarray -t lines < $filepath || continue

	echo "$outfilepath"

	exec 3>"$outfilepath"
	no_text_yet=true
	processing_comment=false

	for line in "${lines[@]}"
	do
		if [ "$line" = "" ] ; then
			printf "\n" >&3
		elif [[ "$line" == *"/**" ]] ; then
			if [ "$no_text_yet" = false ]; then
				printf "%s\n" $codeblockend >&3
			fi
			processing_comment=true
		elif [[ "$line" == *" */" ]] ; then
			processing_comment=false
			printf "%s\n" "$codeblockstart" >&3
		else
			if [ "$no_text_yet" = true ] && [ "$processing_comment" = false ]; then
				printf "%s\n" "$codeblockstart" >&3
			fi
			no_text_yet=false
			printf "%s\n" "$line" >&3
		fi
	done
	if ! $processing_comment ; then
		printf "%s\n" $codeblockend >&3
	fi
done