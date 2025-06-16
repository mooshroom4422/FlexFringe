#!/bin/bash

# $1 -> input data
# $2 -> ini file
# $3 -> combine threshold
# $4 -> iteration number

[[ -f "$1" ]] || { echo "not a valid input file"; exit 1; }
[[ -f "$2" ]] || { echo "not a valid ini file"; exit 1; }
[[ -n "$3" ]] || { echo "add threshold"; exit 1; }
[[ -n "$4" ]] || { echo "add iteration number"; exit 1; }

list=""
num=0

m=$(head -n 1 "$1" | cut -d ' ' -f1)
echo $m

for apta in "tmp$4"/*.final.json
do
	echo "[debug] $apta"
	echo "[debug] ./build/flexfringe --ini "$2" "$1" --mode predict --aptafile $apta --predicttype true"
	./build/flexfringe --ini "$2" "$1" --mode predict --aptafile $apta --predicttype true
	mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type',' predicted type probability' "$apta.result" | tail -n +2 > "$apta.res"

	mlr --fs ';' --icsvlite --ocsv cut -f ' predicted trace type' "$apta.result" | tail -n +2 > "$apta.res.raw"

	list=$list" $apta.res"
	num=$((num+1))
done

echo $m
echo $num
echo $list

if [ "$3" -eq 0 ]; then
	./combine "tmp$4"/final.res "$m" $num $list
else
	echo "./combine tmp$4/final.res $m $num $list $3"
	./combine "tmp$4"/final.res "$m" $num $list "$3"
fi


cat "$1" | tail -n +2 | cut -d ' ' -f 1 > "tmp$4"/expected
total=$(wc -l "tmp$4"/expected | cut -d ' ' -f 1)

for result in "tmp$4"/*.res
do
	echo "[debug] $result"
	acc=$(python3 acc.py "$result" "tmp$4/expected")
	echo $acc
	echo "$acc;$result" >> "tmp$4"/res.txt
done

python3 extended.py "tmp$4"/final.res "tmp$4"/expected "tmp$4"/extended.txt
