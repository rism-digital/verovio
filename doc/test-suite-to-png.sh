#!/bin/bash

DIR=$1

for testcategory in $DIR/*; do
    # skip empty directory
    if [ "$testcategory" = "*" ]; then continue; fi

    if [ -d "$testcategory" ]; then
        echo "* Converting SVG files for ${testcategory##*/}"
        for testfile in $testcategory/*.svg; do
            rsvg-convert --zoom=2.0 $testfile -f png -o ${testfile%.svg}.png
        done
    fi
done

