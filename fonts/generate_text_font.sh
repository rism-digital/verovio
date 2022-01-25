# This script generates XML file with bounding boxes for the plain font
# One input argument (font name) is expected when running this script

scriptdir=$(dirname "$0")
fontfile="${1%.*}"

if [ -z "$1" ]
  then
    echo "Font name is expected as an argument"
	exit 1
fi

echo "Generating metadata for $fontfile font"

# generate SVG and G2N files for the text font
fontforge generate_plain_text.py "$1"
# generate XML file with bounding boxes for the text font
python3 extract-bounding-boxes.py "$fontfile".svg "$fontfile".g2n ../data/text/"$fontfile".xml 


echo "Done!"