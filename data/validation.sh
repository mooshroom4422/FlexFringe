#!/bin/bash

for file in staminadata/*.dat
do
	echo $file
	out="stamina_validation_split/"$(basename $file)
	./splitter $file $out 45451771 0.15 0.15
done
