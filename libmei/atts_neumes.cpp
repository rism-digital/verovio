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

#include "atts_neumes.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttNeumeLog
//----------------------------------------------------------------------------

AttNeumeLog::AttNeumeLog() : Att()
{
    ResetNeumeLog();
}

AttNeumeLog::~AttNeumeLog()
{
}

void AttNeumeLog::ResetNeumeLog()
{
    m_name = "";
}

bool AttNeumeLog::ReadNeumeLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("name")) {
        this->SetName(StrToStr(element.attribute("name").value()));
        element.remove_attribute("name");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttNeumeLog::WriteNeumeLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasName()) {
        element.append_attribute("name") = StrToStr(this->GetName()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttNeumeLog::HasName() const
{
    return (m_name != "");
}

/* include <attname> */

bool Att::SetNeumes(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_NEUMELOG)) {
        AttNeumeLog *att = dynamic_cast<AttNeumeLog *>(element);
        assert(att);
        if (attrType == "name") {
            att->SetName(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetNeumes(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_NEUMELOG)) {
        const AttNeumeLog *att = dynamic_cast<const AttNeumeLog *>(element);
        assert(att);
        if (att->HasName()) {
            attributes->push_back(std::make_pair("name", att->StrToStr(att->GetName())));
        }
    }
}

} // vrv namespace
