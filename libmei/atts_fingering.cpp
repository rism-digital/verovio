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

#include "atts_fingering.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttFingGrpLog
//----------------------------------------------------------------------------

AttFingGrpLog::AttFingGrpLog() : Att()
{
    ResetFingGrpLog();
}

AttFingGrpLog::~AttFingGrpLog()
{
}

void AttFingGrpLog::ResetFingGrpLog()
{
    m_form = fingGrpLog_FORM_NONE;
}

bool AttFingGrpLog::ReadFingGrpLog(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("form")) {
        this->SetForm(StrToFingGrpLogForm(element.attribute("form").value()));
        element.remove_attribute("form");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFingGrpLog::WriteFingGrpLog(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasForm()) {
        element.append_attribute("form") = FingGrpLogFormToStr(this->GetForm()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFingGrpLog::HasForm() const
{
    return (m_form != fingGrpLog_FORM_NONE);
}

/* include <attform> */

bool Att::SetFingering(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_FINGGRPLOG)) {
        AttFingGrpLog *att = dynamic_cast<AttFingGrpLog *>(element);
        assert(att);
        if (attrType == "form") {
            att->SetForm(att->StrToFingGrpLogForm(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetFingering(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_FINGGRPLOG)) {
        const AttFingGrpLog *att = dynamic_cast<const AttFingGrpLog *>(element);
        assert(att);
        if (att->HasForm()) {
            attributes->push_back(std::make_pair("form", att->FingGrpLogFormToStr(att->GetForm())));
        }
    }
}

} // vrv namespace
