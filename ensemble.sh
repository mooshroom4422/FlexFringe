#!/bin/bash

# $1 -> input data
# $2 -> ini file

[[ -f "$1" ]] || { echo "not a valid input file"; exit 1; }
[[ -f "$2" ]] || { echo "not a valid ini file"; exit 1; }
[[ -f "$3" ]] || { echo "not a valid config file"; exit 1; }

mkdir tmp || exit 1;

./preproc "$1" "$3" tmp/ 2> /dev/null

for input in tmp/*.dat 
do
	echo "$input"
	./cmake-build-debug/flexfringe --ini "$2" "$input" --output both
done
