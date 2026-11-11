#!/bin/sh

# Requires python with the fonttools and brotli libraries (install with 'pip3 install fonttools brotli')

PYTHON="python3"

echo "Generating C++ header and name table files ..."
$PYTHON generate.py smufl

echo "Generating Leipzig files ..."
$PYTHON generate.py check Leipzig
$PYTHON generate.py bundle Leipzig

echo "Generating Bravura files ..."
$PYTHON generate.py bundle Bravura

echo "Done!"
