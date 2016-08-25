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

#include "editorial.h"
#include "functorparams.h"
#include "measure.h"
#include "scoredef.h"
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

void Ending::AddChild(Object *child)
{
    if (child->Is() == MEASURE) {
        assert(dynamic_cast<Measure *>(child));
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
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingObjects.push_back(ending);

    return FUNCTOR_SIBLINGS;
}

int Ending::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = dynamic_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We need to group the previous and this ending
        params->m_previousEnding->SetDrawingGrpId(params->m_drawingGrpId);
        this->SetDrawingGrpId(params->m_drawingGrpId);
        // Also set the previous ending to NULL to the grpId is _not_ incremented at the next measure
        // We need this because three or more endings might have to be grouped together
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv