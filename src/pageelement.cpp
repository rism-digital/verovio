/////////////////////////////////////////////////////////////////////////////
// Name:        pageelement.cpp
// Author:      Laurent Pugin
// Created:     2021
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "pageelement.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "mdiv.h"
#include "page.h"

namespace vrv {

//----------------------------------------------------------------------------
// PageElement
//----------------------------------------------------------------------------

PageElement::PageElement() : Object(PAGE_ELEMENT, "pe"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

PageElement::PageElement(ClassId classId) : Object(classId, "pe"), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

PageElement::PageElement(ClassId classId, const std::string &classIdStr) : Object(classId, classIdStr), AttTyped()
{
    RegisterAttClass(ATT_TYPED);

    Reset();
}

PageElement::~PageElement() {}

void PageElement::Reset()
{
    Object::Reset();
    ResetTyped();
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int PageElement::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    assert(params->m_page);
    this->MoveItselfTo(params->m_page);

    return FUNCTOR_CONTINUE;
}

int PageElement::CastOffPages(FunctorParams *functorParams)
{
    CastOffPagesParams *params = vrv_params_cast<CastOffPagesParams *>(functorParams);
    assert(params);

    // Hidden mdivs must be ignored
    if (this->Is(MDIV)) {
        Mdiv *mdiv = vrv_cast<Mdiv *>(this);
        assert(mdiv);
        if (mdiv->m_visibility == Hidden) {
            return FUNCTOR_CONTINUE;
        }
    }

    PageElement *element = dynamic_cast<PageElement *>(params->m_contentPage->Relinquish(this->GetIdx()));
    assert(element);
    // move as pending since we want it at the beginning of the page in case of system break coming
    params->m_pendingPageElements.push_back(element);

    return FUNCTOR_CONTINUE;
}

int PageElement::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentPage);

    return FUNCTOR_SIBLINGS;
}

int PageElement::UnCastOff(FunctorParams *functorParams)
{
    UnCastOffParams *params = vrv_params_cast<UnCastOffParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_page);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
