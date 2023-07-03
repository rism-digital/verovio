/////////////////////////////////////////////////////////////////////////////
// Name:        f.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "f.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// F (Figure)
//----------------------------------------------------------------------------

static const ClassRegistrar<F> s_factory("f", FIGURE);

F::F() : TextElement(FIGURE, "f-"), TimeSpanningInterface(), AttExtender()
{
    this->RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    this->RegisterAttClass(ATT_EXTENDER);

    this->Reset();
}

F::~F() {}

void F::Reset()
{
    TextElement::Reset();
    TimeSpanningInterface::Reset();
    this->ResetExtender();
}

bool F::IsSupportedChild(Object *child)
{
    if (child->Is(TEXT)) {
        assert(dynamic_cast<Text *>(child));
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
// F functor methods
//----------------------------------------------------------------------------

FunctorCode F::Accept(Functor &functor)
{
    return functor.VisitF(this);
}

FunctorCode F::Accept(ConstFunctor &functor) const
{
    return functor.VisitF(this);
}

FunctorCode F::AcceptEnd(Functor &functor)
{
    return functor.VisitFEnd(this);
}

FunctorCode F::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFEnd(this);
}

} // namespace vrv
