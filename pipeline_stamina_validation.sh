#!/bin/bash

echo "starting pipeline" >> log.txt
start=$(date +%s)

mkdir stamina_results || { echo "failed to create directory stamina_results"; exit 1; }
dir="stamina_validation_split"

for (( i=1; i<=100; i++ ))
do 
	train=$i"_training.txt.dat.train"
	tst=$i"_training.txt.dat.test"
	validation=$i"_training.txt.dat.valid"
	echo "train $train" | tee -a log.txt

	./gen_sub "data/$dir/$train" tmp.conf 999999 || exit 1;

	# random shuffle and pick only 100
	model_num=100
	echo "$model_num" > now.conf
	tail -n +2 tmp.conf | shuf | head -n "$model_num" >> now.conf

	# cp config.conf now.conf
	./ensemble.sh "data/$dir/$train" ini/edsm.ini now.conf >> log.txt || exit 1;
	
	cp now.conf tmp/config.conf

	echo "pick $validation" | tee -a log.txt
	./predict.sh "data/$dir/$validation" ini/edsm.ini 2 >> log.txt || exit 1;
	./pick.sh 9 || exit 1;

	# rm ./tmp/res.txt
	rm -r tmp

	./merge "data/$dir/$train" "data/$dir/$validation" ./merged_input.dat

	./ensemble.sh ./merged_input.dat ini/edsm.ini _ensemble_config.conf >> log.txt || exit 1;

	echo "test $tst" | tee -a log.txt
	./predict.sh "data/$dir/$tst" ini/edsm.ini 2 >> log.txt || exit 1;

	cp ./pre_pick.txt ./tmp
	cp ./pre_pick_sort.txt ./tmp

	mv _ensemble_config.conf tmp
	mv tmp stamina_results/stamina"$i"_res

	echo "finished!" | tee -a log.txt
done

# cp config.conf stamina_results
mv log.txt stamina_results
cp ./pipeline_stamina_validation.sh stamina_results

notify-send "finished"
end=$(date +%s)
runtime=$((end-start))
echo "took: $runtime seconds" | tee -a log.txt
