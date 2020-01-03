#!/bin/bash

for i in `\ls *.mei`; do
    xsltproc subtree.xsl $i | sed '/^$/d' > ../../_includes/features/${i%%.mei}.txt
done
