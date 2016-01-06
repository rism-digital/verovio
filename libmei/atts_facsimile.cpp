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

#include "atts_facsimile.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "object.h"

/* #include_block */

namespace vrv {

//----------------------------------------------------------------------------
// AttFacsimile
//----------------------------------------------------------------------------

AttFacsimile::AttFacsimile(): Att()
{
    ResetFacsimile();
}

AttFacsimile::~AttFacsimile()
{
}

void AttFacsimile::ResetFacsimile()
{
    m_facs = URIS_NONE;
}

bool AttFacsimile::ReadFacsimile(pugi::xml_node element)
{
    bool hasAttribute = false;
    if (element.attribute("facs")) {
        this->SetFacs(StrToUris(element.attribute("facs").value()));
        element.remove_attribute("facs");
        hasAttribute = true;
    }
    return hasAttribute;
}

bool AttFacsimile::WriteFacsimile(pugi::xml_node element)
{
    bool wroteAttribute = false;
    if (this->HasFacs()) {
        element.append_attribute("facs") = UrisToStr(this->GetFacs()).c_str();
        wroteAttribute = true;
    }
    return wroteAttribute;
}

bool AttFacsimile::HasFacs()
{
    return (m_facs != URIS_NONE);
}


/* include <attfacs> */

bool Att::SetFacsimile(Object *element, std::string attrType, std::string attrValue)
{
    if (element->HasAttClass(ATT_FACSIMILE)) {
        AttFacsimile *att = dynamic_cast<AttFacsimile*>(element);
        assert(att);
        if (attrType == "facs") {
            att->SetFacs(att->StrToUris(attrValue));
            return true;
        }
    }

    return false;
}

void Att::GetFacsimile(Object *element, ArrayOfStrAttr *attributes)
{
    if (element->HasAttClass(ATT_FACSIMILE)) {
        AttFacsimile *att = dynamic_cast<AttFacsimile*>(element);
        assert(att);
        if (att->HasFacs()) {
            attributes->push_back(std::make_pair("facs", att->UrisToStr(att->GetFacs())));
        }
    }

}

} // vrv namespace

