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

#include "atts_critapp.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

AttCrit::AttCrit() : Att()
{
    this->ResetCrit();
}

void AttCrit::ResetCrit()
{
    m_cause = "";
}

bool AttCrit::ReadCrit(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("cause")) {
        this->SetCause(StrToStr(element.attribute("cause").value()));
        if (removeAttr) element.remove_attribute("cause");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCrit::WriteCrit(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasCause()) {
        element.append_attribute("cause") = StrToStr(this->GetCause()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCrit::HasCause() const
{
    return (m_cause != "");
}

} // namespace vrv
