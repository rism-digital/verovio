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

#include "object.h"

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
        this->SetColspan(StrToInt(element.attribute("colspan").value()));
        hasAttribute = true;
    }
    if (element.attribute("rowspan")) {
        this->SetRowspan(StrToInt(element.attribute("rowspan").value()));
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTabular::WriteTabular(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasColspan()) {
        element.append_attribute("colspan") = IntToStr(this->GetColspan()).c_str();
        wroteAttribute = true;
    }
    if (this->HasRowspan()) {
        element.append_attribute("rowspan") = IntToStr(this->GetRowspan()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttTabular::HasColspan( )
{
    return (m_colspanInt != 0);
}

bool AttTabular::HasRowspan( )
{
    return (m_rowspanInt != 0);
}


/* include <attrowspan> */

bool Att::SetFigtable( Object *element, std::string attrType, std::string attrValue )
{
    if ( (attrType == "colspanInt") && dynamic_cast<AttTabular*>(element) ) {
        AttTabular *att = dynamic_cast<AttTabular*>(element);
        att->SetColspan(att->StrToInt(attrValue));
    return true;
    }
    if ( (attrType == "rowspanInt") && dynamic_cast<AttTabular*>(element) ) {
        AttTabular *att = dynamic_cast<AttTabular*>(element);
        att->SetRowspan(att->StrToInt(attrValue));
    return true;
    }

    return false;
}


} // vrv namespace
    