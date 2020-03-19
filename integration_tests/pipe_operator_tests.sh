#!/bin/bash
# Assignment 4 tests "|"

OUTPUT_echoSingle=$(echo a | tr a-z A-Z)
RSHELLOUTPUT_echoSingle=$(./rshell echo a | tr a-z A-Z)

if [ "$OUTPUT_echoSingle" = "$RSHELLOUTPUT_echoSingle" ]
then
	echo "rshell works as expected on echo a | tr a-z A-Z"
else
	echo "rshell does not work as expected on echo a | tr a-z A-Z"
fi


OUTPUT_catnA=$(cat input.txt | tr a-z A-Z)
RSHELLOUTPUT_catnA=$(./rshell cat input.txt | tr a-z A-Z)

if [ "$OUTPUT_echoSingle" = "$RSHELLOUTPUT_echoSingle" ]
then
	echo "rshell works as expected on cat input.txt | tr a-z A-Z"
else
	echo "rshell does not work as expected on cat input.txt | tr a-z A-Z"
fi

OUTPUT_catnA=$(cat < input.txt | tr a-z A-Z)
RSHELLOUTPUT_catnA=$(./rshell cat < input.txt | tr a-z A-Z)

if [ "$OUTPUT_echoSingle" = "$RSHELLOUTPUT_echoSingle" ]
then
	echo "rshell works as expected on cat < input.txt | tr a-z A-Z"
else
	echo "rshell does not work as expected on cat < input.txt | tr a-z A-Z"
fi

OUTPUT_long=$(cat < input.txt | tr A-Z a-z | tee newInput.txt | tr a-z A-Z > newInput.txt)
RSHELLOUTPUT_long=$(./rshell cat < input.txt | tr A-Z a-z | tee newInput.txt | tr a-z A-Z > newInput.txt)

if ["$OUTPUT_long" = "$RSHELLOUTPUT_long"]
then
	echo "rshell works as expected on cat < input.txt | tr A-Z a-z | tee newInput.txt | tr a-z A-Z > newInput.txt"
else
	echo "rshell does not work as expected on cat < input.txt | tr A-Z a-z | tee newInput.txt | tr a-z A-Z > newInput.txt"
fi

