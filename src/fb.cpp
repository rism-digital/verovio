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

Fb::Fb() : Object(FB)
{

    this->Reset();
}

Fb::~Fb() {}

void Fb::Reset()
{
    Object::Reset();
}

bool Fb::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ FIGURE };

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

FunctorCode Fb::Accept(Functor &functor)
{
    return functor.VisitFb(this);
}

FunctorCode Fb::Accept(ConstFunctor &functor) const
{
    return functor.VisitFb(this);
}

FunctorCode Fb::AcceptEnd(Functor &functor)
{
    return functor.VisitFbEnd(this);
}

FunctorCode Fb::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFbEnd(this);
}

} // namespace vrv
