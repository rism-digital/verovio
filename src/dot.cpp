/////////////////////////////////////////////////////////////////////////////
// Name:        dot.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "dot.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "functorparams.h"
#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

Dot::Dot() : LayerElement("dot-"), PositionInterface(), AttColor(), AttDotLog()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());
    RegisterAttClass(ATT_COLOR);
    RegisterAttClass(ATT_DOTLOG);

    Reset();
}

Dot::~Dot() {}

void Dot::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    ResetColor();
    ResetDotLog();

    m_drawingNote = NULL;
    m_drawingNextElement = NULL;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Dot::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = vrv_params_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    m_drawingNote = params->m_currentNote;
    params->m_lastDot = this;

    return FUNCTOR_CONTINUE;
}

int Dot::ResetDrawing(FunctorParams *functorParams)
{
    // Call parent one too
    LayerElement::ResetDrawing(functorParams);
    PositionInterface::InterfaceResetDrawing(functorParams, this);

    this->m_drawingNote = NULL;
    this->m_drawingNextElement = NULL;

    return FUNCTOR_CONTINUE;
}

int Dot::ResetHorizontalAlignment(FunctorParams *functorParams)
{
    LayerElement::ResetHorizontalAlignment(functorParams);
    PositionInterface::InterfaceResetHorizontalAlignment(functorParams, this);

    return FUNCTOR_CONTINUE;
}

} // namespace vrv
