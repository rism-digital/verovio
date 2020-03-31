#!/bin/bash


# Requires saxon9ee and phantomjs
# saxon9ee can be aliased from Oxygen in ~/.bash_profile
# alias saxon9ee="java -jar /Applications/oxygen/lib/saxon9ee.jar"

shopt -s expand_aliases
[[ -f ~/.bash_profile ]] && source ~/.bash_profile

if [ ! -e tmp ]; then
    mkdir tmp
fi

if ! command -v saxon9ee >/dev/null 2>&1 ; then
	if command -v saxonb-xslt >/dev/null 2>&1 ; then
		alias saxon9ee='saxonb-xslt -ext:on'
	else
		echo >&2 "Saxon9ee or saxonb-xslt is required.  Aborting.";
		exit 1;
	fi
fi

if ! command -v phantomjs >/dev/null 2>&1 ; then
	echo >&2 "Phantomjs is required.  Aborting.";
	exit 1;
fi

echo "Generating metadata for Leipzig..."
fontforge generate_font_metadata.py Leipzig-5.2.sfd

echo "Generating C++ header file ..."
saxon9ee supported.xsl generate-h.xsl > ../include/vrv/smufl.h

echo "Generating Bravura files ..."
saxon9ee Bravura.svg extract-glyphs.xsl > tmp/Bravura-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Bravura-bounding-boxes.svg ../data/Bravura.xml json/bravura_metadata.json

echo "Generating Leipzig files ..."
saxon9ee Leipzig.svg extract-glyphs.xsl > tmp/Leipzig-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Leipzig-bounding-boxes.svg ../data/Leipzig.xml json/leipzig-5.2_metadata.json

echo "Generating Gootville files ..."
saxon9ee Gootville.svg extract-glyphs.xsl > tmp/Gootville-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Gootville-bounding-boxes.svg ../data/Gootville.xml json/gootville-1.2_metadata.json

echo "Generating Petaluma files ..."
saxon9ee Petaluma.svg extract-glyphs.xsl > tmp/Petaluma-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Petaluma-bounding-boxes.svg ../data/Petaluma.xml json/petaluma_metadata.json

echo "Done!"
