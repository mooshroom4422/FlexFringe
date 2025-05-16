#!/bin/bash

for file in staminadata/*.dat
do
	echo $file
	out="stamina_split/"$(basename $file)
	./splitter $file $out 123123 0.2
done
