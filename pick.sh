#!/bin/bash

echo "picking n=$1"
tail -n +2 ./tmp/res.txt | sort -nrt';' -k1 | head -n"$1" | cut -d ';' -f 2 | rev | cut -d'.' -f2- | rev | tee tmp/ensemble.txt
echo "deleting"
grep -Fxv -f ./tmp/ensemble.txt <(ls tmp/*.final.json) | tee /dev/tty | xargs -d '\n' rm --
rm ./tmp/*.res
