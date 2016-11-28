/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////

#include "atts_externalsymbols.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttExtsym
//----------------------------------------------------------------------------

AttExtsym::AttExtsym() : Att()
{
    ResetExtsym();
}

AttExtsym::~AttExtsym()
{
}

void AttExtsym::ResetExtsym()
{
    m_glyphname = "";
    m_glyphnum = "";
}

bool AttExtsym::ReadExtsym(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("glyphname")) {
        this->SetGlyphname(StrToStr(element.attribute("glyphname").value()));
        element.remove_attribute("glyphname");
        hasAttribute = true;
    }
    if (element.attribute("glyphnum")) {
        this->SetGlyphnum(StrToStr(element.attribute("glyphnum").value()));
        element.remove_attribute("glyphnum");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExtsym::WriteExtsym(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGlyphname()) {
        element.append_attribute("glyphname") = StrToStr(this->GetGlyphname()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGlyphnum()) {
        element.append_attribute("glyphnum") = StrToStr(this->GetGlyphnum()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExtsym::HasGlyphname() const
{
    return (m_glyphname != "");
}

bool AttExtsym::HasGlyphnum() const
{
    return (m_glyphnum != "");
}

/* include <attglyphnum> */

bool Att::SetExternalsymbols(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        AttExtsym *att = dynamic_cast<AttExtsym *>(element);
        assert(att);
        if (attrType == "glyphname") {
            att->SetGlyphname(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "glyphnum") {
            att->SetGlyphnum(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetExternalsymbols(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        const AttExtsym *att = dynamic_cast<const AttExtsym *>(element);
        assert(att);
        if (att->HasGlyphname()) {
            attributes->push_back(std::make_pair("glyphname", att->StrToStr(att->GetGlyphname())));
        }
        if (att->HasGlyphnum()) {
            attributes->push_back(std::make_pair("glyphnum", att->StrToStr(att->GetGlyphnum())));
        }
    }
}

} // vrv namespace
