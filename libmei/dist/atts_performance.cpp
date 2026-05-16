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

#include "atts_performance.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttAlignment
//----------------------------------------------------------------------------

AttAlignment::AttAlignment() : Att()
{
    this->ResetAlignment();
}

void AttAlignment::ResetAlignment()
{
    m_when = "";
}

bool AttAlignment::ReadAlignment(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("when")) {
        this->SetWhen(StrToStr(element.attribute("when").value()));
        if (removeAttr) element.remove_attribute("when");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAlignment::WriteAlignment(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasWhen()) {
        element.append_attribute("when") = StrToStr(this->GetWhen()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAlignment::HasWhen() const
{
    return (m_when != "");
}

} // namespace vrv
