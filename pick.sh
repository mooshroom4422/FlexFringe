#!/bin/bash

[[ -n "$1" ]] || { echo "provide number of models"; exit 1; }
[[ -n "$2" ]] || { echo "provide iteartion number"; exit 1; }

echo "picking n=$1"
mv pre_pick_sort"$2".txt uncut"$2".txt

# discard below 0.50, but leave at least one
awk -F';' '{
    if ($1 > 0.5) {
        print; 
        found = 1; 
    } 
    lines[NR] = $0
} 
END {
    if (!found) {
        print lines[1];  # Print the first line if no lines met the condition
    }
}' uncut"$2".txt > pre_pick_sort"$2".txt

head -n"$1" ./pre_pick_sort"$2".txt | cut -d ';' -f 2 | rev | cut -d'.' -f2- | rev | tee "tmp$2"/ensemble.txt
echo "deleting"
grep -Fxv -f ./"tmp$2"/ensemble.txt <(ls "tmp$2"/*.final.json) | tee /dev/tty | xargs -d '\n' rm --
cut -d'/' -f2 ./"tmp$2"/ensemble.txt | sed 's/[^0-9]//g' > ./"tmp$2"/ensemble.raw.txt
echo "$1" > ./"tmp$2"/ensemble_config.conf
awk 'NR==FNR { lines[int($1)+2]; next } FNR in lines' ./"tmp$2"/ensemble.raw.txt ./"tmp$2"/config.conf >> ./"tmp$2"/ensemble_config.conf

rm -f ./"tmp$2"/*.res
