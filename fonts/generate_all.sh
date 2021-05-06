#!/bin/zsh

# Requires phantomjs and python3 with package lxml

if [ ! -e tmp ]; then
    mkdir tmp
fi

if ! command -v phantomjs >/dev/null 2>&1 ; then
	echo >&2 "Phantomjs is required.  Aborting.";
	exit 1;
fi

echo "Generating metadata for Leipzig..."
fontforge generate_font_metadata.py Leipzig-5.2.sfd

echo "Generating C++ header and implementation file ..."
python3 generate-h.py > ../include/vrv/smufl.h
python3 generate-cpp.py > ../src/smufl.cpp

echo "Generating Bravura files ..."
python3 extract-glyphs.py Bravura.svg > tmp/Bravura-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Bravura-bounding-boxes.svg ../data/Bravura.xml json/bravura_metadata.json

echo "Generating Leipzig files ..."
python3 extract-glyphs.py Leipzig.svg > tmp/Leipzig-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Leipzig-bounding-boxes.svg ../data/Leipzig.xml json/leipzig-5.2_metadata.json

echo "Generating Gootville files ..."
python3 extract-glyphs.py Gootville.svg > tmp/Gootville-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Gootville-bounding-boxes.svg ../data/Gootville.xml json/gootville-1.2_metadata.json

echo "Generating Petaluma files ..."
python3 extract-glyphs.py Petaluma.svg > tmp/Petaluma-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Petaluma-bounding-boxes.svg ../data/Petaluma.xml json/petaluma_metadata.json

echo "Generating Leland files ..."
python3 extract-glyphs.py Leland.svg > tmp/Leland-bounding-boxes.svg
phantomjs generate-bbox.js tmp/Leland-bounding-boxes.svg ../data/Leland.xml json/leland_metadata.json

echo "Done!"
