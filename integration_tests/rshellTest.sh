#!/bin/sh

OUTPUT=$(ls)
RSHELLOUTPUT=$(./rshell ls)

if [ "$OUTPUT" == "$RSHELLOUTPUT" ]
then
	echo "rshell works as expected on ls."
else
	echo "rshell does not work as expected on ls."
fi

OUTPUTconnector=$(echo a || echo b && echo c)
RSHELLOUTPUTconnector=$(./rshell echo a || echo b && echo c)

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on connectors."
else
	echo "rshell does not work as expected on connectors."
fi
