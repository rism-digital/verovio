/////////////////////////////////////////////////////////////////////////////
// Name:        annotscore.cpp
// Author:      David Lewis
// Created:     2024
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "annotscore.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "devicecontext.h"
#include "doc.h"
#include "functor.h"
#include "text.h"
#include "verticalaligner.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// AnnotScore
//----------------------------------------------------------------------------

static const ClassRegistrar<AnnotScore> s_factory("annotScore", ANNOTSCORE);

AnnotScore::AnnotScore() : ControlElement(ANNOTSCORE, "annotscore-"), AttPlist()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_PLIST);

    this->Reset();
}

AnnotScore::~AnnotScore() {}

void AnnotScore::Reset()
{
    ControlElement::Reset();
    this->ResetPlist();
    TimeSpanningInterface::Reset();
}

bool AnnotScore::IsSupportedChild(Object *child)
{
    if (child->IsTextElement()) {
        assert(dynamic_cast<TextElement *>(child));
    }
    else if (child->Is(ANNOT)) {
        assert(dynamic_cast<AnnotScore *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// AnnotScore functor methods
//----------------------------------------------------------------------------

FunctorCode AnnotScore::Accept(Functor &functor)
{
    return functor.VisitAnnotScore(this);
}

FunctorCode AnnotScore::Accept(ConstFunctor &functor) const
{
    return functor.VisitAnnotScore(this);
}

FunctorCode AnnotScore::AcceptEnd(Functor &functor)
{
    return functor.VisitAnnotScoreEnd(this);
}

FunctorCode AnnotScore::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitAnnotScoreEnd(this);
}

} // namespace vrv
