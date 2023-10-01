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

Ending::Ending()
    : SystemElement(ENDING, "ending-")
    , SystemMilestoneInterface()
    , AttLabelled()
    , AttLineRend()
    , AttLineRendBase()
    , AttNNumberLike()
{
    this->RegisterAttClass(ATT_LABELLED);
    this->RegisterAttClass(ATT_LINEREND);
    this->RegisterAttClass(ATT_LINERENDBASE);
    this->RegisterAttClass(ATT_NNUMBERLIKE);

    this->Reset();
}

Ending::~Ending() {}

void Ending::Reset()
{
    SystemElement::Reset();
    SystemMilestoneInterface::Reset();
    this->ResetLabelled();
    this->ResetLineRend();
    this->ResetLineRendBase();
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

FunctorCode Ending::Accept(Functor &functor)
{
    return functor.VisitEnding(this);
}

FunctorCode Ending::Accept(ConstFunctor &functor) const
{
    return functor.VisitEnding(this);
}

FunctorCode Ending::AcceptEnd(Functor &functor)
{
    return functor.VisitEndingEnd(this);
}

FunctorCode Ending::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitEndingEnd(this);
}

} // namespace vrv
