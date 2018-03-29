#!/usr/bin/python
import os
import fontforge

fontFileName = sys.argv[1]
path = fontFileName
try:
    font = fontforge.open(path)
    font.generate(os.path.splitext(fontFileName)[0] + ".ttf")
    font.generate(os.path.splitext(fontFileName)[0] + ".otf")
    font.generate(os.path.splitext(fontFileName)[0] + ".woff")

except EnvironmentError:
    print ("Aargh... Error opening font file %s!" % fontFileName)
    sys.exit(1)
