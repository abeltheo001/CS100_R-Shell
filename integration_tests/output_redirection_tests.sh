#!/bin/bash
# Assignment 4 tests ">", ">>"

OUTPUT_echoSingle=$(echo a > a.txt)
RSHELLOUTPUT_echoSingle=$(./rshell echo a > b.txt)

if [ "$OUTPUT_echoSingle" = "$RSHELLOUTPUT_echoSingle" ]
then
	echo "rshell works as expected on echo a > input.txt"
else
	echo "rshell does not work as expected on echo a > input.txt"
fi


OUTPUT_echoDouble=$(echo a >> input.txt)
RSHELLOUTPUT_echoDouble=$(./rshell echo a >> input.txt)

if [ "$OUTPUT_head" = "$RSHELLOUTPUT_head" ]
then
	echo "rshell works as expected on echo a >> input.txt"
else
	echo "rshell does not work as expected on echo a >> input.txt"
fi


OUTPUT_ls=$(ls > input.txt)
RSHELLOUTPUT_ls=$(./rshell ls > input.txt)

if [ "$OUTPUT_ls" = "$RSHELLOUTPUT_ls" ]
then
	echo "rshell works as expected on ls > input.txt"
else
	echo "rshell does not work as expected on ls > input.txt"

