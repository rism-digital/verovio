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

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttMargins
//----------------------------------------------------------------------------

AttMargins::AttMargins() : Att()
{
    ResetMargins();
}

AttMargins::~AttMargins() {}

void AttMargins::ResetMargins()
{
    m_topmar = data_MEASUREMENTUNSIGNED();
    m_botmar = data_MEASUREMENTUNSIGNED();
    m_leftmar = data_MEASUREMENTUNSIGNED();
    m_rightmar = data_MEASUREMENTUNSIGNED();
}

bool AttMargins::ReadMargins(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("topmar")) {
        this->SetTopmar(StrToMeasurementunsigned(element.attribute("topmar").value()));
        element.remove_attribute("topmar");
        hasAttribute = true;
    }
    if (element.attribute("botmar")) {
        this->SetBotmar(StrToMeasurementunsigned(element.attribute("botmar").value()));
        element.remove_attribute("botmar");
        hasAttribute = true;
    }
    if (element.attribute("leftmar")) {
        this->SetLeftmar(StrToMeasurementunsigned(element.attribute("leftmar").value()));
        element.remove_attribute("leftmar");
        hasAttribute = true;
    }
    if (element.attribute("rightmar")) {
        this->SetRightmar(StrToMeasurementunsigned(element.attribute("rightmar").value()));
        element.remove_attribute("rightmar");
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

/* include <attrightmar> */

bool Att::SetPagebased(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_MARGINS)) {
        AttMargins *att = dynamic_cast<AttMargins *>(element);
        assert(att);
        if (attrType == "topmar") {
            att->SetTopmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "botmar") {
            att->SetBotmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "leftmar") {
            att->SetLeftmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
        if (attrType == "rightmar") {
            att->SetRightmar(att->StrToMeasurementunsigned(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetPagebased(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_MARGINS)) {
        const AttMargins *att = dynamic_cast<const AttMargins *>(element);
        assert(att);
        if (att->HasTopmar()) {
            attributes->push_back({ "topmar", att->MeasurementunsignedToStr(att->GetTopmar()) });
        }
        if (att->HasBotmar()) {
            attributes->push_back({ "botmar", att->MeasurementunsignedToStr(att->GetBotmar()) });
        }
        if (att->HasLeftmar()) {
            attributes->push_back({ "leftmar", att->MeasurementunsignedToStr(att->GetLeftmar()) });
        }
        if (att->HasRightmar()) {
            attributes->push_back({ "rightmar", att->MeasurementunsignedToStr(att->GetRightmar()) });
        }
    }
}

} // namespace vrv
