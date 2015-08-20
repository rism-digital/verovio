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
    RegisterAttClass(ATT_CURVATURE);
    
    RegisterInterface( TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface() );
    
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
