/////////////////////////////////////////////////////////////////////////////
// Name:        slur.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "slur.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {

//----------------------------------------------------------------------------
// Slur
//----------------------------------------------------------------------------

Slur::Slur():
    FloatingElement("slur-"), TimeSpanningInterface(),
    AttCurvature()
{
    RegisterInterface(TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface());
    RegisterAttClass(ATT_CURVATURE);

    Reset();
}


Slur::~Slur()
{
}
        
void Slur::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetCurvature();
}
    
} // namespace vrv
