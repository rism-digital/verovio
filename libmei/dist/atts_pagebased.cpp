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

#include "atts_pagebased.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// AttMargins
//----------------------------------------------------------------------------

AttMargins::AttMargins() : Att()
{
    this->ResetMargins();
}

void AttMargins::ResetMargins()
{
    m_topmar = data_MEASUREMENTUNSIGNED();
    m_botmar = data_MEASUREMENTUNSIGNED();
    m_leftmar = data_MEASUREMENTUNSIGNED();
    m_rightmar = data_MEASUREMENTUNSIGNED();
}

bool AttMargins::ReadMargins(pugi::xml_node element, bool removeAttr)
{
    bool hasAttribute = false;
    if (element.attribute("topmar")) {
        this->SetTopmar(StrToMeasurementunsigned(element.attribute("topmar").value()));
        if (removeAttr) element.remove_attribute("topmar");
        hasAttribute = true;
    }
    if (element.attribute("botmar")) {
        this->SetBotmar(StrToMeasurementunsigned(element.attribute("botmar").value()));
        if (removeAttr) element.remove_attribute("botmar");
        hasAttribute = true;
    }
    if (element.attribute("leftmar")) {
        this->SetLeftmar(StrToMeasurementunsigned(element.attribute("leftmar").value()));
        if (removeAttr) element.remove_attribute("leftmar");
        hasAttribute = true;
    }
    if (element.attribute("rightmar")) {
        this->SetRightmar(StrToMeasurementunsigned(element.attribute("rightmar").value()));
        if (removeAttr) element.remove_attribute("rightmar");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMargins::WriteMargins(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTopmar()) {
        element.append_attribute("topmar") = MeasurementunsignedToStr(this->GetTopmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasBotmar()) {
        element.append_attribute("botmar") = MeasurementunsignedToStr(this->GetBotmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLeftmar()) {
        element.append_attribute("leftmar") = MeasurementunsignedToStr(this->GetLeftmar()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRightmar()) {
        element.append_attribute("rightmar") = MeasurementunsignedToStr(this->GetRightmar()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMargins::HasTopmar() const
{
    return (m_topmar != data_MEASUREMENTUNSIGNED());
}

bool AttMargins::HasBotmar() const
{
    return (m_botmar != data_MEASUREMENTUNSIGNED());
}

bool AttMargins::HasLeftmar() const
{
    return (m_leftmar != data_MEASUREMENTUNSIGNED());
}

bool AttMargins::HasRightmar() const
{
    return (m_rightmar != data_MEASUREMENTUNSIGNED());
}

} // namespace vrv
