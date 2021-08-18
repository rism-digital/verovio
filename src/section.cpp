/////////////////////////////////////////////////////////////////////////////
// Name:        section.h
// Author:      Laurent Pugin
// Created:     24/08/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "section.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "doc.h"
#include "editorial.h"
#include "ending.h"
#include "functorparams.h"
#include "measure.h"
#include "page.h"
#include "pages.h"
#include "scoredef.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

static const ClassRegistrar<Section> s_factory("section", SECTION);

Section::Section() : SystemElement("section-"), SystemElementStartInterface(), AttNNumberLike(), AttSectionVis()
{
    RegisterAttClass(ATT_NNUMBERLIKE);
    RegisterAttClass(ATT_SECTIONVIS);

    Reset();
}

Section::~Section() {}

void Section::Reset()
{
    SystemElement::Reset();
    SystemElementStartInterface::Reset();
    ResetNNumberLike();
    ResetSectionVis();
}

bool Section::IsSupportedChild(Object *child)
{
    if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Section functor methods
//----------------------------------------------------------------------------

int Section::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int Section::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    ConvertToPageBasedBoundary(this, params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int Section::ConvertToUnCastOffMensural(FunctorParams *functorParams)
{
    ConvertToUnCastOffMensuralParams *params = vrv_params_cast<ConvertToUnCastOffMensuralParams *>(functorParams);
    assert(params);

    params->m_contentMeasure = NULL;
    params->m_contentLayer = NULL;

    return FUNCTOR_CONTINUE;
}

int Section::PrepareBoundaries(FunctorParams *functorParams)
{
    if (this->IsSystemBoundary()) {
        this->SystemElementStartInterface::InterfacePrepareBoundaries(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Section::ResetDrawing(FunctorParams *functorParams)
{
    FloatingObject::ResetDrawing(functorParams);

    if (this->IsSystemBoundary()) {
        this->SystemElementStartInterface::InterfaceResetDrawing(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
