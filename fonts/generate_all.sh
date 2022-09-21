#!/bin/sh

# Requires python
# Requires svgpathtools python library (install with 'pip3 install svgpathtools')

PYTHON="python3"

echo "Generating C++ header file ..."
$PYTHON generate.py smufl

echo "Generating Bravura files ..."
$PYTHON generate.py extract Bravura

echo "Generating Leipzig files ..."
$PYTHON generate.py extract Leipzig

echo "Generating Gootville files ..."
$PYTHON generate.py extract Gootville

echo "Generating Petaluma files ..."
$PYTHON generate.py extract Petaluma

echo "Generating Leland files ..."
$PYTHON generate.py extract Leland

echo "Done!"
