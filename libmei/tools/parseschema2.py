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

import sys

if sys.version_info < (3, 4):
    raise Exception("requires python 3.4")

import codecs
import logging
import re
import shutil
from argparse import ArgumentParser
from pathlib import Path
from typing import List

from lxml import etree

lg = logging.getLogger('schemaparser')
f = logging.Formatter(
    "%(levelname)s %(asctime)s On Line: %(lineno)d %(message)s")
h = logging.StreamHandler()
h.setFormatter(f)

lg.setLevel(logging.DEBUG)
lg.addHandler(h)

# globals
TEI_NS = {"tei": "http://www.tei-c.org/ns/1.0"}
TEI_RNG_NS = {"tei": "http://www.tei-c.org/ns/1.0",
              "rng": "http://relaxng.org/ns/structure/1.0"}
NAMESPACES = {'xml': 'http://www.w3.org/XML/1998/namespace',
              'xlink': 'http://www.w3.org/1999/xlink'}


class MeiSchema(object):
    def __init__(self, oddfile):
        parser = etree.XMLParser(resolve_entities=True)
        self.schema = etree.parse(oddfile, parser)
        # self.customization = etree.parse(customization_file)

        self.active_modules = []  # the modules active in the resulting output
        self.element_structure = {}  # the element structure.
        self.attribute_group_structure = {}  # the attribute group structure
        # inverted, so we can map attgroups to modules
        self.inverse_attribute_group_structure = {}
        # holding data types and data lists
        self.data_types = {}
        self.data_lists = {}

        self.get_elements()
        self.get_attribute_groups()
        self.get_data_types_and_lists()
        self.invert_attribute_group_structure()
        self.set_active_modules()

        # lg.debug(self.data_lists)

    def get_elements(self):
        """Retrieves all defined elements from the schema."""
        elements = [m for m in self.schema.xpath(
            "//tei:elementSpec", namespaces=TEI_NS)]
        for element in elements:
            modname = element.get("module").split(".")[-1]

            if modname not in self.element_structure.keys():
                self.element_structure[modname] = {}

            element_name = element.get("ident")
            memberships = []

            element_membership = element.xpath(
                "./tei:classes/tei:memberOf", namespaces=TEI_NS)
            for member in element_membership:
                if member.get("key").split(".")[0] != "att":
                    # skip the models that this element might be a member of
                    continue
                self.__get_membership(member, memberships)

            # memberships.kesort()
            self.element_structure[modname][element_name] = memberships

            # need a way to keep self-defined attributes:
            selfattributes = []
            attdefs = element.xpath(
                "./tei:attList/tei:attDef", namespaces=TEI_NS)
            if attdefs:
                for attdef in attdefs:
                    if attdef.get("ident") == "id":
                        continue
                    attname = self.__process_att(attdef)
                    selfattributes.append(attname)

                self.element_structure[modname][element_name].append(
                    selfattributes)

    def get_attribute_groups(self):
        """Retrieves all defined attribute classes from the schema."""
        attribute_groups = [m for m in self.schema.xpath(
            "//tei:classSpec[@type=$at]", at="atts", namespaces=TEI_NS)]
        for group in attribute_groups:
            group_name = group.get("ident")

            if group_name == "att.id":
                continue

            group_module = group.get("module").split(".")[-1]
            attdefs = group.xpath(
                "./tei:attList/tei:attDef", namespaces=TEI_NS)
            if not attdefs:
                continue

            if group_module not in self.attribute_group_structure.keys():
                self.attribute_group_structure[group_module] = {}

            self.attribute_group_structure[group_module][group_name] = []
            for attdef in attdefs:
                if attdef.get("ident") == "id":
                    continue
                attname = self.__process_att(attdef)
                self.attribute_group_structure[group_module][group_name].append(
                    attname)

    def get_data_types_and_lists(self):

        compoundalternate = [m for m in self.schema.xpath(
            "//tei:macroSpec[@type=\"dt\" and .//tei:alternate[@minOccurs=\"1\" and @maxOccurs=\"1\"]]", namespaces=TEI_RNG_NS)]
        for ct in compoundalternate:
            #lg.debug("TYPE - {0}".format(ct.get("ident")))
            data_type = ct.get("ident")
            self.data_types[data_type] = []
            subtypes = [m for m in ct.findall(
                ".//tei:alternate/tei:macroRef", namespaces=TEI_RNG_NS)]
            for st in subtypes:
                #lg.debug("SUBTYPE - {0}".format(st.get("name")))
                subtype = st.xpath("//tei:macroSpec[@ident=\"{0}\"]//tei:valList/tei:valItem".format(
                    st.get("key")), namespaces=TEI_RNG_NS)
                for v in subtype:
                    # lg.debug("\t{0}".format(v.get("ident")))
                    type_value = v.get("ident")
                    self.data_types[data_type].append(type_value)
            if len(self.data_types[data_type]) == 0:
                del self.data_types[data_type]
                #lg.debug("REMOVE {0}".format(data_type))

        compoundchoice = [m for m in self.schema.xpath(
            "//tei:macroSpec[@type=\"dt\" and .//rng:choice]|//tei:dataSpec[.//rng:choice]", namespaces=TEI_RNG_NS)]
        for ct in compoundchoice:
            #lg.debug("TYPE - {0}".format(ct.get("ident")))
            data_type = ct.get("ident")
            self.data_types[data_type] = []
            subtypes = [m for m in ct.xpath(
                ".//rng:choice/rng:ref", namespaces=TEI_RNG_NS)]
            for st in subtypes:
                #lg.debug("SUBTYPE - {0}".format(st.get("name")))
                subtype = st.xpath("//tei:macroSpec[@ident=\"{0}\"]//tei:valList/tei:valItem|//tei:dataSpec[@ident=\"{0}\"]//tei:valList/tei:valItem".format(
                    st.get("name")), namespaces=TEI_RNG_NS)
                for v in subtype:
                    # lg.debug("\t{0}".format(v.get("ident")))
                    type_value = v.get("ident")
                    self.data_types[data_type].append(type_value)
            if len(self.data_types[data_type]) == 0:
                del self.data_types[data_type]
                #lg.debug("REMOVE {0}".format(data_type))

        types = [m for m in self.schema.xpath(
            "//tei:macroSpec[.//tei:valList[@type=\"closed\" or @type=\"semi\"]]|//tei:dataSpec[.//tei:valList[@type=\"closed\" or @type=\"semi\"]]", namespaces=TEI_RNG_NS)]
        for t in types:
            #lg.debug("TYPE - {0}".format(t.get("ident")))
            data_type = t.get("ident")
            self.data_types[data_type] = []
            values = t.findall(".//tei:valList/tei:valItem",
                               namespaces=TEI_RNG_NS)
            for v in values:
                # lg.debug("\t{0}".format(v.get("ident")))
                type_value = v.get("ident")
                self.data_types[data_type].append(type_value)

        vallists = [m for m in self.schema.xpath(
            "//tei:valList[@type=\"closed\" or @type=\"semi\"]", namespaces=TEI_RNG_NS)]
        for vl in vallists:
            element = vl.xpath("./ancestor::tei:classSpec",
                               namespaces=TEI_RNG_NS)
            attName = vl.xpath("./parent::tei:attDef/@ident",
                               namespaces=TEI_RNG_NS)
            # if ($current.valList/ancestor::tei:classSpec) then($current.valList/ancestor::tei:classSpec/@ident) else($current.valList/ancestor::tei:elementSpec/@ident
            if element:
                #lg.debug("VALLIST - ELEMEMT {0} --- {1}".format(element[0].get("ident"),attName[0]))
                data_list = "{0}@{1}".format(
                    element[0].get("ident"), attName[0])
                self.data_lists[data_list] = []
                self.data_lists[data_list]
                values = vl.xpath(".//tei:valItem", namespaces=TEI_RNG_NS)
                for v in values:
                    # lg.debug("\t{0}".format(v.get("ident")))
                    list_value = v.get("ident")
                    self.data_lists[data_list].append(list_value)
            # elif attName:
            #    elName = vl.xpath("./ancestor::tei:elementSpec/@ident", namespaces=TEI_RNG_NS)
            #    lg.debug("VALLIST {0} --- {1}".format(elName[0],attName[0]))
            #    data_list = "{0}.{1}".format(elName[0],attName[0])
            #    self.data_lists[data_list] = []
            #    values = vl.xpath(".//tei:valItem", namespaces=TEI_RNG_NS)
            #    for v in values:
            #        lg.debug("\t{0}".format(v.get("ident")))
            #        list_value = v.get("ident")
            #        self.data_lists[data_list].append(list_value)

    def invert_attribute_group_structure(self):
        for module, groups in self.attribute_group_structure.items():
            for attgroup in groups:
                self.inverse_attribute_group_structure[attgroup] = module

    def set_active_modules(self):
        self.active_modules = list(self.element_structure.keys())
        self.active_modules.sort()

    def __process_att(self, attdef: etree.Element) -> str:
        """Process attribute definition."""
        attname = ""
        attdefident = attdef.get("ident")
        if "-" in attdefident:
            f, l = attdefident.split("-")
            attdefident = f"{f}{l.title()}"

        if attdef.get("ns"):
            attname = f"{attdef.get('ns')}|{attdefident}"
        elif ":" in attdefident:
            pfx, att = attdefident.split(":")
            attname = f"{NAMESPACES[pfx]}|{att}"
        else:
            attname = f"{attdefident}"

        return attname

    def __get_membership(self, member: etree.Element, resarr: List[str]) -> None:
        """Get attribute groups."""
        member_attgroup = member.xpath(
            "//tei:classSpec[@type=$att][@ident=$nm]", att="atts", nm=member.get("key"), namespaces=TEI_NS)

        if member_attgroup:
            member_attgroup = member_attgroup[0]
        else:
            return

        if member_attgroup.xpath("./tei:attList/tei:attDef", namespaces=TEI_NS):
            if member_attgroup.get("ident") == "att.id":
                return
            resarr.append(member_attgroup.get("ident"))
        m2s = member_attgroup.xpath(
            "./tei:classes/tei:memberOf", namespaces=TEI_NS)

        for mship in m2s:
            self.__get_membership(mship, resarr)

    def strpatt(self, string: str) -> str:
        """Returns a version of the string with any leading att. stripped."""
        return string.replace("att.", "")

    def strpdot(self, string: str) -> str:
        """Returns a version of the string without any dots."""
        return "".join([n for n in string.split(".")])

    def cc(self, att_name: str) -> str:
        """Returns a CamelCasedName version of attribute.case.names."""
        return "".join([n[0].upper() + n[1:] for n in att_name.split(".")])

    def get_att_desc(self, att_name: str) -> str:
        """Returns the documentation string for an attribute by name."""
        desc = self.schema.find(
            f"//tei:attDef[@ident='{att_name}']/tei:desc", namespaces=TEI_NS)
        if desc is not None:
            # strip extraneous whitespace
            return re.sub("[\s\t]+", " ", desc.xpath("string()"))
        else:
            return ""

    def get_elem_desc(self, elem_name: str) -> str:
        """Returns the documentation string for an element by name."""
        desc = self.schema.find(
            f"//tei:elementSpec[@ident='{elem_name}']/tei:desc", namespaces=TEI_NS)
        if desc is not None:
            # strip extraneous whitespace
            return re.sub("[\s\t]+", " ", desc.xpath("string()"))
        else:
            return ""


if __name__ == "__main__":
    # Custom usage message to show user [compiled] should go before all other flags
    p = ArgumentParser(
        usage='%(prog)s [compiled | -sl] [-e] [-h] [-df DEFINE] [-o OUTDIR] [-d]')
    exclusive_group = p.add_mutually_exclusive_group()
    # Due to nargs="?", "compiled" will appear as optional and not positional
    exclusive_group.add_argument(
        "compiled", help="A compiled ODD file", nargs="?")
    p.add_argument("-o", "--outdir", default=".", help="output directory")
    p.add_argument("-d", "--debugging",
                   help="Run with verbose output", action="store_true")
    p.add_argument("-e", "--elements",
                   help="Output element classes", action="store_true")
    p.add_argument("-ns", "--namespace", default="vrv", help="C++ output namespace")

    args = p.parse_args()

    compiled_odd = Path(args.compiled)

    mei_source = codecs.open(compiled_odd, 'r', 'utf-8')
    # sf = codecs.open(args.source,'r', "utf-8")
    # cf = codecs.open(args.customization, 'r', "utf-8")
    outdir = Path(args.outdir)

    outdir.mkdir(exist_ok=True)

    schema = MeiSchema(mei_source)

    import generatecode
    output_directory = Path(outdir, "dist")
    if output_directory.exists():
        lg.debug("Removing old Verovio C++ output directory")
        shutil.rmtree(output_directory)
    output_directory.mkdir()
    generatecode.create(args.namespace, schema, output_directory, args.elements)

    mei_source.close()

    sys.exit(0)
