#! /usr/bin/env python3

import json
import os
import sys
# We can 1-to-1 replace ElementTree with lxml to support such features like
# pretty_print and retaining order of attributes during the output
# import xml.etree.ElementTree as ET
from lxml import etree as ET
from svgpathtools import Path

# Define svg namespace
SVG_NS = "http://www.w3.org/2000/svg"

######################
#  Helper Functions  #
######################


def get_file_content(filepath) -> str:
    """Open file in same directory as script and retrieve its content."""
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), "r")
    content = file.read()
    file.close()
    return content


def write_file_content(filepath, content):
    """Write content to file with path relative to the script directory."""
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), "w")
    file.write(content)
    file.close()


def get_svg_elements(root, tag):
    """Retrieve all elements with given tag name from svg."""
    return root.findall(".//svg:" + tag, {"svg": SVG_NS})  # XPath, recursive

########################
#  Parse Fontname.svg  #
########################


def read_svg_font_file(font_file_name: str):
    """Read SVG file which is passed as argument and retrieve information."""
    try:
        font_svg_content = get_file_content(font_file_name)
    except OSError:
        print(f"Error opening font file {font_file_name}!")
        sys.exit(1)
    root = ET.fromstring(bytes(font_svg_content, encoding="utf-8"))
    font_faces = get_svg_elements(root, "font-face")
    if len(font_faces) != 1:
        print(
            f"Error: the file {font_file_name} should have a unique font-face element!")
        print(f"Please check that the svg has correct namespace: {SVG_NS}")
        sys.exit(1)
    font_family = font_faces[0].get("font-family")
    units_per_em = font_faces[0].get("units-per-em")
    glyphs = get_svg_elements(root, "glyph")
    return (font_family, units_per_em, glyphs)


########################
#   Write xml glyphs   #
########################

def write_xml_glyphs(glyphnames: dict):
    """Etracts glyphs from SVG font as specified in glyphnames."""
    rel_path = f"../data/{font_family}"
    for glyph in glyphs:
        glyph_code = glyph.attrib["glyph-name"][-4:]
        if glyph_code not in glyphnames:
            continue
        root = ET.Element("symbol")
        root.set("id", glyph_code)
        root.set("viewBox", f"0 0 {units_per_em} {units_per_em}")
        root.set("overflow", "inherit")
        if "d" in glyph.attrib:
            path = ET.SubElement(root, "path")
            path.set("transform", "scale(1,-1)")
            path.set("d", glyph.attrib["d"])

        os.makedirs(rel_path, exist_ok=True)
        file_path = f"{rel_path}/{glyph_code}.xml"
        xml = ET.tostring(root, encoding="unicode")
        write_file_content(file_path, xml)

#########################
#  Parse json  #
#########################


def get_json_content(filepath: str) -> dict:
    """Retrieve dictionary with supported SMuFL codepoints and name."""
    json_content = get_file_content(filepath)
    json_items = json.loads(json_content)
    return json_items

#########################
#  Parse supported.xsl  #
#########################


def get_supported_glyph_codes() -> dict:
    """Retrieve dictionary with supported SMuFL codepoints and name."""
    supported_xsl = ET.parse("supported.xsl")
    glyphs = supported_xsl.findall(".//glyph")
    supported_glyphs = {}
    for glyph in glyphs:
        supported_glyphs[glyph.attrib["glyph-code"]] = glyph.attrib["smufl-name"]
    return supported_glyphs


#########################
#  Parse font.g2n  #
#########################


def get_text_names_to_codes(filepath: str) -> dict:
    """Retrieve dictionary mapping glyph code to SMuFL name."""
    result = {}
    with open(filepath, "r") as fin:
        for line in fin:
            items = line.split()
            # g2n file contains glyph-to-name mapping
            # index 3 corresponds to glyph name, and 5 - glyph code
            if len(items) == 6:
                result[items[3]] = items[5]
    return result
    
###################################################
#  Parse font_metadata.json for alternate glyphs  #
###################################################


def get_alternate_glyphs(glyphnames: dict, metadata: dict) -> dict:
    """Retrieve alternate glyphs from font metadata"""
    glyph_alternates = metadata["glyphsWithAlternates"] if "glyphsWithAlternates" in metadata else ""
    if not bool(glyph_alternates):
        return glyph_alternates
    additional_glyphs = {}
    for glyphname, alternates in glyph_alternates.items():
        code = [k for k in glyphnames if glyphnames[k] == glyphname]
        # if glyph is in the supported list - extract alternates for it
        if code:
            for element in alternates["alternates"]:
                additional_glyphs[element["codepoint"][2:]] = element["name"]

    return additional_glyphs

#########################
#  Extract SMUFL font   #
#########################


def extract_smufl_font(root, metadata_file):
    glyphnames = get_supported_glyph_codes()
    metadata = get_json_content(metadata_file)
    glyph_anchors = metadata["glyphsWithAnchors"] if "glyphsWithAnchors" in metadata else ""
    # extract alternate glyphs and append them if any
    alternate_glyphs = get_alternate_glyphs(glyphnames, metadata)
    if bool(alternate_glyphs):
        glyphnames.update(alternate_glyphs)

    # (1) Create xml file for each glyph
    write_xml_glyphs(glyphnames)

    # (2) Output bounding box svg
    for glyph in glyphs:
        # set glyph id
        glyph_code = glyph.attrib["glyph-name"][-4:]
        if glyph_code not in glyphnames:
            continue
        g_element = ET.SubElement(root, "g")
        g_element.set("c", glyph_code)

        # set bounding box values if present
        if "d" in glyph.attrib:
            path = Path(glyph.attrib["d"])
            xmin, xmax, ymin, ymax = path.bbox()
            g_element.set("x", str(round(xmin, 2)))
            g_element.set("y", str(round(ymin, 2)))
            g_element.set("w", str(round(xmax - xmin, 2)))
            g_element.set("h", str(round(ymax - ymin, 2)))
        else:
            g_element.set("x", str(0.0))
            g_element.set("y", str(0.0))
            g_element.set("w", str(0.0))
            g_element.set("h", str(0.0))

        # set set horiz-av-x
        if "horiz-adv-x" in glyph.attrib:
            g_element.set("h-a-x", glyph.get("horiz-adv-x"))
            if not float(g_element.get("w")):
                g_element.set("w", glyph.get("horiz-adv-x"))

        # add glyph anchors if present for current glyph
        current_glyphname = glyphnames[glyph_code] if glyph_code in glyphnames else ""
        if current_glyphname:
            g_element.set("n", current_glyphname)
            if current_glyphname in glyph_anchors:
                for key, value in glyph_anchors[current_glyphname].items():
                    a_element = ET.SubElement(g_element, "a")
                    a_element.set("n", key)
                    a_element.set("x", str(round(value[0], 2)))
                    a_element.set("y", str(round(value[1], 2)))

    return root

#########################
#  Extract text font   #
#########################


def extract_text_font(root, metadata_file):
    glyphcodes = get_text_names_to_codes(metadata_file)
    # (2) Output bounding box svg
    for glyph in glyphs:
        if glyph.attrib["glyph-name"] not in glyphcodes:
            continue
        # set glyph id
        g_element = ET.SubElement(root, "g")
        name = glyph.attrib["glyph-name"]
        glyph_code = glyphcodes[name]
        #glyph_code = name.split("uni")[-1]
        g_element.set("c", glyph_code)
        g_element.set("n", name)

        # set bounding box values if present
        if "d" in glyph.attrib:
            path = Path(glyph.attrib["d"])
            xmin, xmax, ymin, ymax = path.bbox()
            g_element.set("x", str(round(xmin, 2)))
            g_element.set("y", str(round(ymin, 2)))
            g_element.set("w", str(round(xmax - xmin, 2)))
            g_element.set("h", str(round(ymax - ymin, 2)))
        else:
            g_element.set("x", str(0.0))
            g_element.set("y", str(0.0))
            g_element.set("w", str(0.0))
            g_element.set("h", str(0.0))

        # set set horiz-av-x
        horiz_adv_x = glyph.attrib["horiz-adv-x"] if "horiz-adv-x" in glyph.attrib else ""
        if horiz_adv_x:
            g_element.set("h-a-x", horiz_adv_x)

    return root

##################
#  Main program  #
##################


if __name__ == "__main__":
    # (1) Parse files
    font_file = os.sys.argv[1]
    metadata_file = os.sys.argv[2]
    (font_family, units_per_em, glyphs) = read_svg_font_file(font_file)

    root = ET.Element("bounding-boxes")
    root.set("font-family", font_family)
    root.set("units-per-em", units_per_em)

    if metadata_file.endswith(".json"):
        root = extract_smufl_font(root, metadata_file)
    elif metadata_file.endswith(".g2n"):
        root = extract_text_font(root, metadata_file)

    # (2) Combine resulting content and output to the file
    xml_output_content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    xml_output_content += ET.tostring(root,
                                      encoding="unicode", pretty_print=True)

    output_file = os.sys.argv[3]
    write_file_content(output_file, xml_output_content)
