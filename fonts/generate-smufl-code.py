#! /usr/bin/env python3

import os
import xml.etree.ElementTree as ET

# Define file output paths
SMUFL_HEADER = "../include/vrv/smufl.h"
SMUFL_SOURCE = "../src/smufl.cpp"

######################
#  Helper Functions  #
######################

# Open file in same directory as script and retrieve its content


def get_file_content(filepath) -> str:
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), "r")
    content = file.read()
    file.close()
    return content

# Write content to file with path relative to the script directory


def write_file_content(filepath, content):
    location = os.path.realpath(os.path.dirname(__file__))
    file = open(os.path.join(location, filepath), "w")
    file.write(content)
    file.close()

#########################
#  Parse supported.xsl  #
#########################

# Retrieve dictionary mapping glyph code to smufl name


def get_supported_glyph_codes() -> dict:
    """Retrieve dictionary with supported SMuFL codepoints and name."""
    supported_xsl = ET.parse("supported.xsl")
    glyphs = supported_xsl.findall(".//glyph")
    supported_glyphs = {}
    for glyph in glyphs:
        supported_glyphs[glyph.attrib["glyph-code"]
                         ] = glyph.attrib["smufl-name"]
    return supported_glyphs

##################
#  Main program  #
##################


if __name__ == "__main__":
    smufl_names = get_supported_glyph_codes()

    # (1) Create header file
    smufl_h_content = get_file_content("smufl.h.inc") + "\n"
    smufl_h_content += "enum {\n"
    for glyph_code in smufl_names:
        # SMUFL_E000_brace = 0xE000,
        smufl_h_content += " " * 4 + \
            f"SMUFL_{glyph_code}_{smufl_names[glyph_code]} = 0x{glyph_code}," + "\n"
    smufl_h_content += "};\n\n"
    smufl_h_content += "/** The number of glyphs for verification **/\n"
    smufl_h_content += f"#define SMUFL_COUNT {len(smufl_names)}" + "\n\n"
    smufl_h_content += "} // namespace vrv\n\n"
    smufl_h_content += "#endif\n"
    write_file_content(SMUFL_HEADER, smufl_h_content)

    # (2) Create cpp file
    smufl_cpp_content = get_file_content("smufl.cpp.inc") + "\n"
    smufl_cpp_content += "const std::map<std::string, wchar_t> Resources::s_smuflNames = {\n"
    for glyph_code in smufl_names:
        # { "brace", SMUFL_E000_brace },
        smufl_cpp_content += " " * 4 + \
            f"{{ \"{smufl_names[glyph_code]}\", SMUFL_{glyph_code}_{smufl_names[glyph_code]} }}," + "\n"
    smufl_cpp_content += "};\n\n"
    smufl_cpp_content += "/** The number of glyphs for verification **/\n"
    smufl_cpp_content += f"// {len(smufl_names)}" + "\n\n"
    smufl_cpp_content += "} // namespace vrv\n"
    write_file_content(SMUFL_SOURCE, smufl_cpp_content)
