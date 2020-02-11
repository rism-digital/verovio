/////////////////////////////////////////////////////////////////////////////
// Name:        fig.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "fig.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "svg.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Fig
//----------------------------------------------------------------------------

Fig::Fig() : TextElement("fig-"), AreaPosInterface()
{
    RegisterInterface(AreaPosInterface::GetAttClasses(), AreaPosInterface::IsInterface());

    Reset();
}

Fig::~Fig() {}

void Fig::Reset()
{
    TextElement::Reset();
    AreaPosInterface::Reset();
}

void Fig::AddChild(Object *child)
{
    if (child->Is(SVG)) {
        assert(dynamic_cast<Svg *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

//----------------------------------------------------------------------------
// Functors methods
//----------------------------------------------------------------------------

int Fig::AlignVertically(FunctorParams *functorParams)
{
    AlignVerticallyParams *params = dynamic_cast<AlignVerticallyParams *>(functorParams);
    assert(params);

    Svg *svg = dynamic_cast<Svg *>(this->FindDescendantByType(SVG));
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
