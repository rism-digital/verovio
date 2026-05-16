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

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAltSym
//----------------------------------------------------------------------------

AttAltSym::AttAltSym() : Att()
{
    this->ResetAltSym();
}

void AttAltSym::ResetAltSym()
{
    m_altsym = "";
}

bool AttAltSym::ReadAltSym(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("altsym")) {
        this->SetAltsym(StrToStr(element.attribute("altsym").value()));
        if (removeAttr) element.remove_attribute("altsym");
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

//----------------------------------------------------------------------------
// AttAnchoredTextLog
//----------------------------------------------------------------------------

AttAnchoredTextLog::AttAnchoredTextLog() : Att()
{
    this->ResetAnchoredTextLog();
}

void AttAnchoredTextLog::ResetAnchoredTextLog()
{
    m_func = "";
}

bool AttAnchoredTextLog::ReadAnchoredTextLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttCurveLog
//----------------------------------------------------------------------------

AttCurveLog::AttCurveLog() : Att()
{
    this->ResetCurveLog();
}

void AttCurveLog::ResetCurveLog()
{
    m_func = "";
}

bool AttCurveLog::ReadCurveLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

//----------------------------------------------------------------------------
// AttLineLog
//----------------------------------------------------------------------------

AttLineLog::AttLineLog() : Att()
{
    this->ResetLineLog();
}

void AttLineLog::ResetLineLog()
{
    m_func = "";
}

bool AttLineLog::ReadLineLog(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("func")) {
        this->SetFunc(StrToStr(element.attribute("func").value()));
        if (removeAttr) element.remove_attribute("func");
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

} // namespace vrv
