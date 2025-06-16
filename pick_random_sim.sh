#!/bin/zsh

[[ -n "$1" ]] || { echo "provide number of models"; exit 1; }
[[ -n "$2" ]] || { echo "provide iteartion number"; exit 1; }
[[ -n "$3" ]] || { echo "provide number of randomwalks"; exit 1; }
[[ -n "$4" ]] || { echo "provide randomwalk factor"; exit 1; }
[[ -n "$5" ]] || { echo "provide accepting sim"; exit 1; }

echo "picking n=$1, randomwalk"
mkdir "asd$2"
for apta in "tmp$2"/*.final.json
do
	./build/flexfringe --ini ini/edsm.ini ./data/staminadata/1_training.txt.dat --mode randomwalk --aptafile "$apta"  --numwalks "$3" --walkfactor "$4"

	config_line=$(echo "$apta" | grep -oP 'model_\K[0-9]+')
	config_line=$((config_line))
	config_line=$((config_line+2))
	line=$(sed -n "$config_line"p "tmp$2/config.conf")
	echo $apta
	echo $config_line
	echo $line
	echo "1" > "tmp$2.walkconf"
	echo "$line" >> "tmp$2.walkconf"
	
	./preproc "$apta.walk" "tmp$2.walkconf" "asd$2"/

	mv "asd$2"/model_00.dat "$apta.walk"
done

rm -rf "asd$2"

mkdir "trash$2"
./pick_sim "./pre_pick_sort$2.txt" "$1" "$5" "comb$2.txt" "tmp$2/config.conf" "trash$2" > ./"tmp$2"/ensemble.txt
rm -rf "trash$2"
rm "comb$2.txt"
rm "comb$2.txt.1"
rm "comb$2.txt.2"
rm "comb$2.txt.res"
rm "comb$2.txt.conf"
rm "tmp$2.walkconf"

cut -d'/' -f2 ./"tmp$2"/ensemble.txt | sed 's/[^0-9]//g' > ./"tmp$2"/ensemble.raw.txt
echo "$1" > ./"tmp$2"/ensemble_config.conf
awk 'NR==FNR { lines[int($1)+2]; next } FNR in lines' ./"tmp$2"/ensemble.raw.txt ./"tmp$2"/config.conf >> ./"tmp$2"/ensemble_config.conf

rm -f ./"tmp$2"/*.res
rm -f ./"tmp$2"/*.walk
