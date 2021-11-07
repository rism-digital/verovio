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

#include "atts_harmony.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttHarmLog
//----------------------------------------------------------------------------

AttHarmLog::AttHarmLog() : Att()
{
    ResetHarmLog();
}

AttHarmLog::~AttHarmLog()
{
}

void AttHarmLog::ResetHarmLog()
{
    m_chordref = "";
}

bool AttHarmLog::ReadHarmLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("chordref")) {
        this->SetChordref(StrToStr(element.attribute("chordref").value()));
        element.remove_attribute("chordref");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttHarmLog::WriteHarmLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasChordref()) {
        element.append_attribute("chordref") = StrToStr(this->GetChordref()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttHarmLog::HasChordref() const
{
    return (m_chordref != "");
}

/* include <attchordref> */

bool Att::SetHarmony(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_HARMLOG)) {
        AttHarmLog *att = dynamic_cast<AttHarmLog *>(element);
        assert(att);
        if (attrType == "chordref") {
            att->SetChordref(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetHarmony(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_HARMLOG)) {
        const AttHarmLog *att = dynamic_cast<const AttHarmLog *>(element);
        assert(att);
        if (att->HasChordref()) {
            attributes->push_back(std::make_pair("chordref", att->StrToStr(att->GetChordref())));
        }
    }
}

} // vrv namespace
