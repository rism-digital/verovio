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

#include "note.h"

namespace vrv {

//----------------------------------------------------------------------------
// Dot
//----------------------------------------------------------------------------

Dot::Dot():
    LayerElement("dot-"), PositionInterface()
{
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
// Functors methods
//----------------------------------------------------------------------------
    
int Dot::PreparePointersByLayer( ArrayPtrVoid params )
{
    // param 0: the current Note
    Note **currentNote = static_cast<Note**>(params[0]);
    
    m_drawingNote = (*currentNote);
    
    return FUNCTOR_CONTINUE;
}
    
int Dot::ResetDarwing( ArrayPtrVoid params )
{
    this->m_drawingNote = NULL;
    return FUNCTOR_CONTINUE;
};

} // namespace vrv
