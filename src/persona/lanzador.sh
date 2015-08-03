#!/bin/bash

for((i=1; i <= $1; i++)); do
	./persona 'n' &
	echo "ingresando persona normal $i" 
done

for((i=1; i <= $1; i++)); do
	./persona 'i' &
	echo "ingresando investigador $i" 
done