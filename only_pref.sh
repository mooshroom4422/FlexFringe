#!/bin/bash

dir="stamina_validation_split"
for (( i=1; i<=100; i++ )) do 
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
	wrong=$(cut -d ';' -f 1 "$result" | tr -d ' ' | diff -b - tmp/expected | grep -c "<")
	ok=$((total-wrong))
	echo "$ok / $total"
	acc=$(echo "scale=4;$ok / $total" | bc)
	echo $acc
	echo "$acc;$result" > ./tmp/res.txt

	mv ./tmp/res.txt ./stamina_results/stamina"$i"_res/pref.txt
	cat ./stamina_results/stamina"$i"_res/pref.txt >> ./stamina_results/stamina"$i"_res/res.txt
done
