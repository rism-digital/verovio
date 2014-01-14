#!/bin/bash

TEST_DIR="../../tests/pae/single/"
DIRS=`ls $TEST_DIR`

HEAD=` cat <<HERE

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

    <title>Dynamic demo</title>

<script src="../build/aruspix.js" type="text/javascript"></script>	

<script type="text/javascript">

convertMusic = Module.cwrap('convertMusic', 'string', ['string', 'string'])

function render_music( music, format )
{	
	var svg = convertMusic(music, JSON.stringify({inputFormat: format, scale: 50, pageHeight: 250, pageWidth: 2000}));
	return svg;

};

</script>
</head>
HERE`

echo "$HEAD"
echo "<BODY>"

for d in $DIRS; do
	echo "<h3>$d</h3>"
	
	FILES=`ls $TEST_DIR/$d`
	
	for f in $FILES; do
		
		echo "$f<p>"
		echo "<div id=\"$f\"></div>"
		echo "</p>"
		
		echo "<xmp id=\"$f-data\" style=\"display:none;\">"
		cat $TEST_DIR/$d/$f
		echo "</xmp>"
		echo "<script>"
		echo "var v = document.getElementById('$f-data').textContent;"
		echo "var svg = render_music(v, 'pae');"
		echo "document.getElementById('$f').innerHTML = svg;"
		echo "</script>"
		
	done
	
done

echo "</BODY>"