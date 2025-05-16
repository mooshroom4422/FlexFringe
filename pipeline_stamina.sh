#!/bin/bash

echo "starting pipeline" >> log.txt

for (( i=1; i<=100; i++ ))
do 
	train=$i"_training.txt.dat.train"
	tst=$i"_training.txt.dat.test"
	echo "train $train" >> log.txt
	echo $train
	./ensemble.sh "data/stamina_split/$train" ini/edsm.ini >> log.txt
	echo "test $tst" >> log.txt
	echo $tst
	./predict.sh "data/stamina_split/$tst" ini/edsm.ini >> log.txt

	mv tmp stamina_results/stamina"$i"_res
	echo "finished!"
	echo "finished!" >> log.txt
done
