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
// AttExtSym
//----------------------------------------------------------------------------

AttExtSym::AttExtSym() : Att()
{
    ResetExtSym();
}

AttExtSym::~AttExtSym()
{
}

void AttExtSym::ResetExtSym()
{
    m_glyphAuth = "";
    m_glyphName = "";
    m_glyphNum = 0;
    m_glyphUri = "";
}

bool AttExtSym::ReadExtSym(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("glyph.auth")) {
        this->SetGlyphAuth(StrToStr(element.attribute("glyph.auth").value()));
        element.remove_attribute("glyph.auth");
        hasAttribute = true;
    }
    if (element.attribute("glyph.name")) {
        this->SetGlyphName(StrToStr(element.attribute("glyph.name").value()));
        element.remove_attribute("glyph.name");
        hasAttribute = true;
    }
    if (element.attribute("glyph.num")) {
        this->SetGlyphNum(StrToHexnum(element.attribute("glyph.num").value()));
        element.remove_attribute("glyph.num");
        hasAttribute = true;
    }
    if (element.attribute("glyph.uri")) {
        this->SetGlyphUri(StrToStr(element.attribute("glyph.uri").value()));
        element.remove_attribute("glyph.uri");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttExtSym::WriteExtSym(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasGlyphAuth()) {
        element.append_attribute("glyph.auth") = StrToStr(this->GetGlyphAuth()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGlyphName()) {
        element.append_attribute("glyph.name") = StrToStr(this->GetGlyphName()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGlyphNum()) {
        element.append_attribute("glyph.num") = HexnumToStr(this->GetGlyphNum()).c_str();
        wroteAttribute = true;
    }
    if (this->HasGlyphUri()) {
        element.append_attribute("glyph.uri") = StrToStr(this->GetGlyphUri()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttExtSym::HasGlyphAuth() const
{
    return (m_glyphAuth != "");
}

bool AttExtSym::HasGlyphName() const
{
    return (m_glyphName != "");
}

bool AttExtSym::HasGlyphNum() const
{
    return (m_glyphNum != 0);
}

bool AttExtSym::HasGlyphUri() const
{
    return (m_glyphUri != "");
}

/* include <attglyph.uri> */

bool Att::SetExternalsymbols(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        AttExtSym *att = dynamic_cast<AttExtSym *>(element);
        assert(att);
        if (attrType == "glyph.auth") {
            att->SetGlyphAuth(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "glyph.name") {
            att->SetGlyphName(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "glyph.num") {
            att->SetGlyphNum(att->StrToHexnum(attrValue));
            return true;
        }
        if (attrType == "glyph.uri") {
            att->SetGlyphUri(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetExternalsymbols(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_EXTSYM)) {
        const AttExtSym *att = dynamic_cast<const AttExtSym *>(element);
        assert(att);
        if (att->HasGlyphAuth()) {
            attributes->push_back(std::make_pair("glyph.auth", att->StrToStr(att->GetGlyphAuth())));
        }
        if (att->HasGlyphName()) {
            attributes->push_back(std::make_pair("glyph.name", att->StrToStr(att->GetGlyphName())));
        }
        if (att->HasGlyphNum()) {
            attributes->push_back(std::make_pair("glyph.num", att->HexnumToStr(att->GetGlyphNum())));
        }
        if (att->HasGlyphUri()) {
            attributes->push_back(std::make_pair("glyph.uri", att->StrToStr(att->GetGlyphUri())));
        }
    }
}

} // vrv namespace
