#!/bin/bash

echo "starting pipeline" >> log.txt

mkdir stamina_results || { echo "failed to create directory stamina_results"; exit 1; }
dir="stamina_validation_split"

for (( i=1; i<=100; i++ ))
do 
	train=$i"_training.txt.dat.train"
	tst=$i"_training.txt.dat.test"
	validation=$i"_training.txt.dat.valid"
	echo "train $train" | tee -a log.txt
	./ensemble.sh "data/$dir/$train" ini/edsm.ini config.conf >> log.txt || exit 1;

	echo "pick $validation" | tee -a log.txt
	./predict.sh "data/$dir/$validation" ini/edsm.ini >> log.txt || exit 1;
	./pick.sh 5
	rm tmp/res.txt

	echo "test $tst" | tee -a log.txt
	./predict.sh "data/$dir/$tst" ini/edsm.ini >> log.txt || exit 1;

	mv tmp stamina_results/stamina"$i"_res
	echo "finished!" | tee -a log.txt
done

cp config.conf stamina_results
mv log.txt stamina_results

notify-send "finished"
