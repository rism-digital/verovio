/////////////////////////////////////////////////////////////////////////////
// Name:        textlayoutelement.cpp
// Author:      Laurent Pugin
// Created:     2023
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "textlayoutelement.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "doc.h"
#include "functor.h"

namespace vrv {

//----------------------------------------------------------------------------
// TextLayoutElement
//----------------------------------------------------------------------------

TextLayoutElement::TextLayoutElement() : Object()
{
    this->Reset();
}

TextLayoutElement::~TextLayoutElement() {}

void TextLayoutElement::Reset()
{
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode TextLayoutElement::Accept(Functor &functor)
{
    return functor.VisitTextLayoutElement(this);
}

FunctorCode TextLayoutElement::Accept(ConstFunctor &functor) const
{
    return functor.VisitTextLayoutElement(this);
}

FunctorCode TextLayoutElement::AcceptEnd(Functor &functor)
{
    return functor.VisitTextLayoutElementEnd(this);
}

FunctorCode TextLayoutElement::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitTextLayoutElementEnd(this);
}

} // namespace vrv
