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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

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
    m_method = regularmethod_METHOD_NONE;
}

bool AttRegularmethod::ReadRegularmethod(pugi::xml_node element) {
    bool hasAttribute = false;
    if (element.attribute("method")) {
        this->SetMethod(StrToRegularmethodMethod(element.attribute("method").value()));
        element.remove_attribute("method");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRegularmethod::WriteRegularmethod(pugi::xml_node element) {
    bool wroteAttribute = false;
    if (this->HasMethod()) {
        element.append_attribute("method") = RegularmethodMethodToStr(this->GetMethod()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRegularmethod::HasMethod()
{
    return (m_method != regularmethod_METHOD_NONE);
}


/* include <attmethod> */

bool Att::SetHeader(Object *element, std::string attrType, std::string attrValue) {
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        AttRegularmethod *att = dynamic_cast<AttRegularmethod*>(element);
        assert(att);
        if (attrType == "method") {
            att->SetMethod(att->StrToRegularmethodMethod(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHeader(Object *element, ArrayOfStrAttr *attributes) {
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        AttRegularmethod *att = dynamic_cast<AttRegularmethod*>(element);
        assert(att);
        if (att->HasMethod()) {
            attributes->push_back(std::make_pair("method", att->RegularmethodMethodToStr(att->GetMethod())));
        }
    }

}
    
} // vrv namespace
    
