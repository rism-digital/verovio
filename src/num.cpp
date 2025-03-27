/////////////////////////////////////////////////////////////////////////////
// Name:        num.cpp
// Author:      Laurent Pugin
// Created:     2018
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "num.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Num
//----------------------------------------------------------------------------

static const ClassRegistrar<Num> s_factory("num", NUM);

Num::Num() : TextElement(NUM)
{
    this->Reset();
}

Num::~Num() {}

void Num::Reset()
{
    m_currentText.SetParent(this);
    m_currentText.SetText(U"");
}

bool Num::IsSupportedChild(ClassId classId)
{
    static const std::vector<ClassId> supported{ TEXT };

    if (std::find(supported.begin(), supported.end(), classId) != supported.end()) {
        return true;
    }
    else {
        return false;
    }
}

FunctorCode Num::Accept(Functor &functor)
{
    return functor.VisitNum(this);
}

FunctorCode Num::Accept(ConstFunctor &functor) const
{
    return functor.VisitNum(this);
}

FunctorCode Num::AcceptEnd(Functor &functor)
{
    return functor.VisitNumEnd(this);
}

FunctorCode Num::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitNumEnd(this);
}

} // namespace vrv
