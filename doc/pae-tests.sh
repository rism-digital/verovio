#!/bin/bash


dir="./tests/pae/"
d=`date`
echo "<h1>Plain and Easy tests - $d</h1>" > pae-tests.html

for i in `\ls $dir`; do
    echo "<h2>${i//_/ }</h2>" >> pae-tests.html
    for k in `\ls $dir/$i/*.pae`; do
        l=${k##*/}
        echo "<h3>${l//_/ }</h3>" >> pae-tests.html
        echo $k
        ../tools/verovio -r ../data/ -f pae -o - $k -s 50 --no-layout >> pae-tests.html
    done
done

