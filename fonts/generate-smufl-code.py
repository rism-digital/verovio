#! /usr/bin/env python3

import os
import xml.etree.ElementTree as ET

# Define file output paths
smufl_h_path = '../include/vrv/smufl.h'
smufl_cpp_path = '../src/smufl.cpp'

######################
#  Helper Functions  #
######################

# Open file in same directory as script and retrieve its content


def get_file_content(filepath):
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), 'r')
    content = file.read()
    file.close()
    return content

# Write content to file with path relative to the script directory


def write_file_content(filepath, content):
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), 'w')
    file.write(content)
    file.close()

# Retrieve all elements with given tag name from xml


def get_elements(xml, tag):
    root = ET.fromstring(bytes(xml, encoding='utf-8'))
    return root.findall('.//' + tag)  # XPath, recursive

#########################
#  Parse supported.xsl  #
#########################

# Retrieve dictionary mapping glyph code to smufl name


def get_supported_glyph_codes():
    xml = get_file_content('supported.xsl')
    glyphs = get_elements(xml, 'glyph')
    result = {}
    for glyph in glyphs:
        result[glyph.attrib["glyph-code"]] = glyph.attrib["smufl-name"]
    return result

##################
#  Main program  #
##################


if __name__ == '__main__':
    smufl_names = get_supported_glyph_codes()

    # (1) Create header file
    smufl_h_content = get_file_content('smufl.h.inc') + '\n'
    smufl_h_content += 'enum {\n'
    for glyph_code in smufl_names:
        # SMUFL_E000_brace = 0xE000,
        smufl_h_content += ' ' * 4 + f"SMUFL_{glyph_code}_{smufl_names[glyph_code]} = 0x{glyph_code}," + '\n'
    smufl_h_content += '};\n\n'
    smufl_h_content += '/** The number of glyphs for verification **/\n'
    smufl_h_content += f"#define SMUFL_COUNT {len(smufl_names)}" + '\n\n'
    smufl_h_content += '} // namespace vrv\n\n'
    smufl_h_content += '#endif\n'
    write_file_content(smufl_h_path, smufl_h_content)

    # (2) Create cpp file
    smufl_cpp_content = get_file_content('smufl.cpp.inc') + '\n'
    smufl_cpp_content += 'const std::map<std::string, wchar_t> Resources::s_smuflNames = {\n'
    for glyph_code in smufl_names:
        # { "brace", SMUFL_E000_brace },
        smufl_cpp_content += ' ' * 4 + \
            f"{{ \"{smufl_names[glyph_code]}\", SMUFL_{glyph_code}_{smufl_names[glyph_code]} }}," + '\n'
    smufl_cpp_content += '};\n\n'
    smufl_cpp_content += '/** The number of glyphs for verification **/\n'
    smufl_cpp_content += f"// {len(smufl_names)}" + '\n\n'
    smufl_cpp_content += '} // namespace vrv\n'
    write_file_content(smufl_cpp_path, smufl_cpp_content)
