#! /usr/bin/env python3

import os
import sys
import xml.etree.ElementTree as ET

# Define svg namespace
svg_ns = 'http://www.w3.org/2000/svg'

######################
#  Helper Functions  #
######################


def get_file_content(filepath):
    """Open file in same directory as script and retrieve its content."""
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), 'r')
    content = file.read()
    file.close()
    return content


def write_file_content(filepath, content):
    """Write content to file with path relative to the script directory."""
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), 'w')
    file.write(content)
    file.close()


def get_elements(xml, tag):
    """Retrieve all elements with given tag name from xml."""
    root = ET.fromstring(bytes(xml, encoding='utf-8'))
    return root.findall('.//' + tag)  # XPath, recursive


def get_svg_elements(root, tag):
    """Retrieve all elements with given tag name from svg."""
    return root.findall('.//svg:' + tag, {'svg': svg_ns})  # XPath, recursive

#########################
#  Parse supported.xsl  #
#########################


def get_supported_glyph_codes():
    """Retrieve dictionary mapping glyph code to SMuFL name."""
    xml = get_file_content('supported.xsl')
    glyphs = get_elements(xml, 'glyph')
    result = {}
    for glyph in glyphs:
        result[glyph.attrib['glyph-code']] = glyph.attrib['smufl-name']
    return result

########################
#  Parse Fontname.svg  #
########################


def read_svg_file():
    """Read svg file which is passed as argument and retrieve information."""
    font_file_name = os.sys.argv[1]
    try:
        font_svg_content = get_file_content(font_file_name)
    except OSError:
        print(f"Error opening font file {font_file_name}!")
        sys.exit(1)
    root = ET.fromstring(bytes(font_svg_content, encoding='utf-8'))
    font_faces = get_svg_elements(root, "font-face")
    if len(font_faces) != 1:
        print(f"Error: the file {font_file_name} should have a unique font-face element!")
        print(f"Please check that the svg has correct namespace: {svg_ns}")
        sys.exit(1)
    font_family = font_faces[0].get("font-family")
    units_per_em = font_faces[0].get("units-per-em")
    glyphs = get_svg_elements(root, "glyph")
    return (font_family, units_per_em, glyphs)

##################
#  Main program  #
##################


if __name__ == '__main__':
    # (1) Parse files
    supported_glyph_codes = get_supported_glyph_codes()
    (font_family, units_per_em, glyphs) = read_svg_file()

    # (2) Create xml file for each glyph
    for glyph in glyphs:
        name = glyph.attrib['glyph-name']
        glyph_code = name.split('uni')[-1]
        if glyph_code in supported_glyph_codes:
            root = ET.Element('symbol')
            root.set('id', glyph_code)
            root.set('viewBox', f"0 0 {units_per_em} {units_per_em}")
            root.set('overflow', 'inherit')
            path = ET.SubElement(root, 'path')
            path.set('transform', 'scale(1,-1)')
            if 'd' in glyph.attrib:
                path.set('d', glyph.attrib['d'])
            file_path = f"../data/{font_family}/{glyph_code}-{supported_glyph_codes[glyph_code]}.xml"
            xml = ET.tostring(root, encoding='unicode')
            write_file_content(file_path, xml)

    # (3) Output bounding box svg
    root = ET.Element('svg')
    root.set('xmlns', svg_ns)
    root.set('version', '1.1')
    root.set('font-family', font_family)
    fontface = ET.SubElement(root, 'font-face')
    fontface.set('units-per-em', units_per_em)
    for glyph in glyphs:
        name = glyph.attrib['glyph-name']
        glyph_code = name.split('uni')[-1]
        if glyph_code in supported_glyph_codes:
            path = ET.SubElement(root, 'path')
            path.set('name', supported_glyph_codes[glyph_code])
            path.set('id', glyph_code)
            horiz_adv_x = glyph.attrib['horiz-adv-x'] if 'horiz-adv-x' in glyph.attrib else ''
            path.set('horiz-adv-x', horiz_adv_x)
            path.set('transform', 'scale(1,-1)')
            if 'd' in glyph.attrib:
                path.set('d', glyph.attrib['d'])
    print(ET.tostring(root, encoding='unicode'))
