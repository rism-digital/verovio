#!/bin/bash

font="VerovioText-1.0.sfd"

shopt -s expand_aliases
[[ -f ~/.bash_profile ]] && source ~/.bash_profile

# Requires saxon9ee and phantomjs
# saxon9ee can be aliased from Oxygen in ~/.bash_profile
# alias saxon9ee="java -jar /Applications/oxygen/lib/saxon9ee.jar"

if [ ! -e tmp ]; then
    mkdir tmp
fi

if ! command -v saxon9ee >/dev/null 2>&1 ; then
	echo >&2 "Saxon9ee is required.  Aborting.";
	exit 1;
fi

if ! command -v phantomjs >/dev/null 2>&1 ; then
	echo >&2 "Phantomjs is required.  Aborting.";
	exit 1;
fi

#./generate_ff.sh $1
fontforge generate_ff.py $font

# base64
woffFont=${font%.sfd}.woff
w=$(base64 $woffFont)

# output
cat woff-1.txt > woff.xml
echo $w >> woff.xml
cat woff-2.txt >> woff.xml

mv woff.xml ../data

# base64
ttfFont=${font%.sfd}.ttf
t=$(base64 $ttfFont)
echo "var vrvTTF = \"$t\";" > vrv-ttf.js

svgFont=${font%.sfd}.svg
baseFont=${font%.sfd}
echo "Generating bounding-box file ..."
saxon9ee $svgFont extract-glyphs-verovio-text.xsl > tmp/${baseFont}-bounding-boxes.svg
phantomjs generate-bbox.js tmp/${baseFont}-bounding-boxes.svg ../data/text/${baseFont}.xml json/verovio-text.json
