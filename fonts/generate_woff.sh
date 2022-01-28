#!/bin/bash

font="VerovioText-1.0.sfd"

# Generate bounding boxes for the VerovioText and move to ../data/text
echo "Generating bounding-box file ..."
./generate_text_font $font

# Generate woff.xml
fontforge generate_ff.py $font

# base64 and ouput woff font
woffFont=${font%.sfd}.woff
w=$(base64 $woffFont)

cat woff-1.txt > woff.xml
echo $w >> woff.xml
cat woff-2.txt >> woff.xml

# move woff to data folder
mv woff.xml ../data

# base64 and output ttf font
ttfFont=${font%.sfd}.ttf
t=$(base64 $ttfFont)
echo "var vrvTTF = \"$t\";" > vrv-ttf.js