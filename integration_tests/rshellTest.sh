#!/bin/bash

# Assignment 3 tests

RSHELLOUTPUTconnector=$(./rshell test -e CMakeLists.txt)

if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on test -e CMakeLists.txt."
else
	echo "rshell does not work as expected on test -e CMakeLists.txt"
fi

# OUTPUTconnector=$([ -e CMakeLists.txt ])
RSHELLOUTPUTconnector=$(./rshell [ -e CMakeLists.txt ])
if [ "(True)" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [-e CMakeLists.txt]."
else
	echo "rshell does not work as expected on on [-e CMakeLists.txt]."
fi

OUTPUTconnector=$(echo A || (echo B))
RSHELLOUTPUTconnector=$(./rshell echo A || (echo B))

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A || (echo B)"
else
	echo "rshell does not work as expected on echo A || (echo B)"
fi

OUTPUTconnector=$(echo A || (echo B || echo C))
RSHELLOUTPUTconnector=$(./rshell echo A || (echo B || echo C))

if [ "$OUTPUTconnector" == "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A || (echo B || echo C)"
else
	echo "rshell does not work as expected on echo A || (echo B || echo C)"
fi