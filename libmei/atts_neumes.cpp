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
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToStr(element.attribute("name").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttIneumeLog::WriteIneumeLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->GetName() == "") {
        element.append_attribute("name") = StrToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
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
        hasAttribute = true;
    }
    if (element.attribute("name")) {
        this->SetName(StrToStr(element.attribute("name").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttUneumeLog::WriteUneumeLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetForm() == "") {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->GetName() == "") {
        element.append_attribute("name") = StrToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attname> */

} // vrv namespace

