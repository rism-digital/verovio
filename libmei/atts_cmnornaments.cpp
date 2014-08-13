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

#include "atts_cmnornaments.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttMordentLog
//----------------------------------------------------------------------------

AttMordentLog::AttMordentLog(): Att() {
    ResetMordentLog();
}

AttMordentLog::~AttMordentLog() {

}

void AttMordentLog::ResetMordentLog() {
    m_form = "";
    m_long = "";
}

bool AttMordentLog::ReadMordentLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    if (element.attribute("long")) {
        this->SetLong(StrToStr(element.attribute("long").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttMordentLog::WriteMordentLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    if (this->HasLong()) {
        element.append_attribute("long") = StrToStr(this->GetLong()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttMordentLog::HasForm( )
{
    return (m_form != "");
}

bool AttMordentLog::HasLong( )
{
    return (m_long != "");
}


/* include <attlong> */

//----------------------------------------------------------------------------
// AttOrnam
//----------------------------------------------------------------------------

AttOrnam::AttOrnam(): Att() {
    ResetOrnam();
}

AttOrnam::~AttOrnam() {

}

void AttOrnam::ResetOrnam() {
    m_ornam = "";
}

bool AttOrnam::ReadOrnam(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("ornam")) {
        this->SetOrnam(StrToStr(element.attribute("ornam").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOrnam::WriteOrnam(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasOrnam()) {
        element.append_attribute("ornam") = StrToStr(this->GetOrnam()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOrnam::HasOrnam( )
{
    return (m_ornam != "");
}


/* include <attornam> */

//----------------------------------------------------------------------------
// AttOrnamentaccid
//----------------------------------------------------------------------------

AttOrnamentaccid::AttOrnamentaccid(): Att() {
    ResetOrnamentaccid();
}

AttOrnamentaccid::~AttOrnamentaccid() {

}

void AttOrnamentaccid::ResetOrnamentaccid() {
    m_accidupper = "";
    m_accidlower = "";
}

bool AttOrnamentaccid::ReadOrnamentaccid(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("accidupper")) {
        this->SetAccidupper(StrToStr(element.attribute("accidupper").value()));
        hasAttribute = true;
    }
    if (element.attribute("accidlower")) {
        this->SetAccidlower(StrToStr(element.attribute("accidlower").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttOrnamentaccid::WriteOrnamentaccid(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasAccidupper()) {
        element.append_attribute("accidupper") = StrToStr(this->GetAccidupper()).c_str();
        wroteAttribute = true;
    }
    if (this->HasAccidlower()) {
        element.append_attribute("accidlower") = StrToStr(this->GetAccidlower()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttOrnamentaccid::HasAccidupper( )
{
    return (m_accidupper != "");
}

bool AttOrnamentaccid::HasAccidlower( )
{
    return (m_accidlower != "");
}


/* include <attaccidlower> */

//----------------------------------------------------------------------------
// AttTurnLog
//----------------------------------------------------------------------------

AttTurnLog::AttTurnLog(): Att() {
    ResetTurnLog();
}

AttTurnLog::~AttTurnLog() {

}

void AttTurnLog::ResetTurnLog() {
    m_delayed = "";
    m_form = "";
}

bool AttTurnLog::ReadTurnLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("delayed")) {
        this->SetDelayed(StrToStr(element.attribute("delayed").value()));
        hasAttribute = true;
    }
    if (element.attribute("form")) {
        this->SetForm(StrToStr(element.attribute("form").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTurnLog::WriteTurnLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasDelayed()) {
        element.append_attribute("delayed") = StrToStr(this->GetDelayed()).c_str();
        wroteAttribute = true;
    }
    if (this->HasForm()) {
        element.append_attribute("form") = StrToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTurnLog::HasDelayed( )
{
    return (m_delayed != "");
}

bool AttTurnLog::HasForm( )
{
    return (m_form != "");
}


/* include <attform> */

} // vrv namespace

