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

#include "atts_tablature.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttNoteGesTablature
//----------------------------------------------------------------------------

AttNoteGesTablature::AttNoteGesTablature(): Att()
{
    ResetNoteGesTablature();
}

AttNoteGesTablature::~AttNoteGesTablature()
{
}

void AttNoteGesTablature::ResetNoteGesTablature()
{
    m_tabFret = FRETNUMBER_NONE;
    m_tabString = STRINGNUMBER_NONE;
}

bool AttNoteGesTablature::ReadNoteGesTablature(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tab.fret")) {
        this->SetTabFret(StrToFretnumber(element.attribute("tab.fret").value()));
        element.remove_attribute("tab.fret");
        hasAttribute = true;
    }
    if (element.attribute("tab.string")) {
        this->SetTabString(StrToStringnumber(element.attribute("tab.string").value()));
        element.remove_attribute("tab.string");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNoteGesTablature::WriteNoteGesTablature(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabFret()) {
        element.append_attribute("tab.fret") = FretnumberToStr(this->GetTabFret()).c_str();
        wroteAttribute = true;
    }
    if (this->HasTabString()) {
        element.append_attribute("tab.string") = StringnumberToStr(this->GetTabString()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNoteGesTablature::HasTabFret()
{
    return (m_tabFret != FRETNUMBER_NONE);
}

bool AttNoteGesTablature::HasTabString()
{
    return (m_tabString != STRINGNUMBER_NONE);
}


/* include <atttab.string> */

//----------------------------------------------------------------------------
// AttStaffDefGesTablature
//----------------------------------------------------------------------------

AttStaffDefGesTablature::AttStaffDefGesTablature(): Att()
{
    ResetStaffDefGesTablature();
}

AttStaffDefGesTablature::~AttStaffDefGesTablature()
{
}

void AttStaffDefGesTablature::ResetStaffDefGesTablature()
{
    m_tabStrings = "";
}

bool AttStaffDefGesTablature::ReadStaffDefGesTablature(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("tab.strings")) {
        this->SetTabStrings(StrToStr(element.attribute("tab.strings").value()));
        element.remove_attribute("tab.strings");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttStaffDefGesTablature::WriteStaffDefGesTablature(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasTabStrings()) {
        element.append_attribute("tab.strings") = StrToStr(this->GetTabStrings()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttStaffDefGesTablature::HasTabStrings()
{
    return (m_tabStrings != "");
}


/* include <atttab.strings> */

bool Att::SetTablature(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_NOTEGESTABLATURE)) {
        AttNoteGesTablature *att = dynamic_cast<AttNoteGesTablature*>(element);
        assert(att);
        if (attrType == "tab.fret") {
            att->SetTabFret(att->StrToFretnumber(attrValue));
            return true;
        }
        if (attrType == "tab.string") {
            att->SetTabString(att->StrToStringnumber(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFGESTABLATURE)) {
        AttStaffDefGesTablature *att = dynamic_cast<AttStaffDefGesTablature*>(element);
        assert(att);
        if (attrType == "tab.strings") {
            att->SetTabStrings(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetTablature(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_NOTEGESTABLATURE)) {
        AttNoteGesTablature *att = dynamic_cast<AttNoteGesTablature*>(element);
        assert(att);
        if (att->HasTabFret()) {
            attributes->push_back(std::make_pair("tab.fret", att->FretnumberToStr(att->GetTabFret())));
        }
        if (att->HasTabString()) {
            attributes->push_back(std::make_pair("tab.string", att->StringnumberToStr(att->GetTabString())));
        }
    }
    if (element->HasAttClass(ATT_STAFFDEFGESTABLATURE)) {
        AttStaffDefGesTablature *att = dynamic_cast<AttStaffDefGesTablature*>(element);
        assert(att);
        if (att->HasTabStrings()) {
            attributes->push_back(std::make_pair("tab.strings", att->StrToStr(att->GetTabStrings())));
        }
    }

}
    
} // vrv namespace
    
