# -- coding: utf-8 --

# Copyright (c) 2011-2022 Andrew Hankinson, Alastair Porter, and Others

# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:

# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import logging
import shutil
import sys
from argparse import ArgumentParser
from pathlib import Path

import yaml

import cpp
from schema import MeiSchema

logging.basicConfig(format="[%(asctime)s] [%(levelname)8s] %(message)s (%(filename)s:%(lineno)s)")
log = logging.getLogger("schemaparser")


def main(configure: dict) -> bool:
    with open(configure["compiled"], "r") as mei_source:
        resolve_elements = configure["elements"]
        schema = MeiSchema(mei_source, resolve_elements=resolve_elements)

    output_directory: Path = Path(configure["output_dir"])
    if output_directory.exists():
        log.debug("Removing old Verovio C++ output directory")
        shutil.rmtree(output_directory)
    output_directory.mkdir(exist_ok=True)

    cpp.create(schema, configure)

    return True


if __name__ == "__main__":
    p = ArgumentParser(usage="%(prog)s [-c config path] [compiled odd path]")

    p.add_argument("compiled", help="A compiled ODD file", type=Path)
    p.add_argument("-c", "--config", default="./config.yml", help="Path to a config file", type=Path)

    args = p.parse_args()

    if not args.config.exists():
        log.error("Could not find a config file at %s", args.config)
        sys.exit(1)

    config: dict = yaml.safe_load(open(args.config, "r"))
    config["compiled"] = args.compiled

    if config["debug"]:
        log.setLevel(logging.DEBUG)
    else:
        log.setLevel(logging.WARN)

    res: bool = main(config)
    if not res:
        sys.exit(1)
