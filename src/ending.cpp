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
    : SystemElement(ENDING)
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

bool Ending::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ MEASURE, SCOREDEF };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsSystemElement(classId)) {
        // without this we would be allowing ending within ending, which is wrong
        return (classId != ENDING);
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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
