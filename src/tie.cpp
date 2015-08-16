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
    FloatingElement("tie-"), TimeSpanningInterface()
{
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
}

} // namespace vrv