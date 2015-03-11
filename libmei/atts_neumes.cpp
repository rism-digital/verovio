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

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttIneumeLog
//----------------------------------------------------------------------------

AttIneumeLog::AttIneumeLog(): Att() {
    ResetIneumeLog();
}

AttIneumeLog::~AttIneumeLog() {

}

void AttIneumeLog::ResetIneumeLog() {
    m_form = "";
    m_name = "";
}

bool AttIneumeLog::ReadIneumeLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToStr(element.attribute("name").value()));
        element.remove_attribute("name");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIneumeLog::WriteIneumeLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasName()) {
        element.append_attribute("name") = StrToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttIneumeLog::HasForm( )
{
    return (m_form != "");
}

bool AttIneumeLog::HasName( )
{
    return (m_name != "");
}


/* include <attname> */

//----------------------------------------------------------------------------
// AttUneumeLog
//----------------------------------------------------------------------------

AttUneumeLog::AttUneumeLog(): Att() {
    ResetUneumeLog();
}

AttUneumeLog::~AttUneumeLog() {

}

void AttUneumeLog::ResetUneumeLog() {
    m_form = "";
    m_name = "";
}

bool AttUneumeLog::ReadUneumeLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToStr(element.attribute("name").value()));
        element.remove_attribute("name");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttUneumeLog::WriteUneumeLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasName()) {
        element.append_attribute("name") = StrToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttUneumeLog::HasForm( )
{
    return (m_form != "");
}

bool AttUneumeLog::HasName( )
{
    return (m_name != "");
}


/* include <attname> */

bool Att::SetNeumes( Object *element, std::string attrType, std::string attrValue ) {
    if (dynamic_cast<AttIneumeLog*>(element) ) {
        AttIneumeLog *att = dynamic_cast<AttIneumeLog*>(element);
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "name") {
            att->SetName(att->StrToStr(attrValue));
            return true;
        }
    }
    if (dynamic_cast<AttUneumeLog*>(element) ) {
        AttUneumeLog *att = dynamic_cast<AttUneumeLog*>(element);
        if (attrType == "form") {
            att->SetForm(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "name") {
            att->SetName(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetNeumes( Object *element, ArrayOfStrAttr *attributes ) {
    if (dynamic_cast<AttIneumeLog*>(element) ) {
        AttIneumeLog *att = dynamic_cast<AttIneumeLog*>(element);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
        if (att->HasName()) {
            attributes->push_back(std::make_pair("name", att->StrToStr(att->GetName())));
        }
    }
    if (dynamic_cast<AttUneumeLog*>(element) ) {
        AttUneumeLog *att = dynamic_cast<AttUneumeLog*>(element);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->StrToStr(att->GetForm())));
        }
        if (att->HasName()) {
            attributes->push_back(std::make_pair("name", att->StrToStr(att->GetName())));
        }
    }

}
    
} // vrv namespace
    
