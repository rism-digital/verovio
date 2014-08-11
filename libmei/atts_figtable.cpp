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

#include "atts_figtable.h"

//----------------------------------------------------------------------------

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttTabular
//----------------------------------------------------------------------------

AttTabular::AttTabular(): Att() {
    ResetTabular();
}

AttTabular::~AttTabular() {

}

void AttTabular::ResetTabular() {
    m_colspanInt = 0;
    m_rowspanInt = 0;
}

bool AttTabular::ReadTabular(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("colspan")) {
        this->SetColspanInt(StrToInt(element.attribute("colspan").value()));
        hasAttribute = true;
    }
    if (element.attribute("rowspan")) {
        this->SetRowspanInt(StrToInt(element.attribute("rowspan").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTabular::WriteTabular(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->GetColspanInt() == 0) {
        element.append_attribute("colspan") = IntToStr(this->GetColspanInt()).c_str();
        wroteAttribute = true;
    }
    if (this->GetRowspanInt() == 0) {
        element.append_attribute("rowspan") = IntToStr(this->GetRowspanInt()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

/* include <attrowspan> */

} // vrv namespace

