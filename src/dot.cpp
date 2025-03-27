/////////////////////////////////////////////////////////////////////////////
// Name:        dot.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dot.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

static const ClassRegistrar<Dot> s_factory("dot", DOT);

Dot::Dot() : LayerElement(DOT), PositionInterface(), AttColor(), AttDotLog()
{
    this->RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    this->RegisterAttClass(ATT_COLOR);
    this->RegisterAttClass(ATT_DOTLOG);

    this->Reset();
}

Dot::~Dot() {}

void Dot::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    this->ResetColor();
    this->ResetDotLog();

    m_drawingPreviousElement = NULL;
    m_drawingNextElement = NULL;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

FunctorCode Dot::Accept(Functor &functor)
{
    return functor.VisitDot(this);
}

FunctorCode Dot::Accept(ConstFunctor &functor) const
{
    return functor.VisitDot(this);
}

FunctorCode Dot::AcceptEnd(Functor &functor)
{
    return functor.VisitDotEnd(this);
}

FunctorCode Dot::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitDotEnd(this);
}

} // namespace vrv
