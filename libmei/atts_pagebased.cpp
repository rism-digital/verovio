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

#include "atts_pagebased.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttSurface
//----------------------------------------------------------------------------

AttSurface::AttSurface() : Att()
{
    ResetSurface();
}

AttSurface::~AttSurface()
{
}

void AttSurface::ResetSurface()
{
    m_surface = "";
}

bool AttSurface::ReadSurface(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("surface")) {
        this->SetSurface(StrToStr(element.attribute("surface").value()));
        element.remove_attribute("surface");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttSurface::WriteSurface(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasSurface()) {
        element.append_attribute("surface") = StrToStr(this->GetSurface()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttSurface::HasSurface() const
{
    return (m_surface != "");
}

/* include <attsurface> */

bool Att::SetPagebased(Object *element, const std::string &attrType, const std::string &attrValue)
{
    if (element->HasAttClass(ATT_SURFACE)) {
        AttSurface *att = dynamic_cast<AttSurface *>(element);
        assert(att);
        if (attrType == "surface") {
            att->SetSurface(att->StrToStr(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetPagebased(const Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_SURFACE)) {
        const AttSurface *att = dynamic_cast<const AttSurface *>(element);
        assert(att);
        if (att->HasSurface()) {
            attributes->push_back(std::make_pair("surface", att->StrToStr(att->GetSurface())));
        }
    }
}

} // vrv namespace
