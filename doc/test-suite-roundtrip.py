# This script it expected to be run from ./bindings/python
import argparse
import json
import os
import sys

import lxml.etree as etree
from xmldiff.main import diff_trees as xmldiff

# Add path for toolkit built in-place
sys.path.append('.')
import verovio

ns = {'mei': 'http://www.music-encoding.org/ns/mei'}

# Optional list for processing only listed test files
# Files must be listed in a file passed with --shortlist, one by line
# Ex. 'accid/accid-001.mei'
shortlist: set | None = None

test_options = {
    'breaks': 'auto',
    'removeIds': True,
    "xmlIdChecksum": True
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
        print(f"Processing {args.shortlist} shortlist file")
        with open(args.shortlist) as f:
            shortlist = set(f.read().splitlines())

    total_changes = 0
    log = []

    test_suite_fpath = args.test_suite_dir.replace(r"\ ", " ")
    output_dir_fpath = args.output_dir.replace(r"\ ", " ")
    path_out = args.output_dir
    test_suite_directories = sorted(os.listdir(test_suite_fpath))

    for test_file_directory in test_suite_directories:
        testing_files_path = os.path.join(test_suite_fpath, test_file_directory)
        if not (os.path.isdir(testing_files_path)):
            continue

        # create the output directory if necessary
        output_files_path = os.path.join(output_dir_fpath, test_file_directory)
        if not (os.path.isdir(output_files_path)):
            os.mkdir(output_files_path)

        testing_files = sorted(os.listdir(testing_files_path))

        for test_file in testing_files:
            if shortlist and os.path.join(test_file_directory, test_file) not in shortlist:
                continue

            # skip directories
            if not (os.path.isfile(os.path.join(test_suite_fpath, test_file_directory, test_file))):
                continue

            # skip hidden files
            if test_file.startswith('.'):
                continue

            name, ext = os.path.splitext(test_file)
            if ext != ".mei":
                continue

            # filenames (input MEI/XML and output SVG)
            input_file = os.path.join(test_suite_fpath, test_file_directory, test_file)
            verovio_output_file = os.path.join(output_dir_fpath, test_file_directory, f"{name}.mei")

            # parse the MEI file
            original_xml = etree.parse(input_file).getroot()

            toskip = original_xml.find(
                ".//mei:appInfo/mei:application[@type='skip-round-trip']", namespaces=ns)
            if toskip is not None:
                print(f"Skipping {test_file}")
                continue

            print(f"Evaluating {test_file}")

            # make a local copy of the options
            options = test_options.copy()

            # try to get the extMeta tag and load the options if existing
            meta = original_xml.findtext(
                ".//mei:meiHead/mei:extMeta", namespaces=ns)
            if meta is not None and meta != '':
                print(meta)
                metaOptions = json.loads(meta)
                options |= metaOptions

            tk.setOptions(options)
            tk.loadFile(input_file)
            # round-trip to MEI
            verovio_output_string = tk.getMEI({})
            verovio_xml = etree.fromstring(verovio_output_string.encode("utf-8"))
            etree.ElementTree(verovio_xml).write(verovio_output_file)

            diff = xmldiff(original_xml, verovio_xml)
            difflen: int = len(diff)
            if difflen > 0:
                log.append(f"****** {test_file}: {difflen} ******")
                total_changes += 1
                print(f"::warning round-trip on {test_file} produced a different MEI ({difflen} nodes)")

            # Clear the options for the next round.
            options = None

    if total_changes > 0:
        with open(os.path.join(path_out, 'log.md'), 'a') as f:
            f.write(f"\n\n{f"{total_changes} round-trip change(s) detected"}\n")
            f.writelines((f"{ll}\n" for ll in log))
