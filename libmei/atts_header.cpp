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

#include "atts_header.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttRegularmethod
//----------------------------------------------------------------------------

AttRegularmethod::AttRegularmethod(): Att() {
    ResetRegularmethod();
}

AttRegularmethod::~AttRegularmethod() {

}

void AttRegularmethod::ResetRegularmethod() {
    m_method = "";
}

bool AttRegularmethod::ReadRegularmethod(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("method")) {
        this->SetMethod(StrToStr(element.attribute("method").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRegularmethod::WriteRegularmethod(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasMethod()) {
        element.append_attribute("method") = StrToStr(this->GetMethod()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRegularmethod::HasMethod( )
{
    return (m_method != "");
}


/* include <attmethod> */

} // vrv namespace

