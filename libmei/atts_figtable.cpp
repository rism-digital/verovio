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

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttTabular
//----------------------------------------------------------------------------

AttTabular::AttTabular() : Att()
{
    ResetTabular();
}

AttTabular::~AttTabular()
{
}

void AttTabular::ResetTabular()
{
    m_colspan = 0;
    m_rowspan = 0;
}

bool AttTabular::ReadTabular(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("colspan")) {
        this->SetColspan(StrToInt(element.attribute("colspan").value()));
        element.remove_attribute("colspan");
        hasAttribute = true;
    }
    if (element.attribute("rowspan")) {
        this->SetRowspan(StrToInt(element.attribute("rowspan").value()));
        element.remove_attribute("rowspan");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttTabular::WriteTabular(pugi::xml_node element)
{
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

bool AttTabular::HasColspan() const
{
    return (m_colspan != 0);
}

bool AttTabular::HasRowspan() const
{
    return (m_rowspan != 0);
}

/* include <attrowspan> */

bool Att::SetFigtable(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_TABULAR)) {
        AttTabular *att = dynamic_cast<AttTabular *>(element);
        assert(att);
        if (attrType == "colspan") {
            att->SetColspan(att->StrToInt(attrValue));
            return true;
        }
        if (attrType == "rowspan") {
            att->SetRowspan(att->StrToInt(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetFigtable(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_TABULAR)) {
        const AttTabular *att = dynamic_cast<const AttTabular *>(element);
        assert(att);
        if (att->HasColspan()) {
            attributes->push_back(std::make_pair("colspan", att->IntToStr(att->GetColspan())));
        }
        if (att->HasRowspan()) {
            attributes->push_back(std::make_pair("rowspan", att->IntToStr(att->GetRowspan())));
        }
    }
}

} // vrv namespace
