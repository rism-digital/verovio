/////////////////////////////////////////////////////////////////////////////
// Name:        fb.cpp
// Author:      Rodolfo Zitellini
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fb.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "f.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fb
//----------------------------------------------------------------------------

static const ClassRegistrar<Fb> s_factory("fb", FB);

Fb::Fb() : Object(FB, "fb-")
{

    this->Reset();
}

Fb::~Fb() {}

void Fb::Reset()
{
    Object::Reset();
}

bool Fb::IsSupportedChild(Object *child)
{
    if (child->Is(FIGURE)) {
        assert(dynamic_cast<F *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        return false;
    }
    return true;
}

FunctorCode Fb::Accept(MutableFunctor &functor)
{
    return functor.VisitFb(this);
}

FunctorCode Fb::Accept(ConstFunctor &functor) const
{
    return functor.VisitFb(this);
}

FunctorCode Fb::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitFbEnd(this);
}

FunctorCode Fb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFbEnd(this);
}

} // namespace vrv
