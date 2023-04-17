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

Label::Label() : Object(LABEL, "label-"), TextListInterface()
{
    this->Reset();
}

Label::~Label() {}

void Label::Reset()
{
    Object::Reset();
}

bool Label::IsSupportedChild(Object *child)
{
    if (child->Is({ LB, REND, TEXT })) {
        assert(dynamic_cast<TextElement *>(child));
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
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Label::Accept(MutableFunctor &functor)
{
    return functor.VisitLabel(this);
}

FunctorCode Label::Accept(ConstFunctor &functor) const
{
    return functor.VisitLabel(this);
}

FunctorCode Label::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitLabelEnd(this);
}

FunctorCode Label::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitLabelEnd(this);
}

} // namespace vrv
