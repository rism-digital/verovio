/////////////////////////////////////////////////////////////////////////////
// Name:        label.cpp
// Author:      Laurent Pugin
// Created:     19/06/2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "label.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "text.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Label
//----------------------------------------------------------------------------

static const ClassRegistrar<Label> s_factory("label", LABEL);

Label::Label() : Object(LABEL), TextListInterface()
{
    this->Reset();
}

Label::~Label() {}

void Label::Reset()
{
    Object::Reset();
}

bool Label::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ LB, REND, TEXT };

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
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Label::Accept(Functor &functor)
{
    return functor.VisitLabel(this);
}

FunctorCode Label::Accept(ConstFunctor &functor) const
{
    return functor.VisitLabel(this);
}

FunctorCode Label::AcceptEnd(Functor &functor)
{
    return functor.VisitLabelEnd(this);
}

FunctorCode Label::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLabelEnd(this);
}

} // namespace vrv
