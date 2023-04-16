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

#include "atts_genetic.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttGeneticState
//----------------------------------------------------------------------------

AttGeneticState::AttGeneticState() : Att()
{
    ResetGeneticState();
}

AttGeneticState::~AttGeneticState()
{
}

void AttGeneticState::ResetGeneticState()
{
    m_instant = "";
    m_state = "";
}

bool AttGeneticState::ReadGeneticState(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("instant")) {
        this->SetInstant(StrToStr(element.attribute("instant").value()));
        element.remove_attribute("instant");
        hasAttribute = true;
    }
    if (element.attribute("state")) {
        this->SetState(StrToStr(element.attribute("state").value()));
        element.remove_attribute("state");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttGeneticState::WriteGeneticState(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasInstant()) {
        element.append_attribute("instant") = StrToStr(this->GetInstant()).c_str();
        wroteAttribute = true;
    }
    if (this->HasState()) {
        element.append_attribute("state") = StrToStr(this->GetState()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttGeneticState::HasInstant() const
{
    return (m_instant != "");
}

bool AttGeneticState::HasState() const
{
    return (m_state != "");
}

/* include <attstate> */

bool Att::SetGenetic(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_GENETICSTATE)) {
        AttGeneticState *att = dynamic_cast<AttGeneticState *>(element);
        assert(att);
        if (attrType == "instant") {
            att->SetInstant(att->StrToStr(attrValue));
            return true;
        }
        if (attrType == "state") {
            att->SetState(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetGenetic(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_GENETICSTATE)) {
        const AttGeneticState *att = dynamic_cast<const AttGeneticState *>(element);
        assert(att);
        if (att->HasInstant()) {
            attributes->push_back({ "instant", att->StrToStr(att->GetInstant()) });
        }
        if (att->HasState()) {
            attributes->push_back({ "state", att->StrToStr(att->GetState()) });
        }
    }
}

} // vrv namespace
