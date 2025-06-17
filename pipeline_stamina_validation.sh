#!/bin/bash

> log.txt
echo "starting pipeline" >> log.txt
start=$(date +%s)

python3 sk.py || exit 1; # make sure that sklearn is installed

mkdir stamina_results || { echo "failed to create directory stamina_results"; exit 1; }
dir="stamina_validation_split_new"

for (( i=1; i<=100; i++ ))
do 
    while (( $(jobs -r | wc -l) >= 80 )); do # for some reason it has to be *4, if there is an inner loop, without it, it works
        sleep 1
    done
	(
	train=$i"_training.txt.dat.train"
	tst=$i"_training.txt.dat.test"
	validation=$i"_training.txt.dat.valid"

	echo "train $train" | tee -a log.txt

	rm -rf "tmp$i"
	mkdir "tmp$i"

	./gen_sub "data/$dir/$train" "tmp$i".conf rss 600 || exit 1;

	# random shuffle and pick only 300 -> moved to gen_sub
	# model_num=300
	# echo "$model_num" > now.conf
	# tail -n +2 "tmp$i".conf | shuf | head -n "$model_num" >> now.conf

	# cp config.conf now.conf
	./ensemble.sh "data/$dir/$train" ini/edsm.ini "tmp$i".conf $i >> log.txt || exit 1;
	
	cp "tmp$i".conf "tmp$i"/config.conf

	echo "pick $validation" | tee -a log.txt
	./predict.sh "data/$dir/$validation" ini/edsm.ini 0.0 $i >> log.txt || exit 1;
	
	tail -n +2 ./"tmp$i"/res.txt | sort -nrt';' -k1 > ./pre_pick_sort"$i".txt
	rm ./"tmp$i"/res.txt

	# for (( ens_size=20; ens_size<=100; ens_size+=2 ))
	# do
	
	# ens_size=80
	echo "starting ensemble $ens_size"

	# ./pick.sh 76 "$i" || exit 1;
	./pick_random_sim.sh 80 "$i" 1000 2 0.9 || exit 1;

	rm ./"tmp$i"/*.dat*

	./merge "data/$dir/$train" "data/$dir/$validation" ./"tmp$i"/merged_input.d

	./ensemble.sh ./"tmp$i"/merged_input.d ini/edsm.ini ./"tmp$i"/ensemble_config.conf $i >> log.txt || exit 1;

	echo "test $tst" | tee -a log.txt
	# for (( ens_size=1; ens_size<100; ens_size+=1 ))
	# do
		# actual=$(echo "scale=2; $ens_size/100" | bc)
	./predict.sh "data/$dir/$tst" ini/edsm.ini 0.48 $i >> log.txt || exit 1;

		# mkdir ./"tmp$i"/"ens$ens_size"
		# mv ./"tmp$i"/res.txt ./"tmp$i"/"ens$ens_size"
		# mv ./"tmp$i"/extended.txt ./"tmp$i"/"ens$ens_size"
		# cp ./"tmp$i"/ensemble_config.conf ./"tmp$i"/"ens$ens_size"
	# done 

	# cp ./pre_pick.txt ./"tmp$i"
	mv ./pre_pick_sort"$i".txt ./"tmp$i"
	mv ./uncut"$i".txt ./"tmp$i"

	# mv _ensemble_config.conf "tmp$i"
	mv "tmp$i" stamina_results/stamina"$i"_res
	rm "tmp$i".conf

	echo "finished! $i" # | tee -a log.txt
	) &
done

wait

# cp config.conf stamina_results

notify-send "finished"
end=$(date +%s)
runtime=$((end-start))
echo "took: $runtime seconds" | tee -a log.txt

mv log.txt stamina_results
cp ./pipeline_stamina_validation.sh stamina_results
