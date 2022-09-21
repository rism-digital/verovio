#!/bin/sh

font="Leipzig"

# base64 and ouput woff font
woffFont=${font}.woff2
woffFontXML=${woffFont}.xml
w=$(base64 $woffFont)

echo "<style type='text/css'>\n@font-face {\n\tfont-family: '$font';\n\tsrc: url(data:application/font-woff2;charset=utf-8;base64,$w) format('woff2');\n\tfont-weight: normal;\n\tfont-style: normal;\n}\n</style>\n" > $woffFontXML

# move woff to data folder
mv $woffFontXML ../data

# base64 and output ttf font
ttfFont=${font}.ttf
t=$(base64 $ttfFont)
echo "var ${font}TTF = \"$t\";" > $font-ttf.js
