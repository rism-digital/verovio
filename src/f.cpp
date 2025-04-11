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

F::F() : TextElement(FIGURE), TimeSpanningInterface(), AttExtender()
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

bool F::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else if (Object::IsEditorialElement(classId)) {
        return true;
    }
    else {
        return false;
    }
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
