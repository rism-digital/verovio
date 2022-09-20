#!/bin/sh

# Requires python
# Requires svgpathtools and lxml python libraries (install with 'pip3 install %name%')

PYTHON="python3"

echo "Generating C++ header file ..."
$PYTHON generate-smufl-code.py

echo "Generating Bravura files ..."
$PYTHON extract-bounding-boxes.py Bravura

echo "Generating Leipzig files ..."
$PYTHON extract-bounding-boxes.py Leipzig

echo "Generating Gootville files ..."
$PYTHON extract-bounding-boxes.py Gootville

echo "Generating Petaluma files ..."
$PYTHON extract-bounding-boxes.py Petaluma

echo "Generating Leland files ..."
$PYTHON extract-bounding-boxes.py Leland

echo "Done!"
