#!/bin/bash

for i in `LS *.mei`; do
    xsltproc subtree.xsl $i | sed '/^$/d' > ../../_includes/features/${i%%.mei}.txt
done