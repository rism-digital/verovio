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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

AttCrit::AttCrit() : Att()
{
    ResetCrit();
}

AttCrit::~AttCrit()
{
}

void AttCrit::ResetCrit()
{
    m_cause = "";
}

bool AttCrit::ReadCrit(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("cause")) {
        this->SetCause(StrToStr(element.attribute("cause").value()));
        element.remove_attribute("cause");
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

bool AttCrit::HasCause()
{
    return (m_cause != "");
}

/* include <attcause> */

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

AttSource::AttSource() : Att()
{
    ResetSource();
}

AttSource::~AttSource()
{
}

void AttSource::ResetSource()
{
    m_source = "";
}

bool AttSource::ReadSource(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("source")) {
        this->SetSource(StrToStr(element.attribute("source").value()));
        element.remove_attribute("source");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSource::WriteSource(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSource()) {
        element.append_attribute("source") = StrToStr(this->GetSource()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSource::HasSource()
{
    return (m_source != "");
}

/* include <attsource> */

bool Att::SetCritapp(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_CRIT)) {
        AttCrit *att = vrv_cast(AttCrit *)(element);
        assert(att);
        if (attrType == "cause") {
            att->SetCause(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_SOURCE)) {
        AttSource *att = vrv_cast(AttSource *)(element);
        assert(att);
        if (attrType == "source") {
            att->SetSource(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetCritapp(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_CRIT)) {
        AttCrit *att = vrv_cast(AttCrit *)(element);
        assert(att);
        if (att->HasCause()) {
            attributes->push_back(std::make_pair("cause", att->StrToStr(att->GetCause())));
        }
    }
    if (element->HasAttClass(ATT_SOURCE)) {
        AttSource *att = vrv_cast(AttSource *)(element);
        assert(att);
        if (att->HasSource()) {
            attributes->push_back(std::make_pair("source", att->StrToStr(att->GetSource())));
        }
    }
}

} // vrv namespace
