# This script it expected to be run from ./bindings/python
import argparse
import json
import os
import sys
import xml.etree.ElementTree as ET

from cairosvg import svg2png

# Add path for toolkit built in-place
sys.path.append('.')
import verovio

ns = {'mei': 'http://www.music-encoding.org/ns/mei'}

# Optional list for processing only listed test files
# Files must be listed in a file passed with --shortlist, one by line
# Ex. 'accid/accid-001.mei'
shortlist = []

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

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('test_suite_dir')
    parser.add_argument('output_dir')
    parser.add_argument('--shortlist', nargs='?', default='')
    args = parser.parse_args()

    # version of the toolkit
    tk = verovio.toolkit(False)
    print(f'Verovio {tk.getVersion()}')

    tk.setResourcePath('../../data')

    # look if we have a shortlist file and read it
    if len(args.shortlist) > 0:
        print(args.shortlist)
        with open(args.shortlist) as f:
            for line in f:
                shortlist.append(line.strip('\n'))
                print('File {} added to the shortlist'.format(line))

    path1 = args.test_suite_dir.replace("\ ", " ")
    path2 = args.output_dir.replace("\ ", " ")
    dir1 = sorted(os.listdir(path1))
    for item1 in dir1:
        if not (os.path.isdir(os.path.join(path1, item1))):
            continue

        # create the output directory if necessary
        if not (os.path.isdir(os.path.join(path2, item1))):
            os.mkdir(os.path.join(path2, item1))

        dir2 = sorted(os.listdir(os.path.join(path1, item1)))
        for item2 in dir2:
            # skip directories
            if not (os.path.isfile(os.path.join(path1, item1, item2))):
                continue
            # skip hidden files
            if item2.startswith('.'):
                continue

            if shortlist and not (os.path.join(item1, item2) in shortlist):
                continue

            # reset the options
            options = testOptions.copy()

            # filenames (input MEI/XML and output SVG)
            inputFile = os.path.join(path1, item1, item2)
            options.update({"xmlIdChecksum": True})
            print(f'Rendering {item2}')
            name, ext = os.path.splitext(item2)
            svgFile = os.path.join(path2, item1, name + '.svg')
            pngFile = os.path.join(path2, item1, name + '.png')
            timeMapFile = os.path.join(path2, item1, name + '.json')

            # parse the MEI file
            if ext == '.mei':
                tree = ET.parse(inputFile)
                root = tree.getroot()
                # try to get the extMeta tag and load the options if existing
                meta = root.findtext(
                    './/mei:meiHead/mei:extMeta', namespaces=ns)
                if meta is not None and meta != '':
                    print(meta)
                    metaOptions = json.loads(meta)
                    options |= metaOptions

            tk.setOptions(options)
            tk.loadFile(inputFile)
            # render to SVG
            svgString = tk.renderToSVG(1)
            svgString = svgString.replace(
                "overflow=\"inherit\"", "overflow=\"visible\"")
            ET.ElementTree(ET.fromstring(svgString)).write(svgFile)
            svg2png(bytestring=svgString, scale=2, write_to=pngFile)
            # create time map
            tk.renderToTimemapFile(timeMapFile)
            tk.resetOptions()
            options.clear()
