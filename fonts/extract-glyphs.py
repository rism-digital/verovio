#! /usr/bin/env python3

import os
from lxml import etree

# Define svg namespace
svg_ns = "http://www.w3.org/2000/svg"

######################
#  Helper Functions  #
######################

# Open file in same directory as script and retrieve its content
def get_file_content(filepath):
   __location__ = os.path.realpath(os.path.dirname(__file__))
   file = open(os.path.join(__location__, filepath), 'r')
   content = file.read()
   file.close()
   return content

# Write content to file with path relative to the script directory
def write_file_content(filepath, content):
   __location__ = os.path.realpath(os.path.dirname(__file__))
   file = open(os.path.join(__location__, filepath), 'w')
   file.write(content)
   file.close()

# Retrieve all elements with given tag name from xml
def get_elements(xml, tag):
   root = etree.fromstring(bytes(xml, encoding='utf-8'))
   return root.findall('.//' + tag)  # XPath, recursive

# Retrieve all elements with given tag name from svg
def get_svg_elements(root, tag):
   return root.findall('.//{' + svg_ns + '}' + tag)  # XPath, recursive

# Create standard svg root node
def create_svg_root():
   root = etree.Element("svg")
   root.set("xmlns", svg_ns)
   root.set("version", "1.1")
   return root

#########################
#  Parse supported.xsl  #
#########################

# Retrieve dictionary mapping glyph code to smufl name
def get_supported_glyph_codes():
   xml = get_file_content('supported.xsl')
   glyphs = get_elements(xml, 'glyph')
   dict = {}
   for glyph in glyphs:
      dict[glyph.attrib["glyph-code"]] = glyph.attrib["smufl-name"]
   return dict

########################
#  Parse Fontname.svg  #
########################

# Read svg file which is passed as argument and retrieve information
def read_svg_file():
   font_file_name = os.sys.argv[1]
   try:
      font_svg_content = get_file_content(font_file_name)
   except EnvironmentError:
      print("Error opening font file %s!" % font_file_name)
      os.sys.exit(1)
   root = etree.fromstring(bytes(font_svg_content, encoding='utf-8'))
   font_faces = get_svg_elements(root, "font-face")
   if (len(font_faces) != 1):
      print("Error: the file %s should have a unique font-face element!" % font_file_name)
      os.sys.exit(1)
   font_family = font_faces[0].get("font-family")
   units_per_em = font_faces[0].get("units-per-em")
   glyphs = get_svg_elements(root, "glyph")
   return (font_family, units_per_em, glyphs)

##################
#  Main program  #
##################

# (1) Parse files
supported_glyph_codes = get_supported_glyph_codes()
(font_family, units_per_em, glyphs) = read_svg_file()

# (2) Create xml file for each glyph
for glyph in glyphs:
   name = glyph.attrib["glyph-name"]
   id = name.split("uni")[-1]
   if (id in supported_glyph_codes):
      root = etree.Element("symbol")
      root.set("id", id)
      root.set("viewBox", f"0 0 {units_per_em} {units_per_em}")
      root.set("overflow", "inherit")
      path = etree.SubElement(root, "path")
      path.set("transform", "scale(1,-1)")
      if "d" in glyph.attrib:
         path.set("d", glyph.attrib["d"])
      file_path = f"../data/{font_family}/{id}-{supported_glyph_codes[id]}.xml"
      xml = etree.tostring(root, pretty_print=True, encoding='unicode')
      write_file_content(file_path, xml)

# (3) Output bounding box svg
root = create_svg_root()
root.set("font-family", font_family)
fontface = etree.SubElement(root, "font-face")
fontface.set("units-per-em", units_per_em)
for glyph in glyphs:
   name = glyph.attrib["glyph-name"]
   id = name.split("uni")[-1]
   if (id in supported_glyph_codes):
      path = etree.SubElement(root, "path")
      path.set("name", supported_glyph_codes[id])
      path.set("id", id)
      horiz_adv_x = ""
      if "horiz-adv-x" in glyph.attrib:
         horiz_adv_x = glyph.attrib["horiz-adv-x"]
      path.set("horiz-adv-x", horiz_adv_x)
      path.set("transform", "scale(1,-1)")
      if "d" in glyph.attrib:
         path.set("d", glyph.attrib["d"])
print(etree.tostring(root, pretty_print=True, encoding='unicode'))
