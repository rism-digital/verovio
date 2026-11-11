#!/usr/bin/env python3
"""Reproduce the tiny ffi test font committed beside this script."""

from pathlib import Path
from copy import deepcopy
import json

from fontTools.feaLib.builder import addOpenTypeFeaturesFromString
from fontTools.fontBuilder import FontBuilder
from fontTools.pens.ttGlyphPen import TTGlyphPen


def rectangle(x_min: int, y_min: int, x_max: int, y_max: int):
    pen = TTGlyphPen(None)
    pen.moveTo((x_min, y_min))
    pen.lineTo((x_max, y_min))
    pen.lineTo((x_max, y_max))
    pen.lineTo((x_min, y_max))
    pen.closePath()
    return pen.glyph()


builder = FontBuilder(1000, isTTF=True)
glyphs = [".notdef", "space", "f", "i", "ffi"]
builder.setupGlyphOrder(glyphs)
builder.setupCharacterMap({0x20: "space", 0x66: "f", 0x69: "i"})
builder.setupGlyf(
    {
        ".notdef": rectangle(50, 0, 450, 700),
        "space": TTGlyphPen(None).glyph(),
        "f": rectangle(50, 0, 350, 700),
        "i": rectangle(75, 0, 225, 700),
        "ffi": rectangle(50, 0, 850, 700),
    }
)
builder.setupHorizontalMetrics({glyph: (900 if glyph == "ffi" else 500, 0) for glyph in glyphs})
builder.setupHorizontalHeader(ascent=800, descent=-200)
builder.setupNameTable(
    {
        "familyName": "Verovio Test Ligature",
        "styleName": "Regular",
        "uniqueFontIdentifier": "Verovio Test Ligature Regular 1.0",
        "fullName": "Verovio Test Ligature Regular",
        "psName": "VerovioTestLigature-Regular",
    }
)
builder.setupOS2(sTypoAscender=800, sTypoDescender=-200, usWinAscent=800, usWinDescent=200)
builder.setupPost()
builder.setupMaxp()
addOpenTypeFeaturesFromString(builder.font, "feature liga { sub f f i by ffi; } liga;")
builder.font["head"].created = builder.font["head"].modified = 2082844800
builder.font.recalcTimestamp = False
builder.save(Path(__file__).with_name("VerovioTestLigature.ttf"))
conflict = deepcopy(builder.font)
conflict["glyf"]["ffi"] = rectangle(50, 0, 650, 500)
conflict.recalcTimestamp = False
conflict.save(Path(__file__).with_name("VerovioTestLigatureConflict.ttf"))

# Reproduce fonts whose only subfamily record is localized. The face is
# authoritatively bold in OS/2 and head, while name ID 2 deliberately avoids
# the English word "Bold".
localized_bold = deepcopy(builder.font)
localized_bold["OS/2"].usWeightClass = 700
localized_bold["OS/2"].fsSelection = 1 << 5
localized_bold["head"].macStyle = 1 << 0
localized_bold["name"].names = [
    record for record in localized_bold["name"].names if record.nameID not in (2, 17)
]
localized_bold["name"].setName("Fett", 2, 3, 1, 0x407)
localized_bold.recalcTimestamp = False
localized_bold.save(Path(__file__).with_name("VerovioTestLocalizedBold.ttf"))

hyphen = FontBuilder(1000, isTTF=True)
hyphen_glyphs = [".notdef", "space", "hyphen", "h", "y", "p", "e", "n"]
hyphen.setupGlyphOrder(hyphen_glyphs)
hyphen.setupCharacterMap(
    {0x20: "space", 0x2D: "hyphen", 0x68: "h", 0x79: "y", 0x70: "p", 0x65: "e", 0x6E: "n"}
)
hyphen.setupGlyf(
    {
        ".notdef": rectangle(50, 0, 450, 700),
        "space": TTGlyphPen(None).glyph(),
        "hyphen": rectangle(40, 260, 760, 380),
        "h": rectangle(50, 0, 420, 700),
        "y": rectangle(80, -200, 460, 500),
        "p": rectangle(60, -200, 440, 500),
        "e": rectangle(50, 0, 450, 500),
        "n": rectangle(70, 0, 430, 500),
    }
)
hyphen.setupHorizontalMetrics({glyph: (800 if glyph == "hyphen" else 520, 0) for glyph in hyphen_glyphs})
hyphen.setupHorizontalHeader(ascent=800, descent=-200)
hyphen.setupNameTable(
    {
        "familyName": "Verovio Test Hyphen",
        "styleName": "Regular",
        "uniqueFontIdentifier": "Verovio Test Hyphen Regular 1.0",
        "fullName": "Verovio Test Hyphen Regular",
        "psName": "VerovioTestHyphen-Regular",
    }
)
hyphen.setupOS2(sTypoAscender=800, sTypoDescender=-200, usWinAscent=800, usWinDescent=200)
hyphen.setupPost()
hyphen.setupMaxp()
hyphen.font["head"].created = hyphen.font["head"].modified = 2082844800
hyphen.font.recalcTimestamp = False
hyphen.save(Path(__file__).with_name("VerovioTestHyphen.ttf"))

music = FontBuilder(1000, isTTF=True)
music_codes = [0xE050, *range(0xE080, 0xE08C), 0xE0A4]
music_names = [f"uni{code:04X}" for code in music_codes]
music_glyphs = [".notdef", *music_names]
music.setupGlyphOrder(music_glyphs)
music.setupCharacterMap(dict(zip(music_codes, music_names)))
music.setupGlyf(
    {
        ".notdef": rectangle(50, 0, 450, 700),
        **{
            name: rectangle(25 + index * 5, -200, 750 - index * 3, 900)
            for index, name in enumerate(music_names)
        },
    }
)
music.setupHorizontalMetrics({glyph: (800, 0) for glyph in music_glyphs})
music.setupHorizontalHeader(ascent=900, descent=-200)
music.setupNameTable(
    {
        "familyName": "Verovio Test Music",
        "styleName": "Regular",
        "uniqueFontIdentifier": "Verovio Test Music Regular 1.0",
        "fullName": "Verovio Test Music Regular",
        "psName": "VerovioTestMusic-Regular",
    }
)
music.setupOS2(sTypoAscender=900, sTypoDescender=-200, usWinAscent=900, usWinDescent=200)
music.setupPost()
music.setupMaxp()
music.font["head"].created = music.font["head"].modified = 2082844800
music.font.recalcTimestamp = False
music.save(Path(__file__).with_name("VerovioTestMusic.ttf"))
Path(__file__).with_name("VerovioTestMusic_metadata.json").write_text(
    json.dumps({"fontName": "Verovio Test Music", "glyphsWithAnchors": {}}, indent=2) + "\n",
    encoding="utf-8",
)
