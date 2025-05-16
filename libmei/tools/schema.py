import logging
import re
from re import Pattern

from lxml import etree

logging.basicConfig(format="[%(asctime)s] [%(levelname)8s] %(message)s (%(filename)s:%(lineno)s)")
log = logging.getLogger("schemaparser")


# globals
TEI_NS = {"tei": "http://www.tei-c.org/ns/1.0"}
TEI_RNG_NS = {"tei": "http://www.tei-c.org/ns/1.0", "rng": "http://relaxng.org/ns/structure/1.0"}
NAMESPACES = {"xml": "http://www.w3.org/XML/1998/namespace", "xlink": "http://www.w3.org/1999/xlink"}
WHITESPACE_REGEX: Pattern = re.compile(r"[\s\t]+")


class MeiSchema(object):
    def __init__(self, oddfile, resolve_elements=False):
        parser = etree.XMLParser(resolve_entities=True)
        self.schema = etree.parse(oddfile, parser)
        # self.customization = etree.parse(customization_file)

        self.active_modules = []  # the modules active in the resulting output
        self.element_structure = {}  # the element structure

        self.attribute_group_structure = {}  # the attribute group structure
        # inverted, so we can map attgroups to modules
        self.inverse_attribute_group_structure = {}
        # holding data types and data lists
        self.data_types = {}
        self.data_lists = {}

        # processing the elements takes a long time, so only do it when needed
        if resolve_elements:
            self.get_elements()

        self.get_attribute_groups()
        self.get_data_types_and_lists()
        self.invert_attribute_group_structure()
        self.set_active_modules()
        # lg.debug(self.data_lists)

    def get_elements(self):
        """
        Retrieve all defined elements from the schema.
        """
        elements: list[etree.Element] = self.schema.xpath("//tei:elementSpec", namespaces=TEI_NS)

        for element in elements:
            mod_name: str = element.get("module").split(".")[-1]

            if mod_name not in self.element_structure:
                self.element_structure[mod_name] = {}

            element_name = element.get("ident")
            memberships = []

            element_membership = element.xpath("./tei:classes/tei:memberOf", namespaces=TEI_NS)
            for member in element_membership:
                if member.get("key").split(".")[0] != "att":
                    # skip the models that this element might be a member of
                    continue

                self.__get_membership(member, memberships)

            self.element_structure[mod_name][element_name] = memberships

            # need a way to keep self-defined attributes:
            selfattributes = []
            attdefs: list[etree.Element] = element.xpath("./tei:attList/tei:attDef", namespaces=TEI_NS)
            if not attdefs:
                continue

            for attdef in attdefs:
                if attdef.get("ident") == "id":
                    continue
                att_name: str = self.__process_att(attdef)
                selfattributes.append(att_name)

            self.element_structure[mod_name][element_name].append(selfattributes)

    def get_attribute_groups(self):
        """
        Retrieve all defined attribute classes from the schema.
        """
        attribute_groups: list[etree.Element] = self.schema.xpath(".//tei:classSpec[@type='atts']", namespaces=TEI_NS)
        for group in attribute_groups:
            group_name: str = group.get("ident")

            if group_name == "att.id":
                continue

            group_module = group.get("module").split(".")[-1]
            attdefs = group.xpath("./tei:attList/tei:attDef", namespaces=TEI_NS)
            if not attdefs:
                continue

            if group_module not in self.attribute_group_structure.keys():
                self.attribute_group_structure[group_module] = {}

            self.attribute_group_structure[group_module][group_name] = []
            for attdef in attdefs:
                if attdef.get("ident") == "id":
                    continue
                attname = self.__process_att(attdef)
                self.attribute_group_structure[group_module][group_name].append(attname)

    def get_data_types_and_lists(self):
        """
        Parse data types from the schema.
        """
        compound_alternate = self.schema.xpath(
            ".//tei:macroSpec[@type='dt' and .//tei:alternate[@minOccurs='1' and @maxOccurs='1']]",
            namespaces=TEI_RNG_NS,
        )

        for ct in compound_alternate:
            data_type = ct.get("ident")
            subtypes = ct.findall(".//tei:alternate/tei:macroRef", namespaces=TEI_RNG_NS)

            for st in subtypes:
                subtype = self.schema.xpath(
                    ".//tei:macroSpec[@ident=$st_ident]//tei:valList/tei:valItem",
                    st_ident=st.get("key"),
                    namespaces=TEI_RNG_NS,
                )
                for v in subtype:
                    if data_type not in self.data_types:
                        self.data_types[data_type] = []
                    self.data_types[data_type].append(v.get("ident"))

        compound_choice = self.schema.xpath(
            ".//tei:macroSpec[@type='dt' and .//rng:choice]|//tei:dataSpec[.//rng:choice]", namespaces=TEI_RNG_NS
        )

        for ct in compound_choice:
            data_type = ct.get("ident")
            subtypes = ct.xpath(".//rng:choice/rng:ref", namespaces=TEI_RNG_NS)

            for st in subtypes:
                subtype = st.xpath(
                    "//tei:macroSpec[@ident=$st_ident]//tei:valList/tei:valItem|//tei:dataSpec[@ident=$st_ident]//tei:valList/tei:valItem",
                    st_ident=st.get("name"),
                    namespaces=TEI_RNG_NS,
                )

                for v in subtype:
                    if data_type not in self.data_types:
                        self.data_types[data_type] = []
                    self.data_types[data_type].append(v.get("ident"))

        types = self.schema.xpath(
            ".//tei:macroSpec[.//tei:valList[@type='closed' or @type='semi']]|//tei:dataSpec[.//tei:valList[@type='closed' or @type='semi']]",
            namespaces=TEI_RNG_NS,
        )
        for t in types:
            data_type = t.get("ident")
            values = t.findall(".//tei:valList/tei:valItem", namespaces=TEI_RNG_NS)
            for v in values:
                if data_type not in self.data_types:
                    self.data_types[data_type] = []
                self.data_types[data_type].append(v.get("ident"))

        vallists = self.schema.xpath("//tei:valList[@type='closed' or @type='semi']", namespaces=TEI_RNG_NS)
        for vl in vallists:
            element = vl.xpath("./ancestor::tei:classSpec", namespaces=TEI_RNG_NS)
            if not element:
                continue

            att_name = vl.xpath("./parent::tei:attDef/@ident", namespaces=TEI_RNG_NS)

            data_list = f"{element[0].get('ident')}@{att_name[0]}"
            values = vl.xpath(".//tei:valItem", namespaces=TEI_RNG_NS)

            for v in values:
                if data_list not in self.data_lists:
                    self.data_lists[data_list] = []
                self.data_lists[data_list].append(v.get("ident"))

    def invert_attribute_group_structure(self):
        for module, groups in self.attribute_group_structure.items():
            for attgroup in groups:
                self.inverse_attribute_group_structure[attgroup] = module

    def set_active_modules(self):
        self.active_modules = list(self.element_structure.keys())
        self.active_modules.sort()

    def __process_att(self, attdef: etree.Element) -> str:
        """
        Process attribute definition.
        """
        attdef_ident = attdef.get("ident")
        if "-" in attdef_ident:
            first, last = attdef_ident.split("-")
            attdef_ident = f"{first}{last.title()}"

        if attdef.get("ns"):
            return f"{attdef.get('ns')}|{attdef_ident}"
        elif ":" in attdef_ident:
            pfx, att = attdef_ident.split(":")
            return f"{NAMESPACES[pfx]}|{att}"
        else:
            return f"{attdef_ident}"

    def __get_membership(self, member: etree.Element, resarr: list[str]) -> None:
        """
        Get attribute groups.
        """
        member_attgroup = self.schema.xpath(
            ".//tei:classSpec[@type='atts'][@ident=$nm]", nm=member.get("key"), namespaces=TEI_NS
        )

        if member_attgroup is None:
            return None

        member_groupel = member_attgroup[0]
        if member_groupel.get("ident") == "att.id":
            return None

        if member_groupel.xpath("./tei:attList/tei:attDef", namespaces=TEI_NS):
            resarr.append(member_groupel.get("ident"))

        m2s = member_groupel.xpath("./tei:classes/tei:memberOf", namespaces=TEI_NS)
        for mship in m2s:
            self.__get_membership(mship, resarr)

    def strpatt(self, string: str) -> str:
        """
        Return a version of the string with any leading att. stripped.
        """
        return string.replace("att.", "")

    def strpdot(self, string: str) -> str:
        """
        Return a version of the string without any dots.
        """
        return "".join(string.split("."))

    def cc(self, att_name: str) -> str:
        """
        Return a CamelCasedName version of attribute.case.names.
        """
        return "".join([n[0].upper() + n[1:] for n in att_name.split(".")])

    def get_att_desc(self, att_name: str) -> str:
        """
        Return the documentation string for an attribute by name.
        """
        desc = self.schema.find(f".//tei:attDef[@ident='{att_name}']/tei:desc", namespaces=TEI_NS)
        if desc is None:
            return ""

        return re.sub(WHITESPACE_REGEX, " ", desc.xpath("string()"))

    def get_elem_desc(self, elem_name: str) -> str:
        """
        Return the documentation string for an element by name.
        """
        desc = self.schema.find(f".//tei:elementSpec[@ident='{elem_name}']/tei:desc", namespaces=TEI_NS)
        if desc is None:
            return ""

        return re.sub(WHITESPACE_REGEX, " ", desc.xpath("string()"))
