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

#include "atts_edittrans.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttAgentIdent
//----------------------------------------------------------------------------

AttAgentIdent::AttAgentIdent() : Att()
{
    ResetAgentIdent();
}

AttAgentIdent::~AttAgentIdent()
{
}

void AttAgentIdent::ResetAgentIdent()
{
    m_agent = "";
}

bool AttAgentIdent::ReadAgentIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("agent")) {
        this->SetAgent(StrToStr(element.attribute("agent").value()));
        element.remove_attribute("agent");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttAgentIdent::WriteAgentIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasAgent()) {
        element.append_attribute("agent") = StrToStr(this->GetAgent()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttAgentIdent::HasAgent() const
{
    return (m_agent != "");
}

/* include <attagent> */

//----------------------------------------------------------------------------
// AttReasonIdent
//----------------------------------------------------------------------------

AttReasonIdent::AttReasonIdent() : Att()
{
    ResetReasonIdent();
}

AttReasonIdent::~AttReasonIdent()
{
}

void AttReasonIdent::ResetReasonIdent()
{
    m_reason = "";
}

bool AttReasonIdent::ReadReasonIdent(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("reason")) {
        this->SetReason(StrToStr(element.attribute("reason").value()));
        element.remove_attribute("reason");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttReasonIdent::WriteReasonIdent(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasReason()) {
        element.append_attribute("reason") = StrToStr(this->GetReason()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttReasonIdent::HasReason() const
{
    return (m_reason != "");
}

/* include <attreason> */

bool Att::SetEdittrans(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_AGENTIDENT)) {
        AttAgentIdent *att = dynamic_cast<AttAgentIdent *>(element);
        assert(att);
        if (attrType == "agent") {
            att->SetAgent(att->StrToStr(attrValue));
            return true;
        }
    }
    if (element->HasAttClass(ATT_REASONIDENT)) {
        AttReasonIdent *att = dynamic_cast<AttReasonIdent *>(element);
        assert(att);
        if (attrType == "reason") {
            att->SetReason(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetEdittrans(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_AGENTIDENT)) {
        const AttAgentIdent *att = dynamic_cast<const AttAgentIdent *>(element);
        assert(att);
        if (att->HasAgent()) {
            attributes->push_back(std::make_pair("agent", att->StrToStr(att->GetAgent())));
        }
    }
    if (element->HasAttClass(ATT_REASONIDENT)) {
        const AttReasonIdent *att = dynamic_cast<const AttReasonIdent *>(element);
        assert(att);
        if (att->HasReason()) {
            attributes->push_back(std::make_pair("reason", att->StrToStr(att->GetReason())));
        }
    }
}

} // vrv namespace
