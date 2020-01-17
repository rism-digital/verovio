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
            print >> log, ('%s (%#0.2x) issues: %s' %
                           (g.glyphname, glyph.unicode, report))
    return report


nargs = len(sys.argv)
fontFileName = ''
fontDir = ''
outDir = 'json/'
if nargs != 2:
    fontFileName = 'November2.sfd'
    path = fontFileName
    #print ( "Oops... Font file name missing!" )
    #sys.exit (1)

if fontFileName == '':
    fontFileName = sys.argv[1]
    path = fontFileName
try:
    font = fontforge.open(path)
except EnvironmentError:
    print("Aargh... Error opening font file %s!" % fontFileName)
    sys.exit(1)

fontName = os.path.splitext(os.path.basename(fontFileName))[0]

log = open(outDir + fontName.lower() + '.log', 'w+')
print >> log, ("\n# # # %s: checking data & generating metadata... # # #" % (
    fontFileName))
# generating OpenType font:
#font.generate( outDir+fontName+'.otf', '', ('glyph-map-file', 'opentype') )
fontVersion = font.version
metadata = {'fontName': fontName, 'fontVersion': fontVersion}

jsonComment = 'Automatically generated Metadata for the' + fontName + \
    ' font. Most (but not all) of these metadata come from the SMuFL specifications.'
metadata["_comment"] = jsonComment
# customize these values if necessary:
engravingDefaults = {
    'arrowShaftThickness': '0.16',
    'barlineSeparation': '0.4',
    'beamSpacing': '0.20',
    'beamThickness': '0.55',
    'bracketThickness': '0.5',
    'dashedBarlineDashLength': '0.5',
    'dashedBarlineGapLength': '0.25',
    'dashedBarlineThickness': '0.16',
    'hairpinThickness': '0.16',
    'legerLineExtension': '0.4',
    'legerLineThickness': '0.16',
    'lyricLineThickness': '0.16',
    'octaveLineThickness': '0.16',
    'pedalLineThickness': '0.16',
    'repeatBarlineDotSeparation': '0.16',
    'repeatEndingLineThickness': '0.16',
    'slurEndpointThickness': '0.1',
    'slurMidpointThickness': '0.22',
    'staffLineThickness': '0.08',
    'stemThickness': '0.076',
    'subBracketThickness': '0.16',
    'textEnclosureThickness': '0.16',
    'thickBarlineThickness': '0.5',
    'thinBarlineThickness': '0.16',
    'tieEndpointThickness': '0.1',
    'tieMidpointThickness': '0.22',
    'tupletBracketThickness': '0.16'}

metadata["engravingDefaults"] = engravingDefaults

lilypondData = '%{\n'
lilypondData += '  TODO: LilyPond data header\n%}\n\n'
lilypondData += '#(define ' + fontName.lower() + ' \'(\n'
lilypondGlyphs = list()

print >> log, ("  Generating unicode metadata...")
print >> log, ("  Generating LilyPond metadata...")

with open(fontFileName) as f:
    lines = f.readlines()
glyphs = dict()
line_iter = iter(lines)
for anyLine in line_iter:
    line1 = anyLine.split()
    if len(line1) > 0 and line1[0] == 'StartChar:':
        glyphName = line1[1]
        line2 = line_iter.next().split()
        line3 = line_iter.next().split()
        value = int(line2[2])
        if value != '-1' and value > 31:
            mainCodepoint = "%#06X" % value
            initialCodepoint = mainCodepoint
            found = False
            if (value >= 0xE000 and value < 0xE400) or (value >= 0xF400 and value < 0xF800):
                found = True
            if line3[0] == 'AltUni2:':
                alternates = list()
                for item in line3:
                    if item != 'AltUni2:':
                        alternate = item.split('.')
                        value = "%#06X" % int(alternate[0], 16)
                        if not found and int(initialCodepoint, 16) <= 0xff and int(value, 16) >= 0xE000 and int(value, 16) < 0xF800:
                            if int(value, 16) < 0xF000 or int(value, 16) > 0xF0FF:
                                found = True
                            temp = mainCodepoint
                            mainCodepoint = value
                            value = temp
                        alternates.append(value)
                glyphs[glyphName] = {
                    'codepoint': mainCodepoint, 'alternateCodepoints': alternates}
            else:
                glyphs[glyphName] = {'codepoint': mainCodepoint}
            lilypondGlyphs.append(
                '   ("' + glyphName + '" . #x' + "%04X" % int(mainCodepoint, 16) + ')')
metadata["glyphs"] = glyphs

lilypondGlyphs.sort()
lilypondData += '\n'.join(lilypondGlyphs) + '\n))\n'

print >> log, ("  Generating bounding box metadata...")
print >> log, ("  Checking font...")
print >> log
glyphBBoxes = dict()
glyphsWithAnchors = dict()
count = 0
undefCount = 0
for glyph in font:
    g = font[glyph]
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
print >> log, ("\n%d defined glyphs processed (there are %d undefined glyphs)" % (
    count, undefCount - 31))
log.close()
log = open(outDir + fontName.lower() + '.log', 'r')
print log.read()

output = json.dumps(metadata, sort_keys=True, indent=4, separators=(',', ': '))
jsonFileName = outDir + fontName.lower() + "_metadata.json"
lilypondDataFileName = outDir + fontName.lower() + "_data.ly"
with open(jsonFileName, "w") as outfile:
    outfile.write(output)
with open(lilypondDataFileName, "w") as outfile:
    outfile.write(lilypondData)
