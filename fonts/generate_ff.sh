#!/opt/local/bin/fontforge

# convert the font with fontforge
Open($1)
Generate($1:r + ".ttf")
Generate($1:r + ".otf")
Generate($1:r + ".woff")

