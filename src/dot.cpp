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

Dot::Dot() : LayerElement("dot-"), PositionInterface()
{
    RegisterInterface(PositionInterface::GetAttClasses(), PositionInterface::IsInterface());

    Reset();
}

Dot::~Dot()
{
}

void Dot::Reset()
{
    LayerElement::Reset();
    PositionInterface::Reset();
    m_drawingNote = NULL;
}

//----------------------------------------------------------------------------
// Functor methods
//----------------------------------------------------------------------------

int Dot::PreparePointersByLayer(FunctorParams *functorParams)
{
    PreparePointersByLayerParams *params = dynamic_cast<PreparePointersByLayerParams *>(functorParams);
    assert(params);

    m_drawingNote = params->m_currentNote;

    return FUNCTOR_CONTINUE;
}

int Dot::ResetDrawing(FunctorParams *functorParams)
{
    this->m_drawingNote = NULL;
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
