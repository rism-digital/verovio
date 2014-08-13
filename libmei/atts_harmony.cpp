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

#include "atts_harmony.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttFretlocation
//----------------------------------------------------------------------------

AttFretlocation::AttFretlocation(): Att() {
    ResetFretlocation();
}

AttFretlocation::~AttFretlocation() {

}

void AttFretlocation::ResetFretlocation() {
    m_fret = "";
}

bool AttFretlocation::ReadFretlocation(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("fret")) {
        this->SetFret(StrToStr(element.attribute("fret").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFretlocation::WriteFretlocation(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasFret()) {
        element.append_attribute("fret") = StrToStr(this->GetFret()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFretlocation::HasFret( )
{
    return (m_fret != "");
}


/* include <attfret> */

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

AttHarmLog::AttHarmLog(): Att() {
    ResetHarmLog();
}

AttHarmLog::~AttHarmLog() {

}

void AttHarmLog::ResetHarmLog() {
    m_chordref = "";
}

bool AttHarmLog::ReadHarmLog(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("chordref")) {
        this->SetChordref(StrToStr(element.attribute("chordref").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmLog::WriteHarmLog(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasChordref()) {
        element.append_attribute("chordref") = StrToStr(this->GetChordref()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmLog::HasChordref( )
{
    return (m_chordref != "");
}


/* include <attchordref> */

//----------------------------------------------------------------------------
// AttHarmVis
//----------------------------------------------------------------------------

AttHarmVis::AttHarmVis(): Att() {
    ResetHarmVis();
}

AttHarmVis::~AttHarmVis() {

}

void AttHarmVis::ResetHarmVis() {
    m_extender = "";
    m_rendgrid = "";
}

bool AttHarmVis::ReadHarmVis(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("extender")) {
        this->SetExtender(StrToStr(element.attribute("extender").value()));
        hasAttribute = true;
    }
    if (element.attribute("rendgrid")) {
        this->SetRendgrid(StrToStr(element.attribute("rendgrid").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmVis::WriteHarmVis(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasExtender()) {
        element.append_attribute("extender") = StrToStr(this->GetExtender()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRendgrid()) {
        element.append_attribute("rendgrid") = StrToStr(this->GetRendgrid()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmVis::HasExtender( )
{
    return (m_extender != "");
}

bool AttHarmVis::HasRendgrid( )
{
    return (m_rendgrid != "");
}


/* include <attrendgrid> */

} // vrv namespace

