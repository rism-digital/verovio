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
// AttRegularMethod
//----------------------------------------------------------------------------

AttRegularMethod::AttRegularMethod() : Att()
{
    ResetRegularMethod();
}

AttRegularMethod::~AttRegularMethod()
{
}

void AttRegularMethod::ResetRegularMethod()
{
    m_method = regularMethod_METHOD_NONE;
}

bool AttRegularMethod::ReadRegularMethod(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("method")) {
        this->SetMethod(StrToRegularMethodMethod(element.attribute("method").value()));
        element.remove_attribute("method");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttRegularMethod::WriteRegularMethod(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasMethod()) {
        element.append_attribute("method") = RegularMethodMethodToStr(this->GetMethod()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttRegularMethod::HasMethod() const
{
    return (m_method != regularMethod_METHOD_NONE);
}

/* include <attmethod> */

bool Att::SetHeader(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        AttRegularMethod *att = dynamic_cast<AttRegularMethod *>(element);
        assert(att);
        if (attrType == "method") {
            att->SetMethod(att->StrToRegularMethodMethod(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHeader(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_REGULARMETHOD)) {
        const AttRegularMethod *att = dynamic_cast<const AttRegularMethod *>(element);
        assert(att);
        if (att->HasMethod()) {
            attributes->push_back(std::make_pair("method", att->RegularMethodMethodToStr(att->GetMethod())));
        }
    }
}

} // vrv namespace
