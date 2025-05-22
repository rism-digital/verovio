# This script it expected to be run from ./bindings/python
import argparse
import json
import os
import sys
import xml.etree.ElementTree as ET

import lxml.etree as etree
from xmldiff.main import diff_trees as xmldiff

# Add path for toolkit built in-place
sys.path.append('.')
import verovio

ns = {'mei': 'http://www.music-encoding.org/ns/mei'}

ET.register_namespace('', 'http://www.music-encoding.org/ns/mei')

# Optional list for processing only listed test files
# Files must be listed in a file passed with --shortlist, one by line
# Ex. 'accid/accid-001.mei'
shortlist = []

testOptions = {
    'breaks': 'auto',
    'removeIds': True
}

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('test_suite_dir')
    parser.add_argument('input_dir2')
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

    totalChanges = 0
    log = []

    path1 = args.test_suite_dir.replace(r"\ ", " ")
    path2 = args.output_dir.replace(r"\ ", " ")
    path_out = args.output_dir
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
            print(f'Evaluating {item2}')
            name, ext = os.path.splitext(item2)
            meiFile = os.path.join(path2, item1, name + '.mei')

            # parse the MEI file
            if ext != '.mei':
                continue

            tree = ET.parse(inputFile)
            root = tree.getroot()

            toskip = root.find(
                './/mei:appInfo/mei:application[@type]', namespaces=ns)
            if toskip is not None and toskip.attrib['type'] == 'skip-round-trip':
                print(f'Skipping {item2}')
                continue

            # try to get the extMeta tag and load the options if existing
            meta = root.findtext(
                './/mei:meiHead/mei:extMeta', namespaces=ns)
            if meta is not None and meta != '':
                print(meta)
                metaOptions = json.loads(meta)
                options |= metaOptions

            tk.setOptions(options)
            tk.loadFile(inputFile)
            # round-trip to MEI
            meiString = tk.getMEI({})
            #print(meiString)
            ET.ElementTree(ET.fromstring(meiString)).write(meiFile)
            options.clear()

            tree1 = etree.parse(inputFile)
            root1 = tree1.getroot()


            root2 = etree.fromstring(meiString.encode('utf-8'))

            diff = xmldiff(root1, root2)
            if (len(diff) > 0):
                log.append("****** {}: {} ******".format(item2, len(diff)))
                totalChanges += 1
                print(f'::warning round-trip on {item2} produced a different MEI ({len(diff)} nodes)')

    text1 = '{} round-trip change(s) detected'.format(totalChanges)

    if (totalChanges > 0):
        logFileOut = os.path.join(path_out, 'log.md')
        with open(logFileOut, 'a') as f:
            f.write("\n\n%s\n" % text1)
            for item in log:
                f.write("%s\n" % item)
