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
#include "scoredef.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Section
//----------------------------------------------------------------------------

Section::Section() : SystemElement("section-"), BoundaryStartInterface(), AttCommon(), AttCommonPart()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

Section::~Section()
{
}

void Section::Reset()
{
    SystemElement::Reset();
    BoundaryStartInterface::Reset();
    ResetCommon();
    ResetCommonPart();
}

void Section::AddChild(Object *child)
{
    if (child->Is() == ENDING) {
        assert(dynamic_cast<Ending *>(child));
    }
    else if (child->Is() == MEASURE) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is() == PB) {
        assert(dynamic_cast<Pb *>(child));
    }
    else if (child->Is() == SB) {
        assert(dynamic_cast<Sb *>(child));
    }
    else if (child->Is() == SCOREDEF) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Pb
//----------------------------------------------------------------------------

Pb::Pb() : SystemElement("pb-"), AttCommon(), AttCommonPart()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

Pb::~Pb()
{
}

void Pb::Reset()
{
    SystemElement::Reset();
    ResetCommon();
    ResetCommonPart();
}

//----------------------------------------------------------------------------
// Sb
//----------------------------------------------------------------------------

Sb::Sb() : SystemElement("pb-"), AttCommon(), AttCommonPart()
{
    RegisterAttClass(ATT_COMMON);
    RegisterAttClass(ATT_COMMONPART);

    Reset();
}

Sb::~Sb()
{
}

void Sb::Reset()
{
    SystemElement::Reset();
    ResetCommon();
    ResetCommonPart();
}

//----------------------------------------------------------------------------
// Section functor methods
//----------------------------------------------------------------------------

int Section::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int Section::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    ConvertToPageBasedBoundary(this, params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int Section::PrepareBoundaries(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfacePrepareBoundaries(functorParams);
    }

    return FUNCTOR_CONTINUE;
}

int Section::ResetDrawing(FunctorParams *functorParams)
{
    if (this->IsBoundary()) {
        this->BoundaryStartInterface::InterfaceResetDrawing(functorParams);
    }

    return FUNCTOR_CONTINUE;
};

int Section::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    Section *section = dynamic_cast<Section *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(section);

    return FUNCTOR_SIBLINGS;
}

int Section::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// Pb functor methods
//----------------------------------------------------------------------------

int Pb::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int Pb::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    Pb *pb = dynamic_cast<Pb *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(pb);

    return FUNCTOR_SIBLINGS;
}

int Pb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    if (!params->m_firstPbProcessed) {
        params->m_firstPbProcessed = true;
    }
    else {

        params->m_currentPage = new Page();
        params->m_doc->AddChild(params->m_currentPage);
        params->m_currentSystem = new System();
        params->m_currentPage->AddChild(params->m_currentSystem);
    }

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

//----------------------------------------------------------------------------
// Sb functor methods
//----------------------------------------------------------------------------

int Sb::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = dynamic_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    this->MoveItselfTo(params->m_pageBasedSystem);

    return FUNCTOR_CONTINUE;
}

int Sb::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    Sb *sb = dynamic_cast<Sb *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(sb);

    return FUNCTOR_SIBLINGS;
}

int Sb::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = dynamic_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    params->m_currentSystem = new System();
    params->m_currentPage->AddChild(params->m_currentSystem);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv