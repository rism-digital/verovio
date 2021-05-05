#! /usr/bin/env python3

import os
from lxml import etree

# Open file in same directory as script and retrieve its content
def get_file_content(filename):
   __location__ = os.path.realpath(os.path.dirname(__file__))
   file = open(os.path.join(__location__, filename), 'r')
   content = file.read()
   file.close()
   return content

# Retrieve all elements with given tag name from xml
def get_elements(xml, tag):
   root = etree.fromstring(bytes(xml, encoding='utf-8'))
   return root.findall('.//' + tag)  # XPath, recursive


##################
#  Main program  #
##################

# (1) Print header
print(get_file_content('smufl.h.inc'))

# (2) Print smufl-enum
print('enum {')
xml = get_file_content('supported.xsl')
glyphs = get_elements(xml, 'glyph')
for glyph in glyphs:
   code = glyph.get('glyph-code')
   name = glyph.get('smufl-name')
   print(" " * 4 + f"SMUFL_{code}_{name} = 0x{code},")  # SMUFL_E000_brace = 0xE000,
print('};\n')

# (3) Print footer
print('/** The number of glyphs for verification **/')
print(f'#define SMUFL_COUNT {len(glyphs)}\n')
print('} // namespace vrv\n')
print('#endif')
