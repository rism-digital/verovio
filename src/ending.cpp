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
#include "functor.h"
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

FunctorCode Ending::Accept(MutableFunctor &functor)
{
    return functor.VisitEnding(this);
}

FunctorCode Ending::Accept(ConstFunctor &functor) const
{
    return functor.VisitEnding(this);
}

FunctorCode Ending::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitEndingEnd(this);
}

FunctorCode Ending::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitEndingEnd(this);
}

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

} // namespace vrv
