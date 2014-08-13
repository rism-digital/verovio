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

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttCrit
//----------------------------------------------------------------------------

AttCrit::AttCrit(): Att() {
    ResetCrit();
}

AttCrit::~AttCrit() {

}

void AttCrit::ResetCrit() {
    m_cause = "";
}

bool AttCrit::ReadCrit(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("cause")) {
        this->SetCause(StrToStr(element.attribute("cause").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttCrit::WriteCrit(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasCause()) {
        element.append_attribute("cause") = StrToStr(this->GetCause()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttCrit::HasCause( )
{
    return (m_cause != "");
}


/* include <attcause> */

//----------------------------------------------------------------------------
// AttSource
//----------------------------------------------------------------------------

AttSource::AttSource(): Att() {
    ResetSource();
}

AttSource::~AttSource() {

}

void AttSource::ResetSource() {
    m_source = "";
}

bool AttSource::ReadSource(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("source")) {
        this->SetSource(StrToStr(element.attribute("source").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSource::WriteSource(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasSource()) {
        element.append_attribute("source") = StrToStr(this->GetSource()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSource::HasSource( )
{
    return (m_source != "");
}


/* include <attsource> */

} // vrv namespace

