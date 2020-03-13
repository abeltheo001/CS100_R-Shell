#!/bin/bash

RSHELLOUTPUTconnector=$(./rshell [ -e CMakeLists.txt ])

if [ "(True)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -e CMakeLists.txt ]"
else
	echo "rshell does not work as expected on [ -e CMakeLists.txt ]"
fi

RSHELLOUTPUTconnector=$(./rshell [ -e blorganorgnorg ])

if [ "(False)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -e blorganorgnorg ]"
else
	echo "rshell does not work as expected on [ -e blorganorgnorg ]"
fi

RSHELLOUTPUTconnector=$(./rshell [ -f CMakeLists.txt ])

if [ "(True)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -f CMakeLists.txt ]"
else
	echo "rshell does not work as expected on [ -f CMakeLists.txt ]"
fi

RSHELLOUTPUTconnector=$(./rshell [ -f header ])

if [ "(False)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -f header ]"
else
	echo "rshell does not work as expected on [ -f header ]"
fi

RSHELLOUTPUTconnector=$(./rshell [ -d header ])

if [ "(True)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -d header ]"
else
	echo "rshell does not work as expected on [ -d header ]"
fi

RSHELLOUTPUTconnector=$(./rshell [ -d CMakeLists.txt ])

if [ "(False)" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on [ -d CMakeLists.txt ]"
else
	echo "rshell does not work as expected on [ -d CMakeLists.txt ]"
fi