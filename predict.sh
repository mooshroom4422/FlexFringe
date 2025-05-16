#!/bin/bash

# $1 -> input data
# $2 -> ini file

[[ -f "$1" ]] || { echo "not a valid input file"; exit 1; }
[[ -f "$2" ]] || { echo "not a valid ini file"; exit 1; }

list=""
num=0

m=$(head -n 1 "$1" | cut -d ' ' -f1)
echo $m

for apta in tmp/*.final.json
do
	echo "[debug] $apta"
	echo "[debug] ./cmake-build-debug/flexfringe --ini "$2" "$1" --mode predict --aptafile $apta --predicttype true"
	./cmake-build-debug/flexfringe --ini "$2" "$1" --mode predict --aptafile $apta --predicttype true
	mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type',' predicted type probability' "$apta.result" | tail -n +2 > "$apta.res"

	list=$list" $apta.res"
	num=$((num+1))
done

echo $m
echo $num
echo $list
./combine tmp/final.res $m $num $list

cat "$1" | tail -n +2 | cut -d ' ' -f 1 > tmp/expected
total=$(wc -l tmp/expected | cut -d ' ' -f 1)

for result in tmp/*.res
do
	echo "[debug] $result"
	# cut -d ';' -f 1 "$result" | tr -d ' ' > "$result.asdasd"
	wrong=$(cut -d ';' -f 1 "$result" | tr -d ' ' | diff -b - tmp/expected | grep -c "<")
	ok=$((total-wrong))
	echo "$ok / $total"
	acc=$(echo "scale=4;$ok / $total" | bc)
	echo $acc
	echo "$acc;$result" >> tmp/res.txt
done
