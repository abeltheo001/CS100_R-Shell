#!/bin/bash

RSHELLOUTPUTconnector=$(./rshell test -e CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -e CMakeLists.txt."
else
	echo "rshell does not work as expected on test -e CMakeLists.txt"
fi

RSHELLOUTPUTconnector=$(./rshell test -e blorganorgnorg)

if [ "(False)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -e blorganorgnorg."
else
	echo "rshell does not work as expected on test -e blorganorgnorg"
fi

RSHELLOUTPUTconnector=$(./rshell test -f CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -f CMakeLists.txt."
else
	echo "rshell does not work as expected on test -f CMakeLists.txt"
fi

RSHELLOUTPUTconnector=$(./rshell test -f header)

if [ "(False)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -f header."
else
	echo "rshell does not work as expected on test -f header"
fi

RSHELLOUTPUTconnector=$(./rshell test -d header)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -d header."
else
	echo "rshell does not work as expected on test -d header"
fi

RSHELLOUTPUTconnector=$(./rshell test -d CMakeLists.txt)

if [ "(False)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -d CMakeLists.txt."
else
	echo "rshell does not work as expected on test -d CMakeLists.txt."
fi