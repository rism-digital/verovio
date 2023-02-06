# -- coding: utf-8 --

import yaml
import logging
import re
import textwrap
from pathlib import Path

lg = logging.getLogger('schemaparser')

NS_PREFIX_MAP = {
    "http://www.w3.org/XML/1998/namespace": "xml",
    "http://www.w3.org/1999/xlink": "xlink"
}

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

{elements}

}} // namespace

#endif // __LIBMEI_{moduleNameCaps}_H__
"""

CLASS_FILE_END = """} // namespace
"""

#
# Templates for the attribute classes methods and members
#

ATTCLASS_H = """
//----------------------------------------------------------------------------
// Att{attGroupNameUpper}
//----------------------------------------------------------------------------

class Att{attGroupNameUpper} : public Att {{
public:
    Att{attGroupNameUpper}();
    virtual ~Att{attGroupNameUpper}();

    /** Reset the default values for the attribute class **/
    void Reset{attGroupNameUpper}();

    /** Read the values for the attribute class **/
    bool Read{attGroupNameUpper}(pugi::xml_node element);

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
{members}
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

{elements}

"""

ATTCLASS_CPP = """
//----------------------------------------------------------------------------
// Att{attGroupNameUpper}
//----------------------------------------------------------------------------

Att{attGroupNameUpper}::Att{attGroupNameUpper}() : Att()
{{
    Reset{attGroupNameUpper}();
}}

Att{attGroupNameUpper}::~Att{attGroupNameUpper}() {{}}

void Att{attGroupNameUpper}::Reset{attGroupNameUpper}()
{{
    {defaults}
}}

bool Att{attGroupNameUpper}::Read{attGroupNameUpper}(pugi::xml_node element)
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
        element.remove_attribute("{attNameLower}");
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
    bool Read(pugi::xml_node element);
    bool Write(pugi::xml_node element);
    void Reset();
}};
"""

ELEMENTCLASS_CPP = """{elementNameUpper}::{elementNameUpper}() :{attClasses}
{{
}}

{elementNameUpper}::~{elementNameUpper}() {{}}

bool {elementNameUpper}::Read(pugi::xml_node element)
{{
    bool hasAttribute = false;{elementRead}
    return hasAttribute;
}}

bool {elementNameUpper}::Write(pugi::xml_node element)
{{
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
};

} // namespace

#endif // __LIBMEI_ATT_CLASSES_H__
"""

#
# These templates generate the type definitions
#

TYPE_GRP_START = """

#ifndef __LIBMEI_ATT_TYPES_H__
#define __LIBMEI_ATT_TYPES_H__

//----------------------------------------------------------------------------

namespace {ns} {{

"""

TYPE_GRP_END = """} // namespace

#endif // __LIBMEI_ATT_TYPES_H__
"""

TYPE_START = """/**
 * MEI {meitype}
 */
enum {vrvtype} {{
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
// AttConverter
//----------------------------------------------------------------------------

class AttConverter {{
public:"""

CONVERTER_METHODS_H = """
    std::string {fname}ToStr({type} data) const;
    {type} StrTo{fname}(const std::string &value, bool logWarning = true) const;
"""

CONVERTER_END_H = """};

} // namespace

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
// AttConverter
//----------------------------------------------------------------------------
"""

CONVERTER_METHOD1_START_CPP = """
std::string AttConverter::{fname}ToStr({type} data) const
{{
    std::string value;
    switch (data) {{"""

CONVERTER_METHOD2_START_CPP = """
{type} AttConverter::StrTo{fname}(const std::string &value, bool logWarning) const
{{"""

CONVERTER_METHOD1_CPP = """
        case {value}: value = "{string}"; break;"""

CONVERTER_METHOD2_CPP = """
    if (value == "{string}") return {value};"""

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
} // namespace
"""

#
# These templates generate a module level static method for setting attribute on an unspecified Object
#

ATTMODULE_FILE_H = """{license}

#ifndef __LIBMEI_ATT_MODULE_H__
#define __LIBMEI_ATT_MODULE_H__

#include <string>
#include <vector>
#include <utility>

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
    AttModule() {{}};
    virtual ~AttModule() {{}};
    ///@}}

    /**
     * @name static method for blind attribute reading or modification
     */
    ///@{{
"""

SETTERS_GETTERS_H = """    static bool Set{moduleNameCap}(Object *element, const std::string &attrType, const std::string &attrValue);
    static void Get{moduleNameCap}(const Object *element, ArrayOfStrAttr *attributes);
    
"""

ATTMODULE_FILE_END_H = """    ///@}

};

} // namespace

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

}} // namespace

"""


def vrv_member_cc(name):
    cc = "".join([n[0].upper() + n[1:] for n in name.split(".")])
    return cc[0].lower() + cc[1:]


def vrv_member_cc_upper(name):
    return "".join([n[0].upper() + n[1:] for n in name.split(".")])


def vrv_converter_cc(name):
    [l, r] = name.split('_', 1)
    r = "".join([n[0].upper() + n[1:].lower() for n in r.split("_")])
    if l == "data":
        return r
    return l[0].upper() + l[1:] + r


# globals
TEI_RNG_NS = {"tei": "http://www.tei-c.org/ns/1.0",
              "rng": "http://relaxng.org/ns/structure/1.0"}


def vrv_load_config():
    """Load the vrv attribute overrides into CONFIG."""
    global CONFIG

    config = Path("tools/", "config.yml")
    if not config.exists():
        lg.debug("Config file could not be found")
        return

    f = config.open("r")
    CONFIG = yaml.load(f, Loader=yaml.FullLoader)


def vrv_get_att_config(module, gp, att):
    if not module in CONFIG["modules"] or not gp in CONFIG["modules"][module]:
        return None
    if not att in CONFIG["modules"][module][gp]:
        return None
    return CONFIG["modules"][module][gp][att]


def vrv_get_type_default(type):
    if not type in CONFIG["defaults"]:
        return "{0}_NONE".format(re.sub(r'^data_', "", type))
    return CONFIG["defaults"][type]


def vrv_is_excluded_type(type):
    if not type in CONFIG["excludes"]:
        return False
    return True


def vrv_is_alternate_type(type):
    if not type in CONFIG["alternates"]:
        return False
    return True


def vrv_get_att_config_type(module, gp, att):
    """Get the att type."""
    att_config = vrv_get_att_config(module, gp, att)
    if att_config is None or "type" not in att_config:
        return None
    att = att_config["type"]
    return att


def vrv_get_att_config_default(module, gp, att):
    """Get the att default value."""
    att_config = vrv_get_att_config(module, gp, att)
    # nothing in the module/att
    if att_config is None or "default" not in att_config:
        return None
    # return the module/att default
    return att_config["default"]


def vrv_getformattedtype(type):
    if type in CONFIG["mapped"]:
        return CONFIG["mapped"][type]
    return type.replace(".", "_")


def vrv_getformattedvallist(att, vallist):
    return "{0}_{1}".format(vrv_member_cc(att.replace("att.", "")), vallist.upper().replace(".", "").replace(":", ""))


def vrv_getatttype(schema, module, gp, aname):
    """Returns the attribute type for element name, or string if not detectable."""

    # Look up if there is an override for this type in the current module, and return it
    # Note that we do not honor pseudo-hungarian notation
    attype = vrv_get_att_config_type(module, gp, aname)
    if attype:
        return attype

    # No override, get it from the schema
    definition = schema.xpath("//tei:classSpec[@ident=$gp]/tei:attList/tei:attDef[@ident=$name]", gp=gp, name=aname, namespaces=TEI_RNG_NS)
    if not len(definition):
        return "std::string"
    # First numbers
    el = definition[0].xpath("tei:datatype/tei:dataRef/@name|tei:datatype/rng:data/@type", name=aname, namespaces=TEI_RNG_NS)
    if el:
        if el[0].endswith("nteger"):
            # We unify "integer", "positiveInteger", and "nonNegativeInteger"
            return "int"
        elif el[0] == "decimal":
            return "double"
    # The data types
    ref = definition[0].xpath("tei:datatype/tei:dataRef/@key|tei:datatype/rng:ref/@name", gp=gp, name=aname, namespaces=TEI_RNG_NS)
    if ref:
        return vrv_getformattedtype("{0}".format(ref[0]))
    # Finally from val lists
    vl = definition[0].find("tei:valList[@type=\"closed\"]", namespaces=TEI_RNG_NS)
    if vl is not None:
        element = vl.xpath("./ancestor::tei:classSpec", namespaces=TEI_RNG_NS)
        attName = vl.xpath("./parent::tei:attDef/@ident",
                           namespaces=TEI_RNG_NS)
        if element:
            return vrv_getformattedvallist(element[0].get("ident"), attName[0])
            #data_list = "{0}.{1}".format(element[0].get("ident"),attName[0])
        # elif attName:
        #    elName = vl[0].xpath("./ancestor::tei:elementSpec/@ident", namespaces=TEI_RNG_NS)
        #    lg.debug("VALLIST {0} --- {1}".format(elName[0],attName[0]))

    # Otherwise as string
    return "std::string"


def vrv_getattdefault(schema, module, gp, aname):
    """Returns the attribute default value for element name, or string if not detectable."""

    attype = vrv_getatttype(schema, module, gp, aname)
    default = vrv_get_att_config_default(module, gp, aname)

    if attype == "int":
        if default == None:
            default = "VRV_UNSET"
        return (default, ["StrToInt", "IntToStr"])
    elif attype == "char":
        if default == None:
            default = 0
        return (default, ["StrToInt", "IntToStr"])
    elif attype == "double":
        if default == None:
            default = 0.0
        return (default, ["StrToDbl", "DblToStr"])
    elif attype == "std::string":
        return ("\"\"", ["StrToStr", "StrToStr"])
    else:
        if default == None:
            default = vrv_get_type_default(attype)
        cname = vrv_converter_cc(attype)
        return (default, [f"StrTo{cname}", f"{cname}ToStr"])


def create(cpp_ns, schema, outdir, elements=False):
    lg.debug("Begin Verovio C++ Output ...")

    vrv_load_config()
    
    # if we output element classes, then we do not output the attmodule class
    if elements: 
        __create_element_classes(cpp_ns, schema, outdir)
    else:
        __create_att_module(cpp_ns, schema, outdir)

    __create_att_classes(cpp_ns, schema, outdir)

    lg.debug("Success!")


def __get_docstr(text, indent=0):
    """
        Format a docstring. Take the first sentence (. followed by a space)
        and use it for the brief. Then put the rest of the text after a blank
        line if there is text there
    """
    text = text.strip()
    dotpos = text.find(". ")
    if dotpos > 0:
        brief = text[:dotpos+1]
        content = text[dotpos+2:]
    else:
        brief = text
        content = ""
    if indent == 0:
        istr = ""
    else:
        istr = "{0:{1}}".format(" ", indent)

    if len(brief) > 1:
        brief = brief[0].upper() + brief[1:]
    else:
        brief = "---"
    brief = "\n{0} * ".format(istr).join(textwrap.wrap(brief, 80))
    content = "\n{0} * ".format(istr).join(textwrap.wrap(content, 80))
    docstr = "{0}/**".format(istr)
    if len(content) > 0 or brief.count("\n"):
        docstr += "\n{0} *".format(istr)
    docstr += " {0}".format(brief)
    if brief.count("\n") and len(content) == 0:
        docstr += "\n{0}".format(istr)
    if len(content) > 0:
        docstr += "\n{0} * {1}\n{0}".format(istr, content)
    docstr += " **/".format(istr)
    return docstr


def __create_att_classes(cpp_ns, schema, outdir):
    lg.debug("Creating AttClass Headers.")
    ###########################################################################
    # Header
    ###########################################################################
    enum = ""
    for module, atgroup in sorted(schema.attribute_group_structure.items()):
        fullout = ""
        classes = ""
        methods = ""

        if not atgroup:
            # continue if we don't have any attribute groups in this module
            continue

        for gp, atts in sorted(atgroup.items()):
            if not atts:
                continue

            members = ""
            methods = ""
            for att in atts:
                if len(att.split("|")) > 1:
                    ns, att = att.split("|")
                atttype = vrv_getatttype(schema.schema, module, gp, att)
                docstr = __get_docstr(schema.get_att_desc(att), indent=4)
                substrings = {
                    "attNameUpper": schema.cc(schema.strpatt(att)),
                    "attNameLower": att,
                    "attNameLowerJoined": vrv_member_cc(att),
                    "documentation": docstr,
                    "attType": atttype
                }
                if len(methods) > 0:
                    methods += "//\n"
                methods += ATTCLASS_METHODS_H.format(**substrings)
                if (vrv_is_alternate_type(atttype)):
                    methods += ATTCLASS_ALTERNATE_METHODS_H.format(**substrings)
                members += ATTCLASS_MEMBERS_H.format(**substrings)

            clsubstr = {
                "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                "methods": methods,
                "members": members,
                "attNameLower": "att{0}".format(att)
            }
            classes += ATTCLASS_H.format(**clsubstr)
            enum += "    ATT_{0},\n".format(schema.cc(schema.strpatt(gp)).upper())

        tplvars = {
            "includes": "#include <string>",
            'license': LICENSE.format(authors=AUTHORS),
            'moduleNameCaps': "ATTS_{0}".format(module.upper()),
            'elements': classes.strip(),
            'ns': cpp_ns
        }

        fullout = CLASS_FILE_H.format(**tplvars)
        f_att_class_h = Path(outdir, "atts_{0}.h".format(module.lower()))
        f_att_class_h.write_text(fullout)
        lg.debug("\tCreated atts_{0}.h".format(module.lower()))

    lg.debug("Creating AttClass Implementations")
    ###########################################################################
    # Implementation
    ###########################################################################
    for module, atgroup in sorted(schema.attribute_group_structure.items()):
        fullout = ""
        classes = ""
        methods = ""

        if not atgroup:
            continue

        for gp, atts in sorted(atgroup.items()):
            if not atts:
                continue
            methods = ""
            defaults = ""
            reads = ""
            writes = ""
            checkers = ""
 
            for att in atts:
                if len(att.split("|")) > 1:
                    # we have a namespaced attribute
                    ns, att = att.split("|")
                    prefix = NS_PREFIX_MAP[ns]
                else:
                    prefix = ""
                atttype = vrv_getatttype(schema.schema, module, gp, att)
                attdefault, converters = vrv_getattdefault(schema.schema, module, gp, att)

                attsubstr = {
                    "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                    "attNameUpper": schema.cc(att),
                    "attNameLower": "{0}:{1}".format(prefix, att) if prefix != "" else "{0}".format(att),
                    "attNameLowerJoined": vrv_member_cc(att),
                    "attDefault": attdefault,
                    "converterRead": converters[0],
                    "converterWrite": converters[1]
                }
                if len(defaults) > 0:
                    defaults += "\n    "
                    reads += "\n    "
                    writes += "\n    "
                defaults += ATTCLASS_MEMBERS_DEFAULT_CPP.format(**attsubstr)
                reads += ATTCLASS_READ_CPP.format(**attsubstr)
                writes += ATTCLASS_WRITE_CPP.format(**attsubstr)
                if (vrv_is_alternate_type(atttype)):
                    checkers += ATTCLASS_CHECK_ALTERNATE_CPP.format(**attsubstr)
                else:
                    checkers += ATTCLASS_CHECK_CPP.format(**attsubstr)

            clsubstr = {
                "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                "defaults": defaults,
                "reads": reads,
                "writes": writes,
                "checkers": checkers,
                "attNameLower": "att{0}".format(att)
            }
            classes += ATTCLASS_CPP.format(**clsubstr)

        tplvars = {
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
            "moduleNameLower": "atts_{0}".format(module.lower()),
            "moduleNameCap": format(module.capitalize()),
            "elements": classes.strip()
        }
        fullout = ATTCLASS_FILE_CPP.format(**tplvars)
        f_att_class_cpp = open(Path(outdir, "atts_{0}.cpp".format(module.lower())), "w")
        f_att_class_cpp.write(fullout)
        f_att_class_cpp.write(CLASS_FILE_END)
        f_att_class_cpp.close()
        lg.debug("\tCreated atts_{0}.cpp".format(module.lower()))

    lg.debug("Writing classes enum")
    ###########################################################################
    # Classes enum
    ###########################################################################
    tplvars = {
        "ns": cpp_ns,
    }
    f_att_classes_h = open(Path(outdir, "attclasses.h"), "w")
    f_att_classes_h.write(LICENSE)
    f_att_classes_h.write(ENUM_GRP_START.format(**tplvars))
    f_att_classes_h.write(enum)
    f_att_classes_h.write(ENUM_GRP_END)
    f_att_classes_h.close()
    lg.debug("\tCreated attclasses.h")

    lg.debug("Writing data types and lists")
    ###########################################################################
    # Types enum
    ###########################################################################
    tplvars = {
        "ns": cpp_ns
    }
    f_att_types_h = open(Path(outdir, "atttypes.h"), "w")
    f_att_types_h.write(LICENSE)
    f_att_types_h.write(TYPE_GRP_START.format(**tplvars))

    for data_type, values in sorted(schema.data_types.items()):
        if vrv_is_excluded_type(data_type) == True:
            lg.debug("Skipping excluded {0}".format(data_type))
            continue

        if vrv_is_alternate_type(data_type) == True:
            lg.debug("Skipping alternate {0}".format(data_type))
            continue

        vstr = ""
        val_prefix = vrv_getformattedtype(data_type).replace("data_", "")
        tpsubstr = {
            "meitype": data_type,
            "vrvtype": vrv_getformattedtype(data_type),
            "val_prefix":  val_prefix
        }
        vstr += TYPE_START.format(**tpsubstr)
        for v in values:
            tpsubstr = {
                "val_prefix": val_prefix,
                "value": v.replace('.', '_').replace('-', '_').replace(',', '_').replace('+', 'plus')
            }
            vstr += TYPE_VALUE.format(**tpsubstr)

        tpsubstr = {
            "val_prefix": val_prefix,
            "test": "test"
        }
        vstr += TYPE_END.format(**tpsubstr)
        f_att_types_h.write(vstr)

    for list_type, values in sorted(schema.data_lists.items()):
        if vrv_is_excluded_type(list_type) == True:
            lg.debug("Skipping {0}".format(list_type))
            continue

        vstr = ""

        val_prefix = vrv_getformattedvallist(list_type.rsplit('@')[0], list_type.rsplit('@')[1])
        tpsubstr = {
            "meitype": list_type,
            "vrvtype": val_prefix,
            "val_prefix":  val_prefix
        }
        vstr += TYPE_START.format(**tpsubstr)
        for v in values:
            tpsubstr = {
                "val_prefix": val_prefix,
                "value": v.replace('.', '_').replace('-', '_').replace(',', '_').replace('+', 'plus')
            }
            vstr += TYPE_VALUE.format(**tpsubstr)

        tpsubstr = {
            "val_prefix": val_prefix
        }
        vstr += TYPE_END.format(**tpsubstr)
        f_att_types_h.write(vstr)

    f_att_types_h.write(TYPE_GRP_END)

    f_att_types_h.close()
    lg.debug("\tCreated atttypes.h")

    lg.debug("Writing libmei att converter class")
    ###########################################################################
    # Classes enum
    ###########################################################################
    f_att_converter_h = open(Path(outdir, "attconverter.h"), "w")
    f_att_converter_h.write(LICENSE)
    f_att_converter_h.write(CONVERTER_H.format(**{ "ns": cpp_ns }))
    f_att_converter_cpp = open(Path(outdir, "attconverter.cpp"), "w")
    f_att_converter_cpp.write(LICENSE)
    f_att_converter_cpp.write(CONVERTER_CPP.format(**{ "ns": cpp_ns }))

    for data_type, values in sorted(schema.data_types.items()):
        if vrv_is_excluded_type(data_type) == True:
            lg.debug("Skipping excluded {0}".format(data_type))
            continue

        if vrv_is_alternate_type(data_type) == True:
            lg.debug("Skipping alternate {0}".format(data_type))
            continue

        vrvtype = vrv_getformattedtype(data_type)
        val_prefix = vrvtype.replace("data_", "")
        vrvfname = vrv_converter_cc(vrvtype)
        tpsubstr = {
            "type": vrvtype,
            "fname": vrvfname
        }
        vstr = CONVERTER_METHODS_H.format(**tpsubstr)
        f_att_converter_h.write(vstr)

        vstr1 = CONVERTER_METHOD1_START_CPP.format(**tpsubstr)
        vstr2 = CONVERTER_METHOD2_START_CPP.format(**tpsubstr)
        for v in values:
            tpsubstr = {
                "value": "{0}_{1}".format(val_prefix, v.replace('.', '_').replace('-', '_').replace(',', '_').replace('+', 'plus')),
                "string": v
            }
            vstr1 += CONVERTER_METHOD1_CPP.format(**tpsubstr)
            vstr2 += CONVERTER_METHOD2_CPP.format(**tpsubstr)

        tpsubstr = {
            "prefix": val_prefix,
            "type": data_type
        }
        vstr1 += CONVERTER_METHOD1_END_CPP.format(**tpsubstr)
        vstr2 += CONVERTER_METHOD2_END_CPP.format(**tpsubstr)
        f_att_converter_cpp.write(vstr1)
        f_att_converter_cpp.write(vstr2)

    for list_type, values in sorted(schema.data_lists.items()):
        if vrv_is_excluded_type(list_type) == True:
            lg.debug("Skipping {0}".format(list_type))
            continue

        val_prefix = vrv_getformattedvallist(list_type.rsplit('@')[0], list_type.rsplit('@')[1])
        vrvtype = val_prefix
        vrvfname = vrv_converter_cc(vrvtype)
        tpsubstr = {
            "type": vrvtype,
            "fname": vrvfname
        }
        vstr = CONVERTER_METHODS_H.format(**tpsubstr)
        f_att_converter_h.write(vstr)

        vstr1 = CONVERTER_METHOD1_START_CPP.format(**tpsubstr)
        vstr2 = CONVERTER_METHOD2_START_CPP.format(**tpsubstr)
        for v in values:
            tpsubstr = {
                "value": "{0}_{1}".format(val_prefix, v.replace('.', '_').replace('-', '_').replace(',', '_').replace('+', 'plus')),
                "string": v
            }
            vstr1 += CONVERTER_METHOD1_CPP.format(**tpsubstr)
            vstr2 += CONVERTER_METHOD2_CPP.format(**tpsubstr)

        tpsubstr = {
            "prefix": val_prefix,
            "type": list_type
        }
        vstr1 += CONVERTER_METHOD1_END_CPP.format(**tpsubstr)
        vstr2 += CONVERTER_METHOD2_END_CPP.format(**tpsubstr)
        f_att_converter_cpp.write(vstr1)
        f_att_converter_cpp.write(vstr2)

    f_att_converter_h.write(CONVERTER_END_H)
    f_att_converter_h.close()
    f_att_converter_cpp.write(CONVERTER_END_CPP)
    f_att_converter_cpp.close()
    lg.debug("\tCreated attconverter.h/cpp")

def __create_element_classes(cpp_ns, schema, outdir):
    lg.debug("Creating Element Headers")
    ###########################################################################
    # Header
    ###########################################################################
    for module, elements in sorted(schema.element_structure.items()):
        element_output = ""

        if not elements:
            continue
        includes = []
        for element, atgroups in sorted(elements.items()):
            element_attClasses = ""
            for attribute in atgroups:
                if isinstance(attribute, list):
                    # it's our self-defined attributes.
                    lg.debug("Skipping attributes within class")
                else:
                    element_attClasses += ", public Att{attNameUpper} ".format(
                        attNameUpper=schema.cc(schema.strpatt(attribute)))

                    # figure out includes
                    if attribute in list(schema.inverse_attribute_group_structure.keys()):
                        mod = schema.inverse_attribute_group_structure[attribute]
                        if mod not in includes:
                            includes.append(mod)

            docstr = __get_docstr(schema.get_elem_desc(element), indent=0)
            elvars = {
                "elementNameUpper": schema.cc(element),
                "attClasses": element_attClasses,
                "documentation": docstr.strip(),
                "elementName": element
            }

            element_output += ELEMENTCLASS_H.format(**elvars)

        incl_output = ""
        incl_output += "#include \"element.h\"\n"
        for incl in sorted(includes):
            incl_output += "#include \"atts_{0}.h\"\n".format(incl.lower())

        outvars = {
            "includes": incl_output,
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
            "moduleNameCaps": (module.upper()).replace("-", "_"),
            "elements": element_output.strip()
        }
        if "std::string" in element_output:
            outvars["includes"] += "#include <string>\n"

        fullout = CLASS_FILE_H.format(**outvars)
        f_element_class_h = open(Path(outdir, f"{module.lower()}.h"), "w")
        f_element_class_h.write(fullout)
        f_element_class_h.close()
        lg.debug(f"\tCreated {module.lower()}.h")

    lg.debug("Creating Element Implementations")
    # ###########################################################################
    # # Implementation
    # ###########################################################################
    for module, elements in sorted(schema.element_structure.items()):
        element_output = ""
        element_constructor = ""

        if not elements:
            continue

        for element, atgroups in sorted(elements.items()):
            element_attClasses = "\n    Element(\"{0}\")".format(element)
            element_read = ""
            element_write = ""
            element_reset = ""
            for attribute in atgroups:
                if isinstance(attribute, list):
                    lg.debug("Skipping attributes within class")
                else:
                    att_str = schema.cc(schema.strpatt(attribute))
                    element_attClasses += ", Att{0}()".format(att_str)
                    element_read += "\n    hasAttribute = (Read{0}(element) || hasAttribute);".format(att_str)
                    element_write += "\n    hasAttribute = (Write{0}(element) || hasAttribute);".format(att_str)
                    element_reset += "\n    Reset{0}();".format(att_str)
            #element_attClass = element_attClass.rstrip(",\n")

            consvars = {
                'elementNameUpper': schema.cc(element),
                'elementNameLower': element,
                'attClasses': element_attClasses,
                'elementRead': element_read,
                'elementWrite': element_write,
                'elementReset': element_reset,
            }
            element_constructor += ELEMENTCLASS_CPP.format(
                **consvars)

        implvars = {
            'moduleNameLower': module.lower(),
            'elements': element_constructor,
            'license': LICENSE.format(authors=AUTHORS),
            'ns': cpp_ns
        }
        fullout = ELEMENTCLASS_FILE_H.format(**implvars)
        fullout += CLASS_FILE_END

        f_element_class_cpp = open(Path(outdir, f"{module.lower()}.cpp"), "w")
        f_element_class_cpp.write(fullout)
        f_element_class_cpp.close()
        lg.debug(f"\t Created {module.lower()}.cpp")

def __create_att_module(cpp_ns, schema, outdir):
    lg.debug("Creating AttModule Header and Implementation.")
    ###########################################################################
    # Getter and setters files
    ###########################################################################
    tplvars = {
        'license': LICENSE.format(authors=AUTHORS),
        'ns': cpp_ns
    }
    f_att_module_h = open(Path(outdir, "attmodule.h"), "w")
    f_att_module_h.write(ATTMODULE_FILE_H.format(**tplvars))

    tplvars = {
        'license': LICENSE.format(authors=AUTHORS),
    }
    f_att_module_cpp = open(Path(outdir, "attmodule.cpp"), "w")
    f_att_module_cpp.write(ATTMODULE_FILE_CPP.format(**tplvars))

    for module, atgroup in sorted(schema.attribute_group_structure.items()):
        setters = ""
        getters = ""

        if not atgroup:
            continue

        for gp, atts in sorted(atgroup.items()):
            if not atts:
                continue
            setters += SETTERS_GRP_START_CPP.format(**{
                                                              "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                                                              "attId": "ATT_{0}".format(schema.cc(schema.strpatt(gp)).upper())})
            getters += GETTERS_GRP_START_CPP.format(**{
                                                              "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                                                              "attId": "ATT_{0}".format(schema.cc(schema.strpatt(gp)).upper())})

            for att in atts:
                if len(att.split("|")) > 1:
                    # we have a namespaced attribute
                    ns, att = att.split("|")
                    prefix = NS_PREFIX_MAP[ns]
                else:
                    prefix = ""
                attdefault, converters = vrv_getattdefault(schema.schema, module, gp, att)

                attsubstr = {
                    "attGroupNameUpper": schema.cc(schema.strpatt(gp)),
                    "attNameUpper": schema.cc(att),
                    "attNameLower": "{0}:{1}".format(prefix, att) if prefix != "" else "{0}".format(att),
                    "attNameLowerJoined": vrv_member_cc(att),
                    "converterRead": converters[0],
                    "converterWrite": converters[1]
                }
                setters += SETTERS_GRP_CPP.format(**attsubstr)
                getters += GETTERS_GRP_CPP.format(**attsubstr)

            setters += SETTERS_GRP_END_CPP.format(**attsubstr)
            getters += GETTERS_GRP_END_CPP.format(**attsubstr)

        tplvars = {
            "license": LICENSE.format(authors=AUTHORS),
            "ns": cpp_ns,
            "moduleNameLower": "atts_{0}".format(module.lower()),
            "moduleNameCap": format(module.capitalize()),
        }

        f_att_module_h.write(SETTERS_GETTERS_H.format(**tplvars))
        f_att_module_cpp.write(SETTERS_START_CPP.format(**tplvars))
        f_att_module_cpp.write(setters)
        f_att_module_cpp.write(SETTERS_END_CPP.format(**tplvars))
        f_att_module_cpp.write(GETTERS_START_CPP.format(**tplvars))
        f_att_module_cpp.write(getters)
        f_att_module_cpp.write(GETTERS_END_CPP.format(**tplvars))

    f_att_module_h.write(ATTMODULE_FILE_END_H)
    f_att_module_h.close()
    lg.debug(f"\t Created attmodule.h/cpp")
    f_att_module_cpp.close()

def __copy_codefile(directory, codefile):
    # files in ./addons are required.
    # These are the only files to be edited by hand.
    # When the output directory is not ".", they need to be copied
    # When the namespace option is specified, the vrv namespace need to be adjusted when copying
    lg.debug("Todo - copy and adjust namespace")
