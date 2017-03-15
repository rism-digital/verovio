/////////////////////////////////////////////////////////////////////////////
// Name:        elementpart.cpp
// Author:      Laurent Pugin
// Created:     2017
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "elementpart.h"

namespace vrv {

//----------------------------------------------------------------------------
// Flag
//----------------------------------------------------------------------------

Flag::Flag() : LayerElement("flag-")
{
    
    Reset();
}

Flag::~Flag()
{
}

void Flag::Reset()
{
    LayerElement::Reset();
}

//----------------------------------------------------------------------------
// NoteHead
//----------------------------------------------------------------------------

NoteHead::NoteHead() : LayerElement("notehead-")
{
    
    Reset();
    IsAttribute(true);
}

NoteHead::~NoteHead()
{
}

void NoteHead::Reset()
{
    LayerElement::Reset();
}
    
//----------------------------------------------------------------------------
// Stem
//----------------------------------------------------------------------------

Stem::Stem() : LayerElement("stem-"), AttStems(), AttStemsCmn()
{
    RegisterAttClass(ATT_STEMS);
    RegisterAttClass(ATT_STEMSCMN);
 
    Reset();
    IsAttribute(true);
}

Stem::~Stem()
{
}

void Stem::Reset()
{
    LayerElement::Reset();
    ResetStems();
    ResetStemsCmn();
}

} // namespace vrv
