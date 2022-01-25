# Requires python
# Requires svgpathtools and lxml python libraries (install with 'pip3 install %name%')

echo "Generating C++ header file ..."
python3 generate-smufl-code.py

echo "Generating Bravura files ..."
python3 extract-bounding-boxes.py Bravura.svg json/bravura_metadata.json ../data/Bravura.xml

echo "Generating Leipzig files ..."
python3 extract-bounding-boxes.py Leipzig.svg json/leipzig_metadata.json ../data/Leipzig.xml

echo "Generating Gootville files ..."
python3 extract-bounding-boxes.py Gootville.svg json/gootville-1.2_metadata.json ../data/Gootville.xml

echo "Generating Petaluma files ..."
python3 extract-bounding-boxes.py Petaluma.svg json/petaluma_metadata.json ../data/Petaluma.xml

echo "Generating Leland files ..."
python3 extract-bounding-boxes.py Leland.svg json/leland_metadata.json ../data/Leland.xml

echo "Done!"
