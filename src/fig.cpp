/////////////////////////////////////////////////////////////////////////////
// Name:        fig.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fig.h"

//----------------------------------------------------------------------------

#include <cassert>

//----------------------------------------------------------------------------

#include "functor.h"
#include "svg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig
//----------------------------------------------------------------------------

static const ClassRegistrar<Fig> s_factory("fig", FIG);

Fig::Fig() : TextElement(FIG, "fig-"), AreaPosInterface()
{
    this->RegisterInterface(AreaPosInterface::GetAttClasses(), AreaPosInterface::IsInterface());

    this->Reset();
}

Fig::~Fig() {}

void Fig::Reset()
{
    TextElement::Reset();
    AreaPosInterface::Reset();
}

bool Fig::IsSupportedChild(Object *child)
{
    if (child->Is(SVG)) {
        assert(dynamic_cast<Svg *>(child));
    }
    else {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

FunctorCode Fig::Accept(Functor &functor)
{
    return functor.VisitFig(this);
}

FunctorCode Fig::Accept(ConstFunctor &functor) const
{
    return functor.VisitFig(this);
}

FunctorCode Fig::AcceptEnd(Functor &functor)
{
    return functor.VisitFigEnd(this);
}

FunctorCode Fig::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFigEnd(this);
}

} // namespace vrv
