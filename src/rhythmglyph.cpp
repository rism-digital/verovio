/////////////////////////////////////////////////////////////////////////////
// Name:        rhythmglyph.cpp
// Author:      Laurent Pugin
// Created:     2019
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "rhythmglyph.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// RhythmGlyph
//----------------------------------------------------------------------------

RhythmGlyph::RhythmGlyph() : LayerElement("element-"), AttNNumberLike()
{
    RegisterAttClass(ATT_NNUMBERLIKE);

    Reset();
}

RhythmGlyph::~RhythmGlyph()
{
}

void RhythmGlyph::Reset()
{
    LayerElement::Reset();
    ResetNNumberLike();
}

void RhythmGlyph::AddChild(Object *child)
{
    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

} // namespace vrv
