/////////////////////////////////////////////////////////////////////////////
// Name:        ending.h
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ending.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

Ending::Ending() : FloatingElement("ending-"), BoundaryStartInterface(), AttCommon()
{
    RegisterAttClass(ATT_COMMON);

    Reset();
}

Ending::~Ending()
{
}

void Ending::Reset()
{
    FloatingElement::Reset();
    BoundaryStartInterface::Reset();
    ResetCommon();
}

//----------------------------------------------------------------------------
// Ending functor methods
//----------------------------------------------------------------------------

int Ending::PrepareBoundaries(FunctorParams *functorParams)
{
    PrepareBoundariesParams *params = dynamic_cast<PrepareBoundariesParams *>(functorParams);
    assert(params);

    // Endings should always have an BoundaryEnd
    assert(this->IsBoundary());

    this->BoundaryStartInterface::InterfacePrepareBoundaries(functorParams);

    params->m_currentEnding = this;

    return FUNCTOR_CONTINUE;
}

int Ending::ResetDrawing(FunctorParams *functorParams)
{
    this->BoundaryStartInterface::InterfaceResetDrawing(functorParams);

    return FUNCTOR_CONTINUE;
};

int Ending::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = dynamic_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->m_parent));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    Ending *ending = dynamic_cast<Ending *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    params->m_currentSystem->AddEnding(ending);

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv