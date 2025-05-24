# -- coding: utf-8 --
from re import Pattern
from typing import Optional

import yaml
import logging
import re
import textwrap
from pathlib import Path

from schema import MeiSchema

lg = logging.getLogger("schemaparser")

NS_PREFIX_MAP = {"http://www.w3.org/XML/1998/namespace": "xml", "http://www.w3.org/1999/xlink": "xlink"}

AUTHORS = "Andrew Hankinson, Alastair Porter, and Others"

LICENSE = """/////////////////////////////////////////////////////////////////////////////
// Authors:     Laurent Pugin and Rodolfo Zitellini
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
//
// Code generated using a modified version of libmei
// by Andrew Hankinson, Alastair Porter, and Others
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// NOTE: this file was generated with the Verovio libmei version and
// should not be edited because changes will be lost.
/////////////////////////////////////////////////////////////////////////////"""

CLASS_FILE_H = """{license}

#ifndef __LIBMEI_{moduleNameCaps}_H__
#define __LIBMEI_{moduleNameCaps}_H__

#include "att.h"
#include "attdef.h"
#include "pugixml.hpp"

//----------------------------------------------------------------------------

{includes}

namespace {ns} {{

{headerElements}

}} // namespace {ns}

#endif // __LIBMEI_{moduleNameCaps}_H__
"""

CLASS_FILE_END = """}} // namespace {ns}
"""

#
# Templates for the attribute classes methods and members
#

ATTCLASS_H = """
//----------------------------------------------------------------------------
// Att{attGroupNameUpper}
//----------------------------------------------------------------------------

class Att{attGroupNameUpper} : public Att {{
protected:
    Att{attGroupNameUpper}();
    ~Att{attGroupNameUpper}() = default;

public:
    /** Reset the default values for the attribute class **/
    void Reset{attGroupNameUpper}();

    /** Read the values for the attribute class **/
    bool Read{attGroupNameUpper}(pugi::xml_node element, bool removeAttr = true);

    /** Write the values for the attribute class **/
    bool Write{attGroupNameUpper}(pugi::xml_node element);

    /**
     * @name Setters, getters and presence checker for class members.
     * The checker returns true if the attribute class is set (e.g., not equal
     * to the default value)
     **/
    ///@{{
{methods}///@}}

private:
{members}}};

//----------------------------------------------------------------------------
// Inst{attGroupNameUpper}
//----------------------------------------------------------------------------

/**
 * Instantiable version of Att{attGroupNameUpper}
 */

class Inst{attGroupNameUpper} : public Att{attGroupNameUpper} {{
public:
    Inst{attGroupNameUpper}() = default;
    virtual ~Inst{attGroupNameUpper}() = default;
}};
"""

ATTCLASS_METHODS_H = """    void Set{attNameUpper}({attType} {attNameLowerJoined}_) {{ m_{attNameLowerJoined} = {attNameLowerJoined}_; }}
    {attType} Get{attNameUpper}() const {{ return m_{attNameLowerJoined}; }}
    bool Has{attNameUpper}() const;
    """

ATTCLASS_ALTERNATE_METHODS_H = """/** Getter for reference (for alternate type only) */
    {attType} *Get{attNameUpper}Alternate() {{ return &m_{attNameLowerJoined}; }}
    """

ATTCLASS_MEMBERS_H = """{documentation}
    {attType} m_{attNameLowerJoined};
"""

ATTCLASS_FILE_CPP = """{license}

#include "{moduleNameLower}.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace {ns} {{

{implElements}

"""

ATTCLASS_CPP = """
//----------------------------------------------------------------------------
// Att{attGroupNameUpper}
//----------------------------------------------------------------------------

Att{attGroupNameUpper}::Att{attGroupNameUpper}() : Att()
{{
    this->Reset{attGroupNameUpper}();
}}

void Att{attGroupNameUpper}::Reset{attGroupNameUpper}()
{{
    {defaults}
}}

bool Att{attGroupNameUpper}::Read{attGroupNameUpper}(pugi::xml_node element, bool removeAttr)
{{
    bool hasAttribute = false;
    {reads}
    return hasAttribute;
}}

bool Att{attGroupNameUpper}::Write{attGroupNameUpper}(pugi::xml_node element)
{{
    bool wroteAttribute = false;
    {writes}
    return wroteAttribute;
}}

{checkers}
"""

ATTCLASS_MEMBERS_DEFAULT_CPP = """m_{attNameLowerJoined} = {attDefault};"""

ATTCLASS_READ_CPP = """if (element.attribute("{attNameLower}")) {{
        this->Set{attNameUpper}({converterRead}(element.attribute("{attNameLower}").value()));
        if (removeAttr) element.remove_attribute("{attNameLower}");
        hasAttribute = true;
    }}"""

ATTCLASS_WRITE_CPP = """if (this->Has{attNameUpper}()) {{
        element.append_attribute("{attNameLower}") = {converterWrite}(this->Get{attNameUpper}()).c_str();
        wroteAttribute = true;
    }}"""

ATTCLASS_CHECK_CPP = """bool Att{attGroupNameUpper}::Has{attNameUpper}() const
{{
    return (m_{attNameLowerJoined} != {attDefault});
}}

"""

ATTCLASS_CHECK_ALTERNATE_CPP = """bool Att{attGroupNameUpper}::Has{attNameUpper}() const
{{
    return (m_{attNameLowerJoined}.HasValue());
}}

"""

#
# Templates for the element classes
#

ELEMENTCLASS_FILE_H = """{license}

#include "{moduleNameLower}.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace {ns} {{

{elements}

"""

ELEMENTCLASS_H = """
{documentation}
class {elementNameUpper} : public Element{attClasses} {{
    public:
        {elementNameUpper}();
        virtual ~{elementNameUpper}();

public:
    bool Read(pugi::xml_node element, bool removeAttr = false);
    bool Write(pugi::xml_node element, const std::string &xmlId = "");
    void Reset();
}};
"""

ELEMENTCLASS_CPP = """{elementNameUpper}::{elementNameUpper}() :{attClasses}
{{
}}

{elementNameUpper}::~{elementNameUpper}() {{}}

bool {elementNameUpper}::Read({readParam})
{{
    if (element.attribute("xml:id")) m_xmlId = element.attribute("xml:id").value();
    bool hasAttribute = false;{elementRead}
    return hasAttribute;
}}

bool {elementNameUpper}::Write({writeParam})
{{
    element.set_name("{elementNameLower}");
    if (xmlId.size() > 0) element.append_attribute("xml:id") = xmlId.c_str();
    bool hasAttribute = false;{elementWrite}
    return hasAttribute;
}}

void {elementNameUpper}::Reset() 
{{     {elementReset}
}}

"""

#
# These templates the list of enum for att classes ids
#

ENUM_GRP_START = """

#ifndef __LIBMEI_ATT_CLASSES_H__
#define __LIBMEI_ATT_CLASSES_H__

//----------------------------------------------------------------------------

namespace {ns} {{

enum AttClassId {{
    ATT_CLASS_min = 0,
"""

ENUM_GRP_END = """    ATT_CLASS_max
}};

}} // namespace {ns}

#endif // __LIBMEI_ATT_CLASSES_H__
"""

#
# These templates generate the type definitions
#

TYPE_GRP_START = """

#ifndef __LIBMEI_ATT_TYPES_H__
#define __LIBMEI_ATT_TYPES_H__

#include <cstdint>

//----------------------------------------------------------------------------

namespace {ns} {{

"""

TYPE_GRP_END = """}} // namespace {ns}

#endif // __LIBMEI_ATT_TYPES_H__
"""

TYPE_START = """/**
 * MEI {meitype}
 */
enum {vrvtype}{enumtype} {{
    {val_prefix}_NONE = 0,"""

TYPE_VALUE = """
    {val_prefix}_{value},"""

TYPE_END = """
    {val_prefix}_MAX
}};

"""

#
# These templates generate converter methods
#

CONVERTER_H = """

#ifndef __LIBMEI_ATT_CONVERTER_H__
#define __LIBMEI_ATT_CONVERTER_H__

#include <string>

//----------------------------------------------------------------------------

#include "attdef.h"

namespace {ns} {{

//----------------------------------------------------------------------------
// AttConverterBase
//----------------------------------------------------------------------------

class AttConverterBase {{
protected:
    AttConverterBase() = default;
    ~AttConverterBase() = default;

public:"""

CONVERTER_METHODS_H = """
    std::string {fname}ToStr({type} data) const;
    {type} StrTo{fname}(const std::string &value, bool logWarning = true) const;
"""

CONVERTER_END_H = """}};

//----------------------------------------------------------------------------
// AttConverter
//----------------------------------------------------------------------------

/**
 * Instantiable version of AttConverterBase
 */

class AttConverter : public AttConverterBase {{
public:
    AttConverter() = default;
    virtual ~AttConverter() = default;
}};

}} // namespace {ns}

#endif // __LIBMEI_ATT_CONVERTER_H__
"""

CONVERTER_CPP = """

#include "attconverter.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "libmei.h"

namespace {ns} {{

//----------------------------------------------------------------------------
// AttConverterBase
//----------------------------------------------------------------------------
"""

CONVERTER_METHOD1_START_CPP = """
std::string AttConverterBase::{fname}ToStr({type} data) const
{{
    std::string value;
    switch (data) {{"""

CONVERTER_METHOD2_START_CPP = """
{type} AttConverterBase::StrTo{fname}(const std::string &value, bool logWarning) const
{{"""

CONVERTER_METHOD1_CPP = """
        case {val_prefix}_{value}: value = "{string}"; break;"""

CONVERTER_METHOD2_CPP = """
    if (value == "{string}") return {val_prefix}_{value};"""

CONVERTER_METHOD1_END_CPP = """
        default:
            LogWarning("Unknown value '%d' for {type}", data);
            value = "";
            break;
    }}
    return value;
}}
"""

CONVERTER_METHOD2_END_CPP = """
    if (logWarning && !value.empty())
        LogWarning("Unsupported value '%s' for {type}", value.c_str());
    return {prefix}_NONE;
}}
"""

CONVERTER_END_CPP = """
}} // namespace {ns}
"""

#
# These templates generate a module level static method for setting attribute on an unspecified Object
#

ATTMODULE_FILE_H = """{license}

#ifndef __LIBMEI_ATT_MODULE_H__
#define __LIBMEI_ATT_MODULE_H__

#include <string>
#include <utility>
#include <vector>

namespace {ns} {{

typedef std::vector<std::pair<std::string, std::string>> ArrayOfStrAttr;

class Object;

//----------------------------------------------------------------------------
// AttModule
//----------------------------------------------------------------------------

/**
 * This is the base class for all MEI att classes.
 * It is not an abstract class but it should not be instanciated directly.
 * The att classes are generated with the libmei parser for Verovio.
 */
class AttModule {{
public:
    /** @name Constructors and destructor */
    ///@{{
    AttModule() {{}}
    virtual ~AttModule() {{}}
    ///@}}

    /**
     * @name static method for blind attribute reading or modification
     */
    ///@{{
"""

SETTERS_GETTERS_H = """    static bool Set{moduleNameCap}(Object *element, const std::string &attrType, const std::string &attrValue);
    static void Get{moduleNameCap}(const Object *element, ArrayOfStrAttr *attributes);
    static void Copy{moduleNameCap}(const Object *element, Object *target);

"""

ATTMODULE_FILE_END_H = """    ///@}}
}};

}} // namespace {ns}

#endif // __LIBMEI_ATT_MODULE_H__
"""

ATTMODULE_FILE_CPP = """{license}

#include "attmodule.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "object.h"
"""

SETTERS_START_CPP = """
#include "{moduleNameLower}.h"

namespace {ns} {{

//----------------------------------------------------------------------------
// {moduleNameCap}
//----------------------------------------------------------------------------

bool AttModule::Set{moduleNameCap}(Object *element, const std::string &attrType, const std::string &attrValue)
{{
"""

SETTERS_GRP_START_CPP = """    if (element->HasAttClass({attId})) {{
        Att{attGroupNameUpper} *att = dynamic_cast<Att{attGroupNameUpper} *>(element);
        assert(att);
"""

SETTERS_GRP_CPP = """        if (attrType == "{attNameLower}") {{
            att->Set{attNameUpper}(att->{converterRead}(attrValue));
            return true;
        }}
"""

SETTERS_GRP_END_CPP = """    }}
"""

SETTERS_END_CPP = """
    return false;
}}

"""

#
# These templates generate a module level static method for getting attributes of an unspecified Object
#

GETTERS_START_CPP = """void AttModule::Get{moduleNameCap}(const Object *element, ArrayOfStrAttr *attributes)
{{
"""

GETTERS_GRP_START_CPP = """    if (element->HasAttClass({attId})) {{
        const Att{attGroupNameUpper} *att = dynamic_cast<const Att{attGroupNameUpper} *>(element);
        assert(att);
"""

GETTERS_GRP_CPP = """        if (att->Has{attNameUpper}()) {{
            attributes->push_back({{ "{attNameLower}", att->{converterWrite}(att->Get{attNameUpper}()) }});
        }}
"""

GETTERS_GRP_END_CPP = """    }}
"""

GETTERS_END_CPP = """}}

"""

#
# These templates generate a module level static method for copying attributes of an unspecified Object
#

COPYERS_START_CPP = """void AttModule::Copy{moduleNameCap}(const Object *element, Object *target)
{{
"""

COPYERS_GRP_START_CPP = """    if (element->HasAttClass({attId})) {{
        const Att{attGroupNameUpper} *att = dynamic_cast<const Att{attGroupNameUpper} *>(element);
        assert(att);
        Att{attGroupNameUpper} *attTarget = dynamic_cast<Att{attGroupNameUpper} *>(target);
        assert(attTarget);
"""

COPYERS_GRP_CPP = """        attTarget->Set{attNameUpper}(att->Get{attNameUpper}());
"""

COPYERS_GRP_END_CPP = """    }}
"""

COPYERS_END_CPP = """}}

}} // namespace {ns}
"""

DATATYPES: dict

TEI_RNG_NS = {"tei": "http://www.tei-c.org/ns/1.0", "rng": "http://relaxng.org/ns/structure/1.0"}


def vrv_member_cc(name: str) -> str:
    cc = "".join([n[0].upper() + n[1:] for n in name.split(".")])
    return cc[0].lower() + cc[1:]


def vrv_member_cc_upper(name: str) -> str:
    return "".join([n[0].upper() + n[1:] for n in name.split(".")])


def vrv_converter_cc(name: str) -> str:
    left, right = name.split("_", 1)
    rest = "".join([n[0].upper() + n[1:].lower() for n in right.split("_")])
    if left == "data":
        return rest
    return left[0].upper() + left[1:] + rest


def vrv_get_att_config(module, gp, att) -> Optional[dict]:
    if module not in DATATYPES["modules"] or gp not in DATATYPES["modules"][module]:
        return None
    if att not in DATATYPES["modules"][module][gp]:
        return None
    return DATATYPES["modules"][module][gp][att]


def vrv_get_type_default(datatype: str) -> str:
    if datatype in DATATYPES["defaults"]:
        return DATATYPES["defaults"][datatype]

    tname: str = re.sub(r"^data_", "", datatype)
    return f"{tname}_NONE"


def vrv_is_excluded_type(datatype: str) -> bool:
    return datatype in DATATYPES["excludes"]


def vrv_is_alternate_type(datatype: str) -> bool:
    return datatype in DATATYPES["alternates"]


def vrv_get_att_config_type(module: str, gp: str, att: str) -> Optional[str]:
    """Get the att type."""
    att_config = vrv_get_att_config(module, gp, att)
    if not att_config or "type" not in att_config:
        return None
    return att_config["type"]


def vrv_get_att_config_default(module: str, gp: str, att: str) -> Optional[str]:
    """Get the att default value."""
    att_config = vrv_get_att_config(module, gp, att)
    # nothing in the module/att
    if att_config is None or "default" not in att_config:
        return None
    # return the module/att default
    return att_config["default"]


def vrv_getformattedtype(datatype: str) -> str:
    if datatype in DATATYPES["mapped"]:
        return DATATYPES["mapped"][datatype]
    return datatype.replace(".", "_")


def vrv_getformattedvallist(att: str, vallist: str) -> str:
    pfx: str = vrv_member_cc(att.replace("att.", ""))
    sfx: str = vallist.upper().replace(".", "").replace(":", "")
    return f"{pfx}_{sfx}"


def vrv_getatttype(schema, module: str, gp: str, aname: str) -> str:
    """Returns the attribute type for element name, or string if not detectable."""
    # Look up if there is an override for this type in the current module, and return it
    # Note that we do not honor pseudo-hungarian notation
    attype: Optional[str] = vrv_get_att_config_type(module, gp, aname)
    if attype:
        return attype

    # No override, get it from the schema
    definition = schema.xpath(
        "//tei:classSpec[@ident=$gp]/tei:attList/tei:attDef[@ident=$name]", gp=gp, name=aname, namespaces=TEI_RNG_NS
    )
    if not definition:
        return "std::string"

    # First numbers
    el = definition[0].xpath(
        "tei:datatype/tei:dataRef/@name|tei:datatype/rng:data/@type", name=aname, namespaces=TEI_RNG_NS
    )
    if el:
        if el[0] in ("integer", "positiveInteger", "nonNegativeInteger"):
            return "int"
        elif el[0] == "decimal":
            return "double"

    # The data types
    ref = definition[0].xpath(
        "tei:datatype/tei:dataRef/@key|tei:datatype/rng:ref/@name", gp=gp, name=aname, namespaces=TEI_RNG_NS
    )
    if ref:
        return vrv_getformattedtype(f"{ref[0]}")

    # Finally from val lists
    vl = definition[0].find("tei:valList[@type='closed']", namespaces=TEI_RNG_NS)
    if vl is not None:
        element = vl.xpath("./ancestor::tei:classSpec", namespaces=TEI_RNG_NS)
        att_name = vl.xpath("./parent::tei:attDef/@ident", namespaces=TEI_RNG_NS)
        if element:
            return vrv_getformattedvallist(element[0].get("ident"), att_name[0])

    # Otherwise as string
    return "std::string"


def vrv_getattdefault(schema, module: str, gp: str, aname: str) -> tuple:
    """Returns the attribute default value for element name, or string if not detectable."""
    attype = vrv_getatttype(schema, module, gp, aname)
    default = vrv_get_att_config_default(module, gp, aname)

    if attype == "int":
        if default is None:
            default = "MEI_UNSET"
        return default, ["StrToInt", "IntToStr"]
    elif attype == "char":
        if default is None:
            default = 0
        return default, ["StrToInt", "IntToStr"]
    elif attype == "double":
        if default is None:
            default = 0.0
        return default, ["StrToDbl", "DblToStr"]
    elif attype == "std::string":
        return '""', ["StrToStr", "StrToStr"]
    else:
        if default is None:
            default = vrv_get_type_default(attype)

        cname = vrv_converter_cc(attype)
        return default, [f"StrTo{cname}", f"{cname}ToStr"]


def create_docstr(text: str, indent: int = 0) -> str:
    """
    Format a docstring. Take the first sentence (. followed by a space)
    and use it for the brief. Then put the rest of the text after a blank
    line if there is text there
    """
    text = text.strip()
    dotpos = text.find(". ")

    if dotpos > 0:
        brief = text[: dotpos + 1]
        content = text[dotpos + 2 :]
    else:
        brief = text
        content = ""

    if indent == 0:
        istr = ""
    else:
        istr = f"{' ':<{indent}}"

    if len(brief) > 1:
        brief = brief[0].upper() + brief[1:]
    else:
        brief = "---"

    brief = f"\n{istr} * ".join(textwrap.wrap(brief, 80))
    content = f"\n{istr} * ".join(textwrap.wrap(content, 80))
    docstr = f"{istr}/**"

    if len(content) > 0 or brief.count("\n"):
        docstr += f"\n{istr} *"
    docstr += f" {brief}"

    if brief.count("\n") and len(content) == 0:
        docstr += f"\n{istr}"

    if len(content) > 0:
        docstr += f"\n{istr} * {content}\n{istr}"
    docstr += " **/"
    return docstr


def create_att_classes(cpp_ns: str, schema, outdir: Path):
    enums: list = []

    for module, atgroup in sorted(schema.attribute_group_structure.items()):
        if not atgroup:
            continue

        header_classes: list = []
        impl_classes: list = []

        for gp, atts in sorted(atgroup.items()):
            if not atts:
                continue

            members: list = []
            methods: list = []
            defaults: list = []
            reads: list = []
            writes: list = []
            checkers: list = []

            for att in atts:
                if "|" in att:
                    # we have a namespaced attribute
                    ns, att = att.split("|")
                    prefix = NS_PREFIX_MAP[ns]
                    att_lower_name = f"{prefix}:{att}"
                else:
                    att_lower_name = att

                att_type = vrv_getatttype(schema.schema, module, gp, att)
                doc_str = create_docstr(schema.get_att_desc(att), indent=4)
                attdefault, converters = vrv_getattdefault(schema.schema, module, gp, att)

                substrings = {
                    "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                    "attNameUpper": schema.cc(schema.strpatt(att)),
                    "attNameLower": att_lower_name,
                    "attNameLowerJoined": vrv_member_cc(att),
                    "documentation": doc_str,
                    "attType": att_type,
                    "attDefault": attdefault,
                    "converterRead": converters[0],
                    "converterWrite": converters[1],
                }

                if len(methods) > 0:
                    methods.append("//\n")

                methods.append(ATTCLASS_METHODS_H.format_map(substrings))
                if vrv_is_alternate_type(att_type):
                    methods.append(ATTCLASS_ALTERNATE_METHODS_H.format_map(substrings))

                members.append(ATTCLASS_MEMBERS_H.format_map(substrings))

                if len(defaults) > 0:
                    defaults.append("\n    ")
                    reads.append("\n    ")
                    writes.append("\n    ")

                defaults.append(ATTCLASS_MEMBERS_DEFAULT_CPP.format_map(substrings))
                reads.append(ATTCLASS_READ_CPP.format_map(substrings))
                writes.append(ATTCLASS_WRITE_CPP.format_map(substrings))

                if vrv_is_alternate_type(att_type):
                    checkers.append(ATTCLASS_CHECK_ALTERNATE_CPP.format_map(substrings))
                else:
                    checkers.append(ATTCLASS_CHECK_CPP.format_map(substrings))

            clsubstr = {
                "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                "methods": "".join(methods),
                "members": "".join(members),
                "defaults": "".join(defaults),
                "reads": "".join(reads),
                "writes": "".join(writes),
                "checkers": "".join(checkers),
            }
            header_classes.append(ATTCLASS_H.format_map(clsubstr).strip())
            impl_classes.append(ATTCLASS_CPP.format_map(clsubstr).strip())
            enums.append(f"    ATT_{schema.cc(schema.strpatt(gp)).upper()},\n")

        tplvars = {
            "includes": "#include <string>",
            "license": LICENSE.format(authors=AUTHORS),
            "moduleNameCaps": f"ATTS_{module.upper()}",
            "moduleNameCap": module.capitalize(),
            "moduleNameLower": f"atts_{module.lower()}",
            "headerElements": "\n\n".join(header_classes),
            "implElements": "\n\n".join(impl_classes),
            "ns": cpp_ns,
        }

        with Path(outdir, f"atts_{module.lower()}.h").open("w") as f_att_class_h:
            lg.debug(f"\tCreating atts_{module.lower()}.h")
            f_att_class_h.write(CLASS_FILE_H.format_map(tplvars))

        with Path(outdir, f"atts_{module.lower()}.cpp").open("w") as f_att_class_cpp:
            lg.debug(f"\tCreating atts_{module.lower()}.cpp")
            f_att_class_cpp.write(ATTCLASS_FILE_CPP.format_map(tplvars))
            f_att_class_cpp.write(CLASS_FILE_END.format(ns=cpp_ns))

    with Path(outdir, "attclasses.h").open("w") as f_att_classes_h:
        f_att_classes_h.write(LICENSE)
        f_att_classes_h.write(ENUM_GRP_START.format(ns=cpp_ns))
        f_att_classes_h.write("".join(enums))
        f_att_classes_h.write(ENUM_GRP_END.format(ns=cpp_ns))


def create_att_datatypes(cpp_ns: str, schema, outdir: Path):
    # data types
    att_type_data_types: list = []
    att_converter_header_data_types: list = []
    att_converter_impl_data_types: list = []
    for data_type, values in sorted(schema.data_types.items()):
        att_converter_impl_from_converters: list = []
        att_converter_impl_to_converters: list = []

        if vrv_is_excluded_type(data_type):
            lg.debug("Skipping excluded %s", data_type)
            continue

        if vrv_is_alternate_type(data_type):
            lg.debug("Skipping alternate %s", data_type)
            continue

        val_prefix = vrv_getformattedtype(data_type).replace("data_", "")
        type_start_fmt = {
            "meitype": data_type,
            "vrvtype": vrv_getformattedtype(data_type),
            "enumtype": " : int8_t" if len(values) < 64 else "",
            "val_prefix": val_prefix,
        }
        att_type_data_types.append(TYPE_START.format_map(type_start_fmt))

        vrv_type = vrv_getformattedtype(data_type)
        vrv_fname = vrv_converter_cc(vrv_type)
        converter_start_fmt = {"type": vrv_type, "fname": vrv_fname}

        att_converter_header_data_types.append(CONVERTER_METHODS_H.format_map(converter_start_fmt))
        att_converter_impl_from_converters.append(CONVERTER_METHOD1_START_CPP.format_map(converter_start_fmt))
        att_converter_impl_to_converters.append(CONVERTER_METHOD2_START_CPP.format_map(converter_start_fmt))

        for v in values:
            val: str = re.sub(r"[\.\-\,]", "_", v)
            val = re.sub(r"\+", "plus", val)
            val_fmt = {"val_prefix": val_prefix, "value": val, "string": v}

            att_type_data_types.append(TYPE_VALUE.format_map(val_fmt))
            att_converter_impl_from_converters.append(CONVERTER_METHOD1_CPP.format_map(val_fmt))
            att_converter_impl_to_converters.append(CONVERTER_METHOD2_CPP.format_map(val_fmt))

        att_type_data_types.append(TYPE_END.format(val_prefix=val_prefix))

        converter_end_fmt = {"prefix": val_prefix, "type": data_type}
        att_converter_impl_from_converters.append(CONVERTER_METHOD1_END_CPP.format_map(converter_end_fmt))
        att_converter_impl_to_converters.append(CONVERTER_METHOD2_END_CPP.format_map(converter_end_fmt))

        att_converter_impl_data_types.extend(att_converter_impl_from_converters)
        att_converter_impl_data_types.extend(att_converter_impl_to_converters)

    att_type_data_list: list = []
    att_converter_header_data_list: list = []
    att_converter_impl_data_list: list = []
    for list_type, values in sorted(schema.data_lists.items()):
        att_converter_impl_from_converters: list = []
        att_converter_impl_to_converters: list = []

        if vrv_is_excluded_type(list_type):
            lg.debug("Skipping %s", list_type)
            continue

        val_prefix = vrv_getformattedvallist(list_type.rsplit("@")[0], list_type.rsplit("@")[1])
        type_start_fmt = {
            "meitype": list_type.replace("@", r"\@"),
            "vrvtype": val_prefix,
            "enumtype": " : int8_t" if len(values) < 64 else "",
            "val_prefix": val_prefix,
        }
        att_type_data_list.append(TYPE_START.format_map(type_start_fmt))
        vrv_type = val_prefix
        vrv_fname = vrv_converter_cc(vrv_type)

        converter_start_fmt = {"type": vrv_type, "fname": vrv_fname}
        att_converter_header_data_list.append(CONVERTER_METHODS_H.format_map(converter_start_fmt))
        att_converter_impl_from_converters.append(CONVERTER_METHOD1_START_CPP.format_map(converter_start_fmt))
        att_converter_impl_to_converters.append(CONVERTER_METHOD2_START_CPP.format_map(converter_start_fmt))

        for v in values:
            val: str = re.sub(r"[\.\-\,]", "_", v)
            val = re.sub(r"\+", "plus", val)
            val_fmt = {"val_prefix": val_prefix, "value": val, "string": v}
            att_type_data_list.append(TYPE_VALUE.format_map(val_fmt))
            att_converter_impl_from_converters.append(CONVERTER_METHOD1_CPP.format_map(val_fmt))
            att_converter_impl_to_converters.append(CONVERTER_METHOD2_CPP.format_map(val_fmt))

        att_type_data_list.append(TYPE_END.format(val_prefix=val_prefix))
        att_converter_impl_from_converters.append(CONVERTER_METHOD1_END_CPP.format(prefix=val_prefix, type=list_type))
        att_converter_impl_to_converters.append(CONVERTER_METHOD2_END_CPP.format(prefix=val_prefix, type=list_type))
        att_converter_impl_data_list.extend(att_converter_impl_from_converters)
        att_converter_impl_data_list.extend(att_converter_impl_to_converters)

    with Path(outdir, "atttypes.h").open("w") as f_att_types_h:
        lg.debug("\tCreating atttypes.h")
        f_att_types_h.write(LICENSE)
        f_att_types_h.write(TYPE_GRP_START.format(ns=cpp_ns))
        f_att_types_h.write("".join(att_type_data_types))
        f_att_types_h.write("".join(att_type_data_list))
        f_att_types_h.write(TYPE_GRP_END.format(ns=cpp_ns))

    with Path(outdir, "attconverter.h").open("w") as f_att_converter_h:
        lg.debug("\tCreating attconverter.h")
        f_att_converter_h.write(LICENSE)
        f_att_converter_h.write(CONVERTER_H.format(ns=cpp_ns))
        f_att_converter_h.write("".join(att_converter_header_data_types))
        f_att_converter_h.write("".join(att_converter_header_data_list))
        f_att_converter_h.write(CONVERTER_END_H.format(ns=cpp_ns))

    with Path(outdir, "attconverter.cpp").open("w") as f_att_converter_cpp:
        lg.debug("\tCreating attconverter.cpp")
        f_att_converter_cpp.write(LICENSE)
        f_att_converter_cpp.write(CONVERTER_CPP.format(ns=cpp_ns))
        f_att_converter_cpp.write("".join(att_converter_impl_data_types))
        f_att_converter_cpp.write("".join(att_converter_impl_data_list))
        f_att_converter_cpp.write(CONVERTER_END_CPP.format(ns=cpp_ns))


def create_element_classes(cpp_ns: str, schema, outdir: Path):
    lg.debug("Creating Element Headers")
    ###########################################################################
    # Header
    ###########################################################################
    for module, elements in sorted(schema.element_structure.items()):
        if not elements:
            continue

        element_output: list = []
        includes: list = []

        for element, atgroups in sorted(elements.items()):
            element_att_classes: list = []
            for attribute in atgroups:
                if isinstance(attribute, list):
                    # it's our self-defined attributes.
                    lg.debug("Skipping attributes within class")
                    continue

                element_att_classes.append(f", public Att{schema.cc(schema.strpatt(attribute))}")

                # figure out includes
                if attribute in schema.inverse_attribute_group_structure:
                    mod = schema.inverse_attribute_group_structure[attribute]
                    if mod not in includes:
                        includes.append(mod)

            docstr = create_docstr(schema.get_elem_desc(element), indent=0)
            elvars = {
                "elementNameUpper": schema.cc(element),
                "attClasses": "".join(element_att_classes),
                "documentation": docstr.strip(),
                "elementName": element,
            }

            element_output.append(ELEMENTCLASS_H.format_map(elvars))

        incl_output: list = ['#include "element.h"\n']
        for incl in sorted(includes):
            incl_output.append(f'#include "atts_{incl.lower()}.h"\n')

        if "std::string" in element_output:
            incl_output.append("#include <string>\n")

        outvars = {
            "includes": "".join(incl_output),
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
            "moduleNameCaps": module.upper().replace("-", "_"),
            "headerElements": "".join(element_output).strip(),
        }

        with Path(outdir, f"{module.lower()}.h").open("w") as f_element_class_h:
            lg.debug(f"\tCreating {module.lower()}.h")
            f_element_class_h.write(CLASS_FILE_H.format_map(outvars))

    lg.debug("Creating Element Implementations")
    # ###########################################################################
    # # Implementation
    # ###########################################################################
    for module, elements in sorted(schema.element_structure.items()):
        if not elements:
            continue

        element_constructor: list = []

        for element, atgroups in sorted(elements.items()):
            element_att_classes: list = [f'\n    Element("{element}")']
            element_read: list = []
            element_write: list = []
            element_reset: list = []
            for attribute in atgroups:
                if isinstance(attribute, list):
                    lg.debug("Skipping attributes within class")
                    continue

                att_str = schema.cc(schema.strpatt(attribute))
                element_att_classes.append(f", Att{att_str}()")
                element_read.append(f"\n    hasAttribute = (Read{att_str}(element, removeAttr) || hasAttribute);")
                element_write.append(f"\n    hasAttribute = (Write{att_str}(element) || hasAttribute);")
                element_reset.append(f"\n    Reset{att_str}();")

            read_param = "pugi::xml_node element, bool"
            if element_read:
                read_param = "pugi::xml_node element, bool removeAttr"
            write_param = "pugi::xml_node element, const std::string &xmlId"

            consvars = {
                "elementNameUpper": schema.cc(element),
                "elementNameLower": element,
                "attClasses": "".join(element_att_classes),
                "elementRead": "".join(element_read),
                "elementWrite": "".join(element_write),
                "elementReset": "".join(element_reset),
                "readParam": read_param,
                "writeParam": write_param,
            }
            element_constructor.append(ELEMENTCLASS_CPP.format_map(consvars))

        implvars = {
            "moduleNameLower": module.lower(),
            "elements": "".join(element_constructor),
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
        }

        with Path(outdir, f"{module.lower()}.cpp").open("w") as f_element_class_cpp:
            lg.debug(f"\t Creating {module.lower()}.cpp")
            f_element_class_cpp.write(ELEMENTCLASS_FILE_H.format_map(implvars))
            f_element_class_cpp.write(CLASS_FILE_END.format(ns=cpp_ns))


def create_att_module(cpp_ns: str, schema, outdir: Path):
    lg.debug("Creating AttModule Header and Implementation.")
    header_modules: list = []
    impl_modules: list = []

    for module, atgroup in sorted(schema.attribute_group_structure.items()):
        if not atgroup:
            continue

        setters: list = []
        getters: list = []
        copyers: list = []

        for gp, atts in sorted(atgroup.items()):
            if not atts:
                continue

            set_get_fmt = {
                "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                "attId": f"ATT_{schema.cc(schema.strpatt(gp)).upper()}",
            }
            setters.append(SETTERS_GRP_START_CPP.format_map(set_get_fmt))
            getters.append(GETTERS_GRP_START_CPP.format_map(set_get_fmt))
            copyers.append(COPYERS_GRP_START_CPP.format_map(set_get_fmt))

            for att in atts:
                if "|" in att:
                    # we have a namespaced attribute
                    ns, att = att.split("|")
                    prefix = NS_PREFIX_MAP[ns]
                    att_name_lower = f"{prefix}:{att}"
                else:
                    att_name_lower = att

                _, converters = vrv_getattdefault(schema.schema, module, gp, att)
                attsubstr = {
                    "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                    "attNameUpper": schema.cc(att),
                    "attNameLower": att_name_lower,
                    "attNameLowerJoined": vrv_member_cc(att),
                    "converterRead": converters[0],
                    "converterWrite": converters[1],
                }
                setters.append(SETTERS_GRP_CPP.format_map(attsubstr))
                getters.append(GETTERS_GRP_CPP.format_map(attsubstr))
                copyers.append(COPYERS_GRP_CPP.format_map(attsubstr))

            setters.append(SETTERS_GRP_END_CPP.format_map(attsubstr))
            getters.append(GETTERS_GRP_END_CPP.format_map(attsubstr))
            copyers.append(COPYERS_GRP_END_CPP.format_map(attsubstr))

        tplvars = {
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
            "moduleNameLower": f"atts_{module.lower()}",
            "moduleNameCap": module.capitalize(),
        }

        header_modules.append(SETTERS_GETTERS_H.format_map(tplvars))
        impl_modules.append(SETTERS_START_CPP.format_map(tplvars))
        impl_modules.append("".join(setters))
        impl_modules.append(SETTERS_END_CPP.format_map(tplvars))
        impl_modules.append(GETTERS_START_CPP.format_map(tplvars))
        impl_modules.append("".join(getters))
        impl_modules.append(GETTERS_END_CPP.format_map(tplvars))
        impl_modules.append(COPYERS_START_CPP.format_map(tplvars))
        impl_modules.append("".join(copyers))
        impl_modules.append(COPYERS_END_CPP.format_map(tplvars))

    with Path(outdir, "attmodule.h").open("w") as f_att_module_h:
        lg.debug("\tCreating attmodule.h")
        lic: str = LICENSE.format(authors=AUTHORS)
        f_att_module_h.write(ATTMODULE_FILE_H.format(license=lic, ns=cpp_ns))
        f_att_module_h.write("".join(header_modules))
        f_att_module_h.write(ATTMODULE_FILE_END_H.format(ns=cpp_ns))

    with Path(outdir, "attmodule.cpp").open("w") as f_att_module_cpp:
        lg.debug("\tCreated attmodule.cpp")
        f_att_module_cpp.write(ATTMODULE_FILE_CPP.format(license=LICENSE.format(authors=AUTHORS)))
        f_att_module_cpp.write("".join(impl_modules))


def copy_addons(namespace: str, addons_dir: Path, outdir: Path):
    # files in ./addons are required.
    # These are the only files to be edited by hand.
    # When the output directory is not ".", they need to be copied
    # When the namespace option is specified, the vrv namespace need to be adjusted when copying
    NS_REGEX_BEGIN: Pattern = re.compile(r"^namespace vrv {$", re.M)
    NS_REGEX_END: Pattern = re.compile(r"^} // namespace vrv$", re.M)

    for f in addons_dir.iterdir():
        if not f.is_file():
            continue

        outfile: Path = Path(outdir, f.name)
        file_contents: str = f.read_text()
        repl_file_contents: str = re.sub(NS_REGEX_BEGIN, f"namespace {namespace} {{", file_contents)
        repl_file_contents: str = re.sub(NS_REGEX_END, f"}} // namespace {namespace}", repl_file_contents)
        outfile.write_text(repl_file_contents)
        lg.debug("Wrote addon %s", outfile)


BASIC_VALID_CPP: str = """
    inline static std::map<std::string, std::vector<std::string>> map = {{
        {nameAttributeMap}
    }};
"""

MEI_BASIC_H: str = """
class MEIBasic {{
public:
    static bool IsAllowed(const std::string &element, const std::string &attr)
    {{
        if (attr == "xml:id") return true;
        if (!MEIBasic::map.count(element)) return false;
        const std::vector<std::string> &attributes = MEIBasic::map.at(element);
        return (std::find(attributes.begin(), attributes.end(), attr) != attributes.end());
    }}

    // clang-format off
    {basicAttributeMap}
    // clang-format on
}};
"""


def create_basic_validator(configure: dict, outdir: Path):
    basic_path = Path(configure["basic_odd"])
    with basic_path.open("r") as basic_schema:
        bschema = MeiSchema(basic_schema, resolve_elements=True)

    flat_att_groups = {}
    for mod, attgrp in bschema.attribute_group_structure.items():
        for attgrpname, attrs in attgrp.items():
            flat_att_groups[attgrpname] = attrs

    elres: dict = {}
    for module, elements in bschema.element_structure.items():
        for elname, elattrs in elements.items():
            out_list = []
            for att in elattrs:
                if isinstance(att, list):
                    out_list.extend(att)
                else:
                    out_list.extend(flat_att_groups.get(att, []))
            elres[elname] = out_list

    formatted_attr_map: list = []
    for elname, elattrs in elres.items():
        attrlist = []
        for att in elattrs:
            if "|" in att:
                # we have a namespaced attribute
                ns, att = att.split("|")
                prefix = NS_PREFIX_MAP[ns]
                attrlist.append(f"{prefix}:{att}")
            else:
                attrlist.append(att)

        if attrlist:
            fmt_attr = '", "'.join(attrlist)
            fmt_attr_str = f'{{"{fmt_attr}"}}'
        else:
            fmt_attr_str = "{}"

        fmt_attr_map = f'        {{"{elname}", {fmt_attr_str}}},\n'
        formatted_attr_map.append(fmt_attr_map)

    name_attribute_map = "".join(formatted_attr_map).strip()
    formatted_output = BASIC_VALID_CPP.format(nameAttributeMap=name_attribute_map)

    with Path(outdir, "meibasic.h").open("w") as mei_basic_h:
        lg.debug("Writing meibasic.h")
        mei_basic_h.write(MEI_BASIC_H.format(basicAttributeMap=formatted_output))


def create(schema, configure: dict) -> bool:
    global DATATYPES
    lg.debug("Begin Verovio C++ Output ...")

    datatypes: Path = Path(configure["datatypes"])
    if not datatypes.exists():
        lg.error("Datatypes file could not be found")
        return False

    DATATYPES = yaml.safe_load(datatypes.open("r"))

    ns: str = configure["namespace"]
    outdir: Path = Path(configure["output_dir"])

    # if we output element classes, then we do not output the attmodule class
    if configure["elements"]:
        create_element_classes(ns, schema, outdir)
        if addons := configure.get("addons_dir"):
            copy_addons(ns, Path(addons), outdir)
        else:
            lg.warning("Element output was configured, but no addons directory was provided.")
    else:
        create_att_module(ns, schema, outdir)

    create_att_classes(ns, schema, outdir)
    create_att_datatypes(ns, schema, outdir)

    if configure["basic_odd"]:
        create_basic_validator(configure, outdir)

    lg.debug("Success!")
    return True
