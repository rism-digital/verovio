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

#include "atts_mei.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttNotationType
//----------------------------------------------------------------------------

AttNotationType::AttNotationType() : Att()
{
    this->ResetNotationType();
}

void AttNotationType::ResetNotationType()
{
    m_notationtype = NOTATIONTYPE_NONE;
    m_notationsubtype = "";
}

bool AttNotationType::ReadNotationType(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("notationtype")) {
        this->SetNotationtype(StrToNotationtype(element.attribute("notationtype").value()));
        if (removeAttr) element.remove_attribute("notationtype");
        hasAttribute = true;
    }
    if (element.attribute("notationsubtype")) {
        this->SetNotationsubtype(StrToStr(element.attribute("notationsubtype").value()));
        if (removeAttr) element.remove_attribute("notationsubtype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNotationType::WriteNotationType(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasNotationtype()) {
        element.append_attribute("notationtype") = NotationtypeToStr(this->GetNotationtype()).c_str();
        wroteAttribute = true;
    }
    if (this->HasNotationsubtype()) {
        element.append_attribute("notationsubtype") = StrToStr(this->GetNotationsubtype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNotationType::HasNotationtype() const
{
    return (m_notationtype != NOTATIONTYPE_NONE);
}

bool AttNotationType::HasNotationsubtype() const
{
    return (m_notationsubtype != "");
}

} // namespace vrv
