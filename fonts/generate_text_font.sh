# This script generates XML file with bounding boxes for the plain font
# It assumes that shell function for saxon is set in .bashrc. E.g.:
# function saxon {
# java -jar /application_path/saxon9he.jar "$@"
# }
# export -f saxon
# Alternatively check generate_all.sh on how to achieve this with aliases

if [ -z "$1" ]
then
	echo "Text font name need to be provided as argument."
	exit 1
fi

scriptdir=$(dirname "$0")
fontfile="${1%.*}"
fontname=$(basename $fontfile)

echo "Generating metadata for $fontname font"

# generate SVG and G2N files for the text font
fontforge "$scriptdir"/generate_plain_font_metadata.py "$1"
find . -type f -name "$fontfile*.svg" -not -path "./tmp/*" -print0 | while IFS= read -r -d '' fname;
do
	font="${fname%.*}"
	# generate bounding boxes SVG for the text font
	saxon "$font".svg "$scriptdir"/extract-glyphs-all-fonts.xsl > "$scriptdir"/tmp/"$font"-bounding-boxes.svg
	# generate XML file with bounding boxes
	phantomjs "$scriptdir"/generate-bbox.js "$scriptdir"/tmp/"$font"-bounding-boxes.svg "$scriptdir"/../data/text/"$font".xml "$font".g2n
done

echo "Done!"