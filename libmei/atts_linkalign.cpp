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

#include "atts_linkalign.h"

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {
    
//----------------------------------------------------------------------------
// AttAlignment
//----------------------------------------------------------------------------

AttAlignment::AttAlignment(): Att() {
    ResetAlignment();
}

AttAlignment::~AttAlignment() {

}

void AttAlignment::ResetAlignment() {
    m_when = "";
}

bool AttAlignment::ReadAlignment(  pugi::xml_node element ) {
    bool hasAttribute = false;
    if (element.attribute("when")) {
        this->SetWhen(StrToStr(element.attribute("when").value()));
        element.remove_attribute("when");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAlignment::WriteAlignment(  pugi::xml_node element ) {
    bool wroteAttribute = false;
    if (this->HasWhen()) {
        element.append_attribute("when") = StrToStr(this->GetWhen()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAlignment::HasWhen( )
{
    return (m_when != "");
}


/* include <attwhen> */

bool Att::SetLinkalign( Object *element, std::string attrType, std::string attrValue ) {
    if (dynamic_cast<AttAlignment*>(element) ) {
        AttAlignment *att = dynamic_cast<AttAlignment*>(element);
        if (attrType == "when") {
            att->SetWhen(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetLinkalign( Object *element, ArrayOfStrAttr *attributes ) {
    if (dynamic_cast<AttAlignment*>(element) ) {
        AttAlignment *att = dynamic_cast<AttAlignment*>(element);
        if (att->HasWhen()) {
            attributes->push_back(std::make_pair("when", att->StrToStr(att->GetWhen())));
        }
    }

}
    
} // vrv namespace
    
