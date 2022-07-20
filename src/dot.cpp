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

#include "functorparams.h"
#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

static const ClassRegistrar<Dot> s_factory("dot", DOT);

Dot::Dot() : LayerElement(DOT, "dot-"), PositionInterface(), AttColor(), AttDotLog()
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

int Dot::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = vrv_params_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    m_drawingPreviousElement = params->m_currentElement;
    params->m_lastDot = this;

    return FUNCTOR_CONTINUE;
}

int Dot::ResetData(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetData(functorParams);
    PositionInterface::InterfaceResetData(functorParams, this);

    m_drawingPreviousElement = NULL;
    m_drawingNextElement = NULL;

    return FUNCTOR_CONTINUE;
}

int Dot::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
