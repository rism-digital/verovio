/////////////////////////////////////////////////////////////////////////////
// Name:        tie.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "tie.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

namespace vrv {
    
//----------------------------------------------------------------------------
// Tie
//----------------------------------------------------------------------------

Tie::Tie():
    FloatingElement("tie-"), TimeSpanningInterface(),
    AttCurvature()
{
    RegisterAttClass(ATT_CURVATURE);
    
    RegisterInterface( TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface() );
    
    Reset();
}


Tie::~Tie()
{
}
    
void Tie::Reset()
{
    FloatingElement::Reset();
    TimeSpanningInterface::Reset();
    ResetCurvature();
}

} // namespace vrv