#!/usr/bin/python
import os
import fontforge

fontFileName = os.sys.argv[1]
path = fontFileName
for font in fontforge.fontsInFile(fontFileName):
    try:
        f = fontforge.open(u'%s(%s)' % (fontFileName, font))
        f.generate(font + ".svg")
        f.generate(font + ".otf", "", "glyph-map-file")

    except EnvironmentError:
        print("Error opening font file %s!" % fontFileName)
        os.sys.exit(1)
