/////////////////////////////////////////////////////////////////////////////
// Name:        ending.cpp
// Author:      Laurent Pugin
// Created:     14/07/2016
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "ending.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functorparams.h"
#include "measure.h"
#include "scoredef.h"
#include "section.h"
#include "system.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Ending
//----------------------------------------------------------------------------

static const ClassRegistrar<Ending> s_factory("ending", ENDING);

Ending::Ending() : SystemElement(ENDING, "ending-"), SystemMilestoneInterface(), AttLineRend(), AttNNumberLike()
{
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_NINTEGER);

    this->Reset();
}

Ending::~Ending() {}

void Ending::Reset()
{
    SystemElement::Reset();
    SystemMilestoneInterface::Reset();
    this->ResetLineRend();
    this->ResetNNumberLike();
}

bool Ending::IsSupportedChild(Object *child)
{
    if (child->Is(MEASURE)) {
        assert(dynamic_cast<Measure *>(child));
    }
    else if (child->Is(SCOREDEF)) {
        assert(dynamic_cast<ScoreDef *>(child));
    }
    else if (child->IsSystemElement()) {
        assert(dynamic_cast<SystemElement *>(child));
        // here we are actually allowing ending within ending, which is wrong
        if (child->Is(ENDING)) {
            return false;
        }
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
// Ending functor methods
//----------------------------------------------------------------------------

int Ending::ConvertToPageBased(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    assert(params->m_currentSystem);
    this->MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int Ending::ConvertToPageBasedEnd(FunctorParams *functorParams)
{
    ConvertToPageBasedParams *params = vrv_params_cast<ConvertToPageBasedParams *>(functorParams);
    assert(params);

    ConvertToPageBasedMilestone(this, params->m_currentSystem);

    return FUNCTOR_CONTINUE;
}

int Ending::PrepareMilestones(FunctorParams *functorParams)
{
    PrepareMilestonesParams *params = vrv_params_cast<PrepareMilestonesParams *>(functorParams);
    assert(params);

    // Endings should always have an SystemMilestoneEnd
    assert(this->IsSystemMilestone());

    this->SystemMilestoneInterface::InterfacePrepareMilestones(functorParams);

    params->m_currentEnding = this;

    return FUNCTOR_CONTINUE;
}

int Ending::ResetData(FunctorParams *functorParams)
{
    FloatingObject::ResetData(functorParams);

    this->SystemMilestoneInterface::InterfaceResetData(functorParams);

    return FUNCTOR_CONTINUE;
}

int Ending::CastOffSystems(FunctorParams *functorParams)
{
    CastOffSystemsParams *params = vrv_params_cast<CastOffSystemsParams *>(functorParams);
    assert(params);

    // Since the functor returns FUNCTOR_SIBLINGS we should never go lower than the system children
    assert(dynamic_cast<System *>(this->GetParent()));

    // Special case where we use the Relinquish method.
    // We want to move the measure to the currentSystem. However, we cannot use DetachChild
    // from the content System because this screws up the iterator. Relinquish gives up
    // the ownership of the Measure - the contentSystem will be deleted afterwards.
    Ending *ending = dynamic_cast<Ending *>(params->m_contentSystem->Relinquish(this->GetIdx()));
    // move as pending since we want it at the beginning of the system in case of system break coming
    params->m_pendingElements.push_back(ending);

    return FUNCTOR_SIBLINGS;
}

int Ending::CastOffEncoding(FunctorParams *functorParams)
{
    CastOffEncodingParams *params = vrv_params_cast<CastOffEncodingParams *>(functorParams);
    assert(params);

    MoveItselfTo(params->m_currentSystem);

    return FUNCTOR_SIBLINGS;
}

int Ending::PrepareFloatingGrps(FunctorParams *functorParams)
{
    PrepareFloatingGrpsParams *params = vrv_params_cast<PrepareFloatingGrpsParams *>(functorParams);
    assert(params);

    if (params->m_previousEnding) {
        // We need to group the previous and this ending - the previous one should have a grpId
        if (params->m_previousEnding->GetDrawingGrpId() == 0) {
            LogDebug("Something went wrong with the gouping of the endings");
        }
        this->SetDrawingGrpId(params->m_previousEnding->GetDrawingGrpId());
        // Also set the previous ending to NULL to the grpId is _not_ incremented at the next measure
        // We need this because three or more endings might have to be grouped together
        params->m_previousEnding = NULL;
    }

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
