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

#include "atts_facsimile.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttFacsimile
//----------------------------------------------------------------------------

AttFacsimile::AttFacsimile(): Att() {
    ResetFacsimile();
}

AttFacsimile::~AttFacsimile() {

}

void AttFacsimile::ResetFacsimile() {
    m_facs = "";
}

bool AttFacsimile::ReadFacsimile(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("facs")) {
        this->SetFacs(StrToStr(element.attribute("facs").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFacsimile::WriteFacsimile(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetFacs() == "") {
        element.append_attribute("facs") = StrToStr(this->GetFacs()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

void AttFacsimile::getCoords() {
    return;
}

} // vrv namespace

