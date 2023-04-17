#!/bin/sh

# Requires python
# Requires svgpathtools python library (install with 'pip3 install svgpathtools')

PYTHON="python3"

echo "Generating C++ header file ..."
$PYTHON generate.py smufl

echo "Generating Bravura files ..."
$PYTHON generate.py extract Bravura
$PYTHON generate.py css Bravura

echo "Generating Leipzig files ..."
$PYTHON generate.py check Leipzig
$PYTHON generate.py extract Leipzig
$PYTHON generate.py css Leipzig

echo "Generating Gootville files ..."
$PYTHON generate.py extract Gootville
$PYTHON generate.py css Gootville

echo "Generating Petaluma files ..."
$PYTHON generate.py extract Petaluma
$PYTHON generate.py css Petaluma

echo "Generating Leland files ..."
$PYTHON generate.py extract Leland
$PYTHON generate.py css Leland

echo "Done!"
