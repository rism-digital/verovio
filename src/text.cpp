/////////////////////////////////////////////////////////////////////////////
// Name:        text.cpp
// Author:      Laurent Pugin
// Created:     2015
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "text.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "editorial.h"
#include "functor.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Text
//----------------------------------------------------------------------------

Text::Text() : TextElement(TEXT, "text-")
{
    this->Reset();
}

Text::~Text() {}

void Text::Reset()
{
    TextElement::Reset();
}

FunctorCode Text::Accept(MutableFunctor &functor)
{
    return functor.VisitText(this);
}

FunctorCode Text::Accept(ConstFunctor &functor) const
{
    return functor.VisitText(this);
}

FunctorCode Text::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitTextEnd(this);
}

FunctorCode Text::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTextEnd(this);
}

} // namespace vrv
