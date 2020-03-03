#!/bin/bash

# Assignment 2 tests

OUTPUT=$(ls)
RSHELLOUTPUT=$(./rshell ls)

if [ $OUTPUTconnector == $RSHELLOUTPUTconnector ]
then
	echo "rshell works as expected on ls."
else
	echo "rshell does not work as expected on ls."
fi

OUTPUTconnector=$(echo a || echo b && echo c)
RSHELLOUTPUTconnector=$(./rshell echo a || echo b && echo c)

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo a || echo b && echo c."
else
	echo "rshell does not work as expected on echo a || echo b && echo c."
fi

OUTPUT_ls_a=$(ls -a)
RSHELLOUTPUT_ls_a=$(./rshell "ls -a")

if [ "$OUTPUT_ls_a" == "$RSHELLOUTPUT_ls_a" ]
then
	echo "rshell works as expected on ls -a"
else
	echo "rshell does not work as expected on ls -a"
fi

OUTPUT_ls_j=$(ls -j)
RSHELLOUTPUT_ls_j=$(./rshell ls -j)

if [ "$OUTPUT_ls_j" == "$RSHELLOUTPUT_ls_j" ]
then
	echo "rshell works as expected on ls-j."
else
	echo "rshell does not work as expected on ls-j."
fi

OUTPUTecho=$(echo echo)
RSHELLOUTPUTecho=$(./rshell echo echo)

if [ "$OUTPUTecho" == "$RSHELLOUTPUTecho" ]
then
	echo "rshell works as expected on echo echo."
else
	echo "rshell does not work as expected on echo echo."
fi

OUTPUTconnector=$(echo A \#&& echo B )
RSHELLOUTPUTconnector=$(./rshell echo A \#&& echo B )

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A #&& echo B."
else
	echo "rshell does not work as expected on echo A #&& echo B."
fi

OUTPUTconnector=$(git status)
RSHELLOUTPUTconnector=$(./rshell git status)

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on git status."
else
	echo "rshell does not work as expected on git status."
fi

# Assignment 3 tests

RSHELLOUTPUTconnector=$(./rshell test -e CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -e CMakeLists.txt."
else
	echo "rshell does not work as expected on test -e CMakeLists.."
fi

OUTPUTconnector=$(echo A || exit)
RSHELLOUTPUTconnector=$(./rshell echo A || exit)

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo \"A || exit \" ."
else
	echo "rshell does not work as expected on echo \"A || exit \"."
fi

# OUTPUTconnector=$([ -e CMakeLists.txt ])
RSHELLOUTPUTconnector=$(./rshell [ -e CMakeLists.txt ])
if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [-e CMakeLists.txt]."
else
	echo "rshell does not work as expected on on [-e CMakeLists.txt]."
fi
