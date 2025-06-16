#!/bin/bash

for file in staminadata/*.dat
do
	echo $file
	out="stamina_validation_split_new/"$(basename $file)
	python3 split.py $file $out.train $out.valid $out.test
done
