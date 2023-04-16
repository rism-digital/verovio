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

#include "atts_msdesc.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttComponentType
//----------------------------------------------------------------------------

AttComponentType::AttComponentType() : Att()
{
    ResetComponentType();
}

AttComponentType::~AttComponentType()
{
}

void AttComponentType::ResetComponentType()
{
    m_comptype = componentType_COMPTYPE_NONE;
}

bool AttComponentType::ReadComponentType(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("comptype")) {
        this->SetComptype(StrToComponentTypeComptype(element.attribute("comptype").value()));
        element.remove_attribute("comptype");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttComponentType::WriteComponentType(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasComptype()) {
        element.append_attribute("comptype") = ComponentTypeComptypeToStr(this->GetComptype()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttComponentType::HasComptype() const
{
    return (m_comptype != componentType_COMPTYPE_NONE);
}

/* include <attcomptype> */

bool Att::SetMsdesc(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_COMPONENTTYPE)) {
        AttComponentType *att = dynamic_cast<AttComponentType *>(element);
        assert(att);
        if (attrType == "comptype") {
            att->SetComptype(att->StrToComponentTypeComptype(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetMsdesc(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_COMPONENTTYPE)) {
        const AttComponentType *att = dynamic_cast<const AttComponentType *>(element);
        assert(att);
        if (att->HasComptype()) {
            attributes->push_back({ "comptype", att->ComponentTypeComptypeToStr(att->GetComptype()) });
        }
    }
}

} // vrv namespace
