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
    MeasureElement("tie-"), TimeSpanningInterface()
{
    RegisterInterface( TimeSpanningInterface::GetAttClasses(), TimeSpanningInterface::IsInterface() );
    
    Reset();
}


Tie::~Tie()
{
}
    
void Tie::Reset()
{
    MeasureElement::Reset();
    TimeSpanningInterface::Reset();
}

//----------------------------------------------------------------------------
// Tie functor methods
//----------------------------------------------------------------------------

int Tie::PrepareTimeSpanning( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    return TimeSpanningInterface::PrepareTimeSpanning(params, this);
}


int Tie::FillStaffCurrentTimeSpanning( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::FillStaffCurrentTimeSpanning(params, this);
}
    
int Tie::ResetDarwing( ArrayPtrVoid *params )
{
    // Pass it to the pseudo functor of the interface
    return  TimeSpanningInterface::ResetDrawing(params, this);
};

} // namespace vrv