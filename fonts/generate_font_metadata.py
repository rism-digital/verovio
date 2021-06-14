#! /usr/bin/env python
# -*- coding: utf-8 -*-

# generate_font_metadata.py
# SMuFL metadata generator & font checker for FontForge.
# The script must be invoked from within the font's directory
# Features currently supported:
# - engraving defaults
# - bounding boxes
# - glyph name <-> codepoint(s)
# - anchors
# Metadata for LilyPond are exported in a special format separately (.ly)
# Written by Robert Piéchaud - poeticprocessing 2014
# while designing the November 2.0 font.
# Use, distribute and edit this script as you wish!

import fontforge
import sys
import os
import json


def validateGlyph(glyph):
    result = g.validate(True)
    report = ''
    if result & 0x1 or True:
        if result & 0x2:
            report += '- OPEN CONTOUR'
        if result & 0x4:
            report += '- INTERSECTION'
        if result & 0x8:
            report += '- WRONG DIRECTION'
        if result & 0x20:
            report += '- missing extrema'
        if result & 0x80:
            report += '- too many points'
        # if result & 0x200:
        #    report += '- invalid glyph name'
        if result & 0x40000:
            report += '- points too far apart'
        if result & 0x200000:
            report += '- DUPLICATE GLYPH NAME'
        if result & 0x400000:
            report += '- DUPLICATE UNICODE POINT'

        if report != '':
            print('%s (%#0.2x) issues: %s' % (g.glyphname, glyph.unicode, report))
    return report


nargs = len(sys.argv)
fontFileName = ''
fontDir = ''
outDir = 'json/'
if nargs != 2:
    fontFileName = 'Leipzig.sfd'
    path = fontFileName
    #print ( "Font file name missing" )
    #sys.exit (1)

if fontFileName == '':
    fontFileName = sys.argv[1]
    path = fontFileName
try:
    font = fontforge.open(path)
except EnvironmentError:
    print("Can't open font file %sßß" % fontFileName)
    sys.exit(1)

fontName = os.path.splitext(os.path.basename(fontFileName))[0]
metadata = {'fontName': font.fontname, 'fontVersion': font.version}

jsonComment = 'Automatically generated Metadata for the ' + font.fontname + \
    ' font. Most (but not all) of these metadata come from the SMuFL specifications.'
metadata["_comment"] = jsonComment
# customize these values if necessary:
engravingDefaults = {
    'arrowShaftThickness': 0.16,
    'barlineSeparation': 0.4,
    'beamSpacing': 0.20,
    'beamThickness': 0.55,
    'bracketThickness': 0.5,
    'dashedBarlineDashLength': 0.5,
    'dashedBarlineGapLength': 0.25,
    'dashedBarlineThickness': 0.16,
    'hairpinThickness': 0.16,
    'legerLineExtension': 0.27,
    'legerLineThickness': 0.16,
    'lyricLineThickness': 0.16,
    'octaveLineThickness': 0.16,
    'pedalLineThickness': 0.16,
    'repeatBarlineDotSeparation': 0.16,
    'repeatEndingLineThickness': 0.16,
    'slurEndpointThickness': 0.1,
    'slurMidpointThickness': 0.22,
    'staffLineThickness': 0.08,
    'stemThickness': 0.076,
    'subBracketThickness': 0.16,
    'textEnclosureThickness': 0.16,
    'thickBarlineThickness': 0.5,
    'thinBarlineThickness': 0.16,
    'tieEndpointThickness': 0.1,
    'tieMidpointThickness': 0.22,
    'tupletBracketThickness': 0.16}

metadata["engravingDefaults"] = engravingDefaults

with open('glyphnames.json') as smuflGlyphnamesFile:
    smuflGlyphnames = json.load(smuflGlyphnamesFile)

glyphBBoxes = dict()
glyphsWithAnchors = dict()
count = 0
undefCount = 0
for glyph in font:
    g = font[glyph]

    if "uni" in glyph:
        for smuflGlyphname in smuflGlyphnames.items():
            if glyph.split("uni")[1] in list(smuflGlyphname[1].values())[0]:
                g.glyphname = smuflGlyphname[0]

    if g.unicode != -1 and g.unicode > 31:
        count += 1
        validateGlyph(g)
        g.addExtrema()
        bbox = [round(bb / 250., 4) for bb in g.boundingBox()]
        bBoxSW = (bbox[0], bbox[1])
        bBoxNE = (bbox[2], bbox[3])
        glyphBBoxes[g.glyphname] = {'bBoxNE': bBoxNE, 'bBoxSW': bBoxSW}
        if len(g.anchorPoints) > 0:
            anchors = dict()
            for item in g.anchorPoints:
                anchors[item[0]] = (round(item[2] / 250., 4),
                                    round(item[3] / 250., 4))
            glyphsWithAnchors[g.glyphname] = anchors
    else:
        undefCount += 1
metadata["glyphBBoxes"] = glyphBBoxes
metadata["glyphsWithAnchors"] = glyphsWithAnchors
font.close()

print("\n%d defined glyphs processed (there are %d undefined glyphs)" % (
    count, undefCount - 31))

output = json.dumps(metadata, sort_keys=True, indent=4, separators=(',', ': '))
jsonFileName = outDir + fontName.lower() + "_metadata.json"
with open(jsonFileName, "w") as outfile:
    outfile.write(output)
