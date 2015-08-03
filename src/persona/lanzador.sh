#!/bin/bash

for((i=1; i <= $1; i++)); do
	./persona 'n' &
	echo "ingresando persona $i" 
#	sleep 1
done
