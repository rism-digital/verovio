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

Num::Num() : TextElement(NUM, "num-")
{
    this->Reset();
}

Num::~Num() {}

void Num::Reset()
{
    m_currentText.SetParent(this);
    m_currentText.SetText(U"");
}

bool Num::IsSupportedChild(Object *child)
{
    if (child->Is(TEXT)) {
        assert(dynamic_cast<Text *>(child));
    }
    else {
        return false;
    }
    return true;
}

FunctorCode Num::Accept(MutableFunctor &functor)
{
    return functor.VisitNum(this);
}

FunctorCode Num::Accept(ConstFunctor &functor) const
{
    return functor.VisitNum(this);
}

FunctorCode Num::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitNumEnd(this);
}

FunctorCode Num::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitNumEnd(this);
}

} // namespace vrv
