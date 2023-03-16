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
#include "functorparams.h"
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

FunctorCode Fig::Accept(MutableFunctor &functor)
{
    return functor.VisitFig(this);
}

FunctorCode Fig::Accept(ConstFunctor &functor) const
{
    return functor.VisitFig(this);
}

FunctorCode Fig::AcceptEnd(MutableFunctor &functor)
{
    return functor.VisitFigEnd(this);
}

FunctorCode Fig::AcceptEnd(ConstFunctor &functor) const
{
    return functor.VisitFigEnd(this);
}

int Fig::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = vrv_params_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    Svg *svg = vrv_cast<Svg *>(this->FindDescendantByType(SVG));
    int width = (svg) ? svg->GetWidth() : 0;

    if (this->GetHalign() == HORIZONTALALIGNMENT_right) {
        this->SetDrawingXRel(params->m_pageWidth - width);
    }
    else if (this->GetHalign() == HORIZONTALALIGNMENT_center) {
        this->SetDrawingXRel((params->m_pageWidth - width) / 2);
    }

    return FUNCTOR_SIBLINGS;
}

} // namespace vrv
