#!/bin/bash

# $1 -> input data
# $2 -> ini file

[[ -f "$1" ]] || { echo "not a valid input file"; exit 1; }
[[ -f "$2" ]] || { echo "not a valid ini file"; exit 1; }
[[ -f "$3" ]] || { echo "not a valid config file"; exit 1; }
[[ -n "$4" ]] || { echo "provide iteration number"; exit 1; }

# mkdir "tmp$4" || exit 1;

echo "./preproc "$1" "$3" "tmp$4"/ 2> /dev/null"
./preproc "$1" "$3" "tmp$4"/ 2> /dev/null

for input in "tmp$4"/*.dat 
do
	echo "$input"
	./build/flexfringe --ini "$2" "$input" --output both
done
