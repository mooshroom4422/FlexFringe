#!/bin/bash

echo "picking n=$1"
cp ./tmp/res.txt ./pre_pick.txt
tail -n +2 ./tmp/res.txt | sort -nrt';' -k1 > ./pre_pick_sort.txt
tail -n +2 ./tmp/res.txt | sort -nrt';' -k1 | head -n"$1" | cut -d ';' -f 2 | rev | cut -d'.' -f2- | rev | tee tmp/ensemble.txt
echo "deleting"
grep -Fxv -f ./tmp/ensemble.txt <(ls tmp/*.final.json) | tee /dev/tty | xargs -d '\n' rm --
sed 's/[^0-9]//g' ./tmp/ensemble.txt > ./tmp/ensemble.raw.txt
echo "$1" > _ensemble_config.conf
awk 'NR==FNR { lines[int($1)+2]; next } FNR in lines' ./tmp/ensemble.raw.txt ./tmp/config.conf >> _ensemble_config.conf
rm ./tmp/*.res
