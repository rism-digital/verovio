# This script it expected to be run from ./bindings/python
import argparse
import json
import os
import sys
import xml.etree as etree
import xml.etree.ElementTree as ET

# Add path for toolkit built in-place
sys.path.append('.')
import verovio

ns = {'mei': 'http://www.music-encoding.org/ns/mei'}

testOptions = {
    'adjustPageHeight': True,
    'breaks': 'auto',
    'pageHeight': 2970,
    'pageWidth': 2100,
    'header': 'none',
    'footer': 'none',
    'scale': 40,
    'spacingStaff': 4
}

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("test_suite_dir")
    parser.add_argument("output_dir")
    args = parser.parse_args()

    tk = verovio.toolkit(False)
    # version of the toolkit
    print(tk.getVersion())

    # keep all the options to be able to reset them for each test
    defaultOptions = json.loads(tk.getOptions(True))

    tk.setResourcePath('../../data')

    path1 = args.test_suite_dir
    path2 = args.output_dir
    dir1 = sorted(os.listdir(path1))
    for item1 in dir1:
        if not(os.path.isdir(os.path.join(path1, item1))):
            continue

        # create the output directory if necessary
        if not(os.path.isdir(os.path.join(path2, item1))):
            os.mkdir(os.path.join(path2, item1))

        dir2 = sorted(os.listdir(os.path.join(path1, item1)))
        for item2 in dir2:
            # skip directories
            if not(os.path.isfile(os.path.join(path1, item1, item2))):
                continue
            # skip hidden files
            if item2.startswith('.'):
                continue

            # reset the options
            options = {**defaultOptions, **testOptions}

            # filenames (input MEI and output SVG)
            meiFile = os.path.join(path1, item1, item2)
            name, ext = os.path.splitext(item2)
            svgFile = os.path.join(path2, item1, name + '.svg')

            # parse the MEI file
            tree = ET.parse(meiFile)
            root = tree.getroot()
            # try to get the extMeta tag and load the options if existing
            meta = root.findtext(".//mei:meiHead/mei:extMeta", namespaces=ns)
            if (meta != None and meta != ''):
                print(meta)
                metaOptions = json.loads(meta)
                options = {**options, **metaOptions}

            tk.setOptions(json.dumps(options))
            tk.loadFile(meiFile)
            tk.renderToSVGFile(svgFile, 1)
