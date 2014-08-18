/////////////////////////////////////////////////////////////////////////////
// Name:        verse.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "verse.h"

//----------------------------------------------------------------------------

#include "syl.h"

namespace vrv {

//----------------------------------------------------------------------------
// Verse
//----------------------------------------------------------------------------

Verse::Verse():
    DocObject("verse-")
{
}

Verse::~Verse()
{
}

void Verse::AddSyl(Syl *syl) {
    
    syl->SetParent( this );
    m_children.push_back(syl);
    Modify();
}

} // namespace vrv
