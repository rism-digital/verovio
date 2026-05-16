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

#include "atts_figtable.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttTabular
//----------------------------------------------------------------------------

AttTabular::AttTabular() : Att()
{
    this->ResetTabular();
}

void AttTabular::ResetTabular()
{
    m_colspan = MEI_UNSET;
    m_rowspan = MEI_UNSET;
}

bool AttTabular::ReadTabular(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("colspan")) {
        this->SetColspan(StrToInt(element.attribute("colspan").value()));
        if (removeAttr) element.remove_attribute("colspan");
        hasAttribute = true;
    }
    if (element.attribute("rowspan")) {
        this->SetRowspan(StrToInt(element.attribute("rowspan").value()));
        if (removeAttr) element.remove_attribute("rowspan");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTabular::WriteTabular(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasColspan()) {
        element.append_attribute("colspan") = IntToStr(this->GetColspan()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRowspan()) {
        element.append_attribute("rowspan") = IntToStr(this->GetRowspan()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTabular::HasColspan() const
{
    return (m_colspan != MEI_UNSET);
}

bool AttTabular::HasRowspan() const
{
    return (m_rowspan != MEI_UNSET);
}

} // namespace vrv
