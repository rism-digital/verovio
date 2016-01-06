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

#include "atts_neumes.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttIneumeLog
//----------------------------------------------------------------------------

AttIneumeLog::AttIneumeLog(): Att()
{
    ResetIneumeLog();
}

AttIneumeLog::~AttIneumeLog()
{
}

void AttIneumeLog::ResetIneumeLog()
{
    m_form = INEUMEFORM_NONE;
    m_name = INEUMENAME_NONE;
}

bool AttIneumeLog::ReadIneumeLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToIneumeform(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToIneumename(element.attribute("name").value()));
        element.remove_attribute("name");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIneumeLog::WriteIneumeLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = IneumeformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasName()) {
        element.append_attribute("name") = IneumenameToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIneumeLog::HasForm()
{
    return (m_form != INEUMEFORM_NONE);
}

bool AttIneumeLog::HasName()
{
    return (m_name != INEUMENAME_NONE);
}


/* include <attname> */

//----------------------------------------------------------------------------
// AttUneumeLog
//----------------------------------------------------------------------------

AttUneumeLog::AttUneumeLog(): Att()
{
    ResetUneumeLog();
}

AttUneumeLog::~AttUneumeLog()
{
}

void AttUneumeLog::ResetUneumeLog()
{
    m_form = UNEUMEFORM_NONE;
    m_name = UNEUMENAME_NONE;
}

bool AttUneumeLog::ReadUneumeLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToUneumeform(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToUneumename(element.attribute("name").value()));
        element.remove_attribute("name");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttUneumeLog::WriteUneumeLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = UneumeformToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasName()) {
        element.append_attribute("name") = UneumenameToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttUneumeLog::HasForm()
{
    return (m_form != UNEUMEFORM_NONE);
}

bool AttUneumeLog::HasName()
{
    return (m_name != UNEUMENAME_NONE);
}


/* include <attname> */

bool Att::SetNeumes(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_INEUMELOG)) {
        AttIneumeLog *att = dynamic_cast<AttIneumeLog*>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToIneumeform(attrValue));
            return true;
        }
        if (attrType == "name") {
            att->SetName(att->StrToIneumename(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_UNEUMELOG)) {
        AttUneumeLog *att = dynamic_cast<AttUneumeLog*>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToUneumeform(attrValue));
            return true;
        }
        if (attrType == "name") {
            att->SetName(att->StrToUneumename(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetNeumes(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_INEUMELOG)) {
        AttIneumeLog *att = dynamic_cast<AttIneumeLog*>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->IneumeformToStr(att->GetForm())));
        }
        if (att->HasName()) {
            attributes->push_back(std::make_pair("name", att->IneumenameToStr(att->GetName())));
        }
    }
    if (element->HasAttClass(ATT_UNEUMELOG)) {
        AttUneumeLog *att = dynamic_cast<AttUneumeLog*>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->UneumeformToStr(att->GetForm())));
        }
        if (att->HasName()) {
            attributes->push_back(std::make_pair("name", att->UneumenameToStr(att->GetName())));
        }
    }

}
    
} // vrv namespace
    
