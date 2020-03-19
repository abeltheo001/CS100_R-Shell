#!/bin/bash
# Assignment 4 tests "<"

OUTPUT=$(cat < input.txt)
RSHELLOUTPUT=$(./rshell cat < input.txt)

if [ "$OUTPUT" = "$RSHELLOUTPUT" ]
then
	echo "rshell works as expected on cat < input.txt"
else
	echo "rshell does not work as expected on cat < input.txt"
fi

OUTPUT_head=$(head -n 2 < input.txt)
RSHELLOUTPUT_head=$(./rshell head -n 2 < input.txt)

if [ "$OUTPUT_head" = "$RSHELLOUTPUT_head" ]
then
	echo "rshell works as expected on head -n 2 < input.txt"
else
	echo "rshell does not work as expected on head -n 2 < input.txt"
fi

OUTPUT_uniq=$(uniq < input.txt)
RSHELLOUTPUT_uniq=$(./rshell uniq < input.txt)

if [ "$OUTPUT_uniq" = "$RSHELLOUTPUT_uniq" ]
then
	echo "rshell works as expected on uniq < input.txt"
else
	echo "rshell does not work as expected on uniq < input.txt"
fi

