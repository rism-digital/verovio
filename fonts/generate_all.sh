#!/bin/sh

# Requires python
# Requires svgpathtools and lxml python libraries (install with 'pip3 install %name%')

PYTHON="python3"

echo "Generating C++ header file ..."
$PYTHON generate-smufl-code.py

echo "Generating Bravura files ..."
$PYTHON extract-bounding-boxes.py Bravura.svg json/bravura_metadata.json ../data/Bravura.xml

echo "Generating Leipzig files ..."
$PYTHON extract-bounding-boxes.py Leipzig.svg json/leipzig_metadata.json ../data/Leipzig.xml

echo "Generating Gootville files ..."
$PYTHON extract-bounding-boxes.py Gootville.svg json/gootville-1.2_metadata.json ../data/Gootville.xml

echo "Generating Petaluma files ..."
$PYTHON extract-bounding-boxes.py Petaluma.svg json/petaluma_metadata.json ../data/Petaluma.xml

echo "Generating Leland files ..."
$PYTHON extract-bounding-boxes.py Leland.svg json/leland_metadata.json ../data/Leland.xml

echo "Done!"
