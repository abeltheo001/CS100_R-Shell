#!/bin/bash

RSHELLOUTPUTconnector=$(./rshell test -e CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -e CMakeLists.txt."
else
	echo "rshell does not work as expected on test -e CMakeLists.txt"
fi

RSHELLOUTPUTconnector=$(./rshell test -f CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -f CMakeLists.txt."
else
	echo "rshell does not work as expected on test -f CMakeLists.txt"
fi