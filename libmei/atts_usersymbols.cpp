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

#include "atts_usersymbols.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttAltSym
//----------------------------------------------------------------------------

AttAltSym::AttAltSym() : Att()
{
    ResetAltSym();
}

AttAltSym::~AttAltSym()
{
}

void AttAltSym::ResetAltSym()
{
    m_altsym = "";
}

bool AttAltSym::ReadAltSym(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("altsym")) {
        this->SetAltsym(StrToStr(element.attribute("altsym").value()));
        element.remove_attribute("altsym");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAltSym::WriteAltSym(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAltsym()) {
        element.append_attribute("altsym") = StrToStr(this->GetAltsym()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAltSym::HasAltsym() const
{
    return (m_altsym != "");
}

/* include <attaltsym> */

//----------------------------------------------------------------------------
// AttAnchoredTextLog
//----------------------------------------------------------------------------

AttAnchoredTextLog::AttAnchoredTextLog() : Att()
{
    ResetAnchoredTextLog();
}

AttAnchoredTextLog::~AttAnchoredTextLog()
{
}

void AttAnchoredTextLog::ResetAnchoredTextLog()
{
    m_func = "";
}

bool AttAnchoredTextLog::ReadAnchoredTextLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAnchoredTextLog::WriteAnchoredTextLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAnchoredTextLog::HasFunc() const
{
    return (m_func != "");
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttCurveLog
//----------------------------------------------------------------------------

AttCurveLog::AttCurveLog() : Att()
{
    ResetCurveLog();
}

AttCurveLog::~AttCurveLog()
{
}

void AttCurveLog::ResetCurveLog()
{
    m_func = "";
}

bool AttCurveLog::ReadCurveLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCurveLog::WriteCurveLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCurveLog::HasFunc() const
{
    return (m_func != "");
}

/* include <attfunc> */

//----------------------------------------------------------------------------
// AttLineLog
//----------------------------------------------------------------------------

AttLineLog::AttLineLog() : Att()
{
    ResetLineLog();
}

AttLineLog::~AttLineLog()
{
}

void AttLineLog::ResetLineLog()
{
    m_func = "";
}

bool AttLineLog::ReadLineLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        element.remove_attribute("func");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttLineLog::WriteLineLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFunc()) {
        element.append_attribute("func") = StrToStr(this->GetFunc()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttLineLog::HasFunc() const
{
    return (m_func != "");
}

/* include <attfunc> */

bool Att::SetUsersymbols(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_ALTSYM)) {
        AttAltSym *att = dynamic_cast<AttAltSym *>(element);
        assert(att);
        if (attrType == "altsym") {
            att->SetAltsym(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_ANCHOREDTEXTLOG)) {
        AttAnchoredTextLog *att = dynamic_cast<AttAnchoredTextLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_CURVELOG)) {
        AttCurveLog *att = dynamic_cast<AttCurveLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_LINELOG)) {
        AttLineLog *att = dynamic_cast<AttLineLog *>(element);
        assert(att);
        if (attrType == "func") {
            att->SetFunc(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetUsersymbols(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_ALTSYM)) {
        const AttAltSym *att = dynamic_cast<const AttAltSym *>(element);
        assert(att);
        if (att->HasAltsym()) {
            attributes->push_back(std::make_pair("altsym", att->StrToStr(att->GetAltsym())));
        }
    }
    if (element->HasAttClass(ATT_ANCHOREDTEXTLOG)) {
        const AttAnchoredTextLog *att = dynamic_cast<const AttAnchoredTextLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_CURVELOG)) {
        const AttCurveLog *att = dynamic_cast<const AttCurveLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
    if (element->HasAttClass(ATT_LINELOG)) {
        const AttLineLog *att = dynamic_cast<const AttLineLog *>(element);
        assert(att);
        if (att->HasFunc()) {
            attributes->push_back(std::make_pair("func", att->StrToStr(att->GetFunc())));
        }
    }
}

} // vrv namespace
