#!/bin/bash

mkdir stamina_results || { echo "failed to create directory stamina_results"; exit 1; }
rm -rf tmp
echo "" > ./only_pref.txt
dir="stamina_validation_split_new"
for (( i=1; i<=100; i++ )) do 
	mkdir tmp
	echo "[debug] $i"
	train=$i"_training.txt.dat.train"
	tst=$i"_training.txt.dat.test"
	validation=$i"_training.txt.dat.valid"
	
	./merge "data/$dir/$train" "data/$dir/$validation" ./tmp/input.tmp
	./cmake-build-debug/flexfringe --ini ./ini/edsm.ini ./tmp/input.tmp --output both
		
	apta=./tmp/input.tmp.ff.final.json
	./cmake-build-debug/flexfringe --ini ./ini/edsm.ini "data/$dir/$tst" --mode predict --aptafile $apta --predicttype true
	mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type',' predicted type probability' "$apta.result" | tail -n +2 > "$apta.res"

	mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type' "$apta.result" | tail -n +2 > "$apta.res.raw"

	cat "data/$dir/$tst" | tail -n +2 | cut -d ' ' -f 1 > tmp/expected
	total=$(wc -l tmp/expected | cut -d ' ' -f 1)

	result=tmp/input.tmp.ff.final.json.res
	echo "[debug] $result"
	# cut -d ';' -f 1 "$result" | tr -d ' ' > "$result.asdasd"
	# WRONG! diff doesnt work this way!!
	# wrong=$(cut -d ';' -f 1 "$result" | tr -d ' ' | diff -b - tmp/expected | grep -c "<")
	# ok=$((total-wrong))
	# echo "$ok / $total"
	# acc=$(echo "scale=4;$ok / $total" | bc)
	acc=$(python3 acc.py "$result" tmp/expected)
	echo $acc
	echo $acc >> ./only_pref.txt
	echo "$acc;$result" > ./tmp/res.txt

	python3 extended.py "$result" tmp/expected tmp/extended.txt
	
	mv tmp ./stamina_results/stamina"$i"_res
	# mv ./tmp/res.txt ./stamina_results/stamina"$i"_res/pref.txt
	# cat ./stamina_results/stamina"$i"_res/pref.txt >> ./stamina_results/stamina"$i"_res/res.txt
	# mv ./tmp/input.tmp ./stamina_results/stamina"$i"_res
	# mv ./tmp/input.tmp.ff.final.json ./stamina_results/stamina"$i"_res
	# mv ./tmp/input.tmp.ff.final.json.res ./stamina_results/stamina"$i"_res
	# mv ./tmp/input.tmp.ff.final.json.res.raw ./stamina_results/stamina"$i"_res
	# mv ./tmp/extended.txt ./stamina_results/stamina"$i"_res
done
