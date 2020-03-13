#!/bin/bash

OUTPUTconnector=$(echo A || (echo B))
RSHELLOUTPUTconnector=$(./rshell echo A || (echo B))

if [ "$OUTPUTconnector" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A || (echo B)"
else
	echo "rshell does not work as expected on echo A || (echo B)"
fi

OUTPUTconnector=$(echo A || (echo B || echo C))
RSHELLOUTPUTconnector=$(./rshell echo A || (echo B || echo C))

if [ "$OUTPUTconnector" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A || (echo B || echo C)"
else
	echo "rshell does not work as expected on echo A || (echo B || echo C)"
fi

OUTPUTconnector=$((echo A && echo B) || (echo C && echo D))
RSHELLOUTPUTconnector=$(./rshell '(echo A && echo B) || (echo C && echo D)')

if [ "$OUTPUTconnector" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on (echo A && echo B) || (echo C && echo D)"
else
	echo "rshell does not work as expected on (echo A && echo B) || (echo C && echo D)"
fi

OUTPUTconnector=$( echo A && ((     echo B                  ) || echo C ) )
RSHELLOUTPUTconnector=$(./rshell 'echo A && ((     echo B                  ) || echo C )')

if [ "$OUTPUTconnector" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on echo A && ((     echo B                  ) || echo C )"
else
	echo "rshell does not work as expected on echo A && ((     echo B                  ) || echo C )"
fi

OUTPUTconnector=$( (((((echo E) && echo D ) && echo C ) && echo B ) && echo A) )
RSHELLOUTPUTconnector=$(./rshell '(((((echo E) && echo D ) && echo C ) && echo B ) && echo A)')

if [ "$OUTPUTconnector" = "$RSHELLOUTPUTconnector" ]
then
	echo "rshell works as expected on (((((echo E) && echo D ) && echo C ) && echo B ) && echo A)"
else
	echo "rshell does not work as expected on (((((echo E) && echo D ) && echo C ) && echo B ) && echo A)"
fi