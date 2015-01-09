/////////////////////////////////////////////////////////////////////////////
// Name:        syl.cpp
// Author:      Laurent Pugin
// Created:     2014
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "syl.h"

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Syl
//----------------------------------------------------------------------------

Syl::Syl():
    LayerElement("syl-"),
    AttSylLog()
{
    Reset();
}

Syl::~Syl()
{
}
    
void Syl::Reset()
{
    LayerElement::Reset();
    ResetSylLog();
}

} // namespace vrv
